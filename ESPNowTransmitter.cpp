
#include "ESPNowTransmitter.h"
#include "kmpf_constants.h"

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

namespace kmpf {

    ESPNowTransmitter::ESPNowTransmitter(atmt::ESPNowHandler* linked_handler, std::function<atmt::JoystickState()> state_getter):
        m_send_state_loop{ atmt::TimeoutManager(consts::ESPNow::k_SendStateDelaySec) },
        m_last_packet_timestamp{ atmt::Timestamp(0) },
        m_is_connected_to_receiver{ false },
        m_connected_receiver_address{ 0 },
        m_known_receivers{ },
        m_linked_espnow_handler{ linked_handler },
        m_state_getter{ state_getter },
        m_initialized_without_error{ false }
    {
        
    }
    ESPNowTransmitter::~ESPNowTransmitter() {

    };

    void ESPNowTransmitter::init() {

        // ESPNow will be init() by ESPNowHandler as long as it is registered

        if (!m_linked_espnow_handler) {
            atmt::platform_printf("Linked ESPNowHandler Not Valid");
            return;
        }
        m_initialized_without_error = true;
    };
    void ESPNowTransmitter::systemPeriodic() {
        if (!m_initialized_without_error) return;

        while (m_linked_espnow_handler->packet.availableMessages()) {
            uint8_t prefix;
            bool result = m_linked_espnow_handler->packet.peekNextMessagePrefix(prefix);
            if (!result) break;

            switch (static_cast<PacketPrefixes>(prefix)) {
                case PacketPrefixes::Receiver_InitiatePair:
                    // Ignore if already connected
                    if (!m_is_connected_to_receiver) {
                        uint8_t buffer[sizeof(Packet_PairingPacket)];
                        uint8_t length;
                        uint8_t sender;

                        m_linked_espnow_handler->packet.popNextMessagePrefixed(prefix, buffer, length, sender); // No need to memcpy buffer
                        if (length != sizeof(Packet_PairingPacket)) break;
                        const Packet_PairingPacket& packet = *reinterpret_cast<Packet_PairingPacket*>(buffer);

                        bool already_known = false;
                        for (const KnownReceiver &receiver : m_known_receivers) {
                            if (strcmp(packet.sender_name, receiver.name.data()) == 0 && memcmp(packet.sender_mac_address, receiver.mac_address, 6) == 0 && receiver.receiver_address == sender) {
                                already_known = true;
                                break;
                            }
                        }
                        if (!already_known) {
                            KnownReceiver new_receiver;
                            memcpy(new_receiver.mac_address, packet.sender_mac_address, 6);
                            new_receiver.receiver_address = sender;
                            new_receiver.name = packet.sender_name;
                            m_known_receivers.push_back(new_receiver);
                        }
                    }
                    break;
                case PacketPrefixes::Receiver_AcknowledgePair:
                    // Ignore if not pending a connection
                    if (m_attempting_connect_to_receiver && !m_is_connected_to_receiver) {
                        uint8_t buffer[sizeof(Packet_PairingPacket)];
                        uint8_t length;
                        uint8_t sender;

                        m_linked_espnow_handler->packet.popNextMessage(buffer, length, sender); // One byte message
                        if (length != 1) break;
                        if (sender != m_connected_receiver_address) break;
                        
                        m_is_connected_to_receiver = true;
                    }
                    break;
                case PacketPrefixes::Receiver_SendHeartbeat:
                    // Ignore if not connected
                    if (m_is_connected_to_receiver) {
                        uint8_t buffer[sizeof(Packet_PairingPacket)];
                        uint8_t length;
                        uint8_t sender;

                        m_linked_espnow_handler->packet.popNextMessage(buffer, length, sender); // One byte message
                        if (length != 1) break;
                        if (sender != m_connected_receiver_address) break;
                        
                        m_last_packet_timestamp = atmt::getSystemTime();
                    }
                    break;
                case PacketPrefixes::Controller_AcknowledgePair:
                    // Ignore
                    break;
                case PacketPrefixes::Controller_SendJoystickState:
                    // Ignore
                    break;
            }
        }
        
        if (!m_is_connected_to_receiver || m_last_packet_timestamp.getTimeDifference(atmt::getSystemTime()) > consts::ESPNow::k_ESPNowDisableTimeoutSec) {
            if (m_is_connected_to_receiver) { // Timeout disconnect
                m_linked_espnow_handler->setTargetMACAddress(atmt::k_ESPNowBroadcastAddress);
                m_attempting_connect_to_receiver = false;
                m_is_connected_to_receiver = false;
            }

            // Either no connection or pending handshake
        }else {
            sendState();
        }
    };
    void ESPNowTransmitter::disabledPeriodic() {};
    void ESPNowTransmitter::autonomousPeriodic() {};
    void ESPNowTransmitter::teleopPeriodic() {};

    const std::vector<KnownReceiver>& ESPNowTransmitter::getKnownReceivers() {
        return m_known_receivers;
    }
    void ESPNowTransmitter::pairWithReceiver(KnownReceiver receiver) {
        m_linked_espnow_handler->setTargetMACAddress(receiver.mac_address);
        m_is_connected_to_receiver = false;
        m_attempting_connect_to_receiver = true;
        m_connected_receiver_address = receiver.receiver_address;
        acknowledgePair();
    }
    void ESPNowTransmitter::acknowledgePair() {
        if (!m_initialized_without_error) return;

        Packet_PairingPacket message{ };
        memcpy(message.sender_mac_address, m_linked_espnow_handler->GetSystemMACAddress(), 6);
        // memcpy(message.sender_name, m_receiver_name.data(), std::min(m_receiver_name.size(), 10u));
        
        bool result = m_linked_espnow_handler->packet.sendMessagePrefixed(m_connected_receiver_address, static_cast<uint8_t>(PacketPrefixes::Controller_AcknowledgePair), reinterpret_cast<uint8_t*>(&message), sizeof(Packet_PairingPacket));
        if (result) {
            atmt::platform_printf("Acknowledge Pair Message Sent Successfully\n");
        }else {
            atmt::platform_printf("Acknowledge Pair Message Failure: %02x\n", result);
        }
    };
    void ESPNowTransmitter::sendState() {
        if (!m_initialized_without_error) return;

        if (m_send_state_loop.checkTimeout()) { // Limits to once per k_SendStateDelaySec (0.05 seconds)
            atmt::JoystickState message = m_state_getter();

            bool result = m_linked_espnow_handler->packet.sendMessagePrefixed(m_connected_receiver_address, static_cast<uint8_t>(PacketPrefixes::Controller_SendJoystickState), reinterpret_cast<uint8_t*>(&message), sizeof(atmt::JoystickState));
            if (result) {
                // atmt::platform_printf("State Packet Message Sent Successfully\n");
            }else {
                atmt::platform_printf("State Packet Message Failure: %02x\n", result);
            }
        }
    };
    
    bool ESPNowTransmitter::isAttemptingConnection() {
        return m_attempting_connect_to_receiver && !m_is_connected_to_receiver;
    };
    bool ESPNowTransmitter::isConnected() {
        return m_is_connected_to_receiver;
    };

};
