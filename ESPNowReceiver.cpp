
#include "ESPNowReceiver.h"
#include "kmpf_constants.h"

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

namespace kmpf {

    ESPNowReceiver::ESPNowReceiver(std::string receiver_name, atmt::Joystick* linked_joystick, atmt::ESPNowHandler* linked_handler):
        m_broadcast_loop{ atmt::TimeoutManager(consts::ESPNow::k_BroadcastDelaySec) },
        m_heartbeat_loop{ atmt::TimeoutManager(consts::ESPNow::k_HeartbeatDelaySec) },
        m_led_1_transmit_loop{ atmt::TimeoutManager(consts::LED::k_BlinkRateSec) },
        m_led_2_receive_loop{ atmt::TimeoutManager(consts::LED::k_BlinkRateSec) },
        m_last_packet_timestamp{ atmt::Timestamp(0) },
        m_is_connected_to_controller{ false },
        m_receiver_name{ receiver_name },
        m_linked_joystick{ linked_joystick },
        m_linked_espnow_handler{ linked_handler },
        m_initialized_without_error{ false }
    {
        
    }
    ESPNowReceiver::~ESPNowReceiver() {

    };

    void ESPNowReceiver::init() {
        pinMode(consts::LED::k_LED1_Transmit, OUTPUT);
        pinMode(consts::LED::k_LED2_Received, OUTPUT);
        
        digitalWrite(consts::LED::k_LED1_Transmit, LOW);
        digitalWrite(consts::LED::k_LED2_Received, LOW);

        // ESPNow will be init() by ESPNowHandler as long as it is registered

        if (!m_linked_espnow_handler) {
            atmt::platform_printf("Linked ESPNowHandler Not Valid");
            return;
        }
        if (!m_linked_joystick) {
            atmt::platform_printf("Linked Joystick Not Valid");
            return;
        }
        m_initialized_without_error = true;
    };
    void ESPNowReceiver::systemPeriodic() {
        if (!m_initialized_without_error) return;

        if (m_led_1_transmit_loop.checkTimeoutWithoutReset()) {
            digitalWrite(consts::LED::k_LED1_Transmit, LOW);
        }
        if (m_led_2_receive_loop.checkTimeoutWithoutReset()) {
            digitalWrite(consts::LED::k_LED2_Received, LOW);
        }

        while (m_linked_espnow_handler->packet.availableMessages()) {
            uint8_t prefix;
            bool result = m_linked_espnow_handler->packet.peekNextMessagePrefix(prefix);
            if (!result) continue;

            switch (static_cast<PacketPrefixes>(prefix)) {
                case PacketPrefixes::Receiver_InitiatePair:
                    // Ignore
                    break;
                case PacketPrefixes::Receiver_AcknowledgePair:
                    // Ignore
                    break;
                case PacketPrefixes::Receiver_SendHeartbeat:
                    // Ignore
                    break;
                case PacketPrefixes::Controller_AcknowledgePair:
                    // Ignore if already connected
                    if (!m_is_connected_to_controller) {
                        uint8_t buffer[sizeof(Packet_PairingPacket)];
                        uint8_t length;

                        m_linked_espnow_handler->packet.popNextMessagePrefixed(prefix, buffer, length); // No need to memcpy buffer
                        if (length != sizeof(Packet_PairingPacket)) break;
                        const Packet_PairingPacket& packet = *reinterpret_cast<Packet_PairingPacket*>(buffer);

                        m_linked_espnow_handler->setTargetMACAddress(packet.sender_mac_address);
                        m_is_connected_to_controller = true;
                        acknowledgePair();
                    }
                    break;
                case PacketPrefixes::Controller_SendJoystickState:
                    // Ignore if not connected
                    if (m_is_connected_to_controller) {
                        uint8_t buffer[sizeof(atmt::JoystickState)];
                        uint8_t length;

                        m_linked_espnow_handler->packet.popNextMessagePrefixed(prefix, buffer, length); // No need to memcpy buffer
                        if (length != sizeof(atmt::JoystickState)) break;
                        const atmt::JoystickState& packet = *reinterpret_cast<atmt::JoystickState*>(buffer);

                        m_linked_joystick->updateState(packet);
                        m_last_packet_timestamp = atmt::getSystemTime();
                    }
                    break;
            }
        }
        
        if (!m_is_connected_to_controller || m_last_packet_timestamp.getTimeDifference(atmt::getSystemTime()) > consts::ESPNow::k_ESPNowDisableTimeoutSec) {
            if (m_is_connected_to_controller) { // Timeout disconnect
                m_linked_espnow_handler->setTargetMACAddress(atmt::k_ESPNowBroadcastAddress);
                m_is_connected_to_controller = false;
                m_linked_joystick->updateState(zeroJoystickInputs());
            }

            broadcastLocateController();
        }else {
            sendHeartbeat();
        }
    };
    void ESPNowReceiver::disabledPeriodic() {};
    void ESPNowReceiver::autonomousPeriodic() {};
    void ESPNowReceiver::teleopPeriodic() {};

    void ESPNowReceiver::broadcastLocateController() {
        if (!m_initialized_without_error) return;

        if (m_broadcast_loop.checkTimeout()) { // Limits to once per k_BroadcastDelaySec (0.5 seconds)
            Packet_PairingPacket message{ };
            memcpy(message.sender_mac_address, m_linked_espnow_handler->GetMACAddress(), 6);
            // memcpy(message.sender_name, m_receiver_name.data(), std::min(m_receiver_name.size(), 10u));
            strncpy(message.sender_name, m_receiver_name.c_str(), sizeof(message.sender_name)-1);
            
            // int result = esp_now_send(consts::ESPNow::k_ESPNowBroadcastAddress, reinterpret_cast<const uint8_t*>(m_receiver_name.data()), m_receiver_name.size());
            bool result = m_linked_espnow_handler->packet.sendMessagePrefixedAll(static_cast<uint8_t>(PacketPrefixes::Receiver_InitiatePair), reinterpret_cast<uint8_t*>(&message), sizeof(Packet_PairingPacket));
            // if (result == ESP_OK) {
            if (result) {
                // int current_led_status = digitalRead(consts::LED::k_LED1_Transmit);
                if (m_led_1_transmit_loop.checkTimeout()) {
                    digitalWrite(consts::LED::k_LED1_Transmit, HIGH); // Blink LED
                }
                atmt::platform_printf("Broadcast Message Sent Successfully\n");
            }else {
                if (m_led_1_transmit_loop.checkTimeout()) {
                    digitalWrite(consts::LED::k_LED1_Transmit, LOW);
                }
                atmt::platform_printf("Broadcast Message Failure: %02x\n", result);
            }
        }
    };
    void ESPNowReceiver::acknowledgePair() {
        if (!m_initialized_without_error) return;

        bool result = m_linked_espnow_handler->packet.sendMessageAll(static_cast<uint8_t>(PacketPrefixes::Receiver_AcknowledgePair));
        if (result) {
            if (m_led_1_transmit_loop.checkTimeout()) {
                digitalWrite(consts::LED::k_LED1_Transmit, HIGH); // Blink LED
            }
            atmt::platform_printf("Acknowledge Pair Message Sent Successfully\n");
        }else {
            if (m_led_1_transmit_loop.checkTimeout()) {
                digitalWrite(consts::LED::k_LED1_Transmit, LOW);
            }
            atmt::platform_printf("Heartbeat Message Failure: %02x\n", result);
        }
    };
    void ESPNowReceiver::sendHeartbeat() {
        if (!m_initialized_without_error) return;

        if (m_heartbeat_loop.checkTimeout()) { // Limits to once per k_BroadcastDelaySec (0.5 seconds)
            // int result = esp_now_send(consts::ESPNow::k_ESPNowBroadcastAddress, reinterpret_cast<const uint8_t*>(m_receiver_name.data()), m_receiver_name.size());
            bool result = m_linked_espnow_handler->packet.sendMessageAll(static_cast<uint8_t>(PacketPrefixes::Receiver_SendHeartbeat));
            // if (result == ESP_OK) {
            if (result) {
                // int current_led_status = digitalRead(consts::LED::k_LED1_Transmit);
                if (m_led_1_transmit_loop.checkTimeout()) {
                    digitalWrite(consts::LED::k_LED1_Transmit, HIGH); // Blink LED
                }
                // atmt::platform_printf("Heartbeat Message Sent Successfully\n");
            }else {
                if (m_led_1_transmit_loop.checkTimeout()) {
                    digitalWrite(consts::LED::k_LED1_Transmit, LOW);
                }
                atmt::platform_printf("Heartbeat Message Failure: %02x\n", result);
            }
        }
    };
    atmt::JoystickState ESPNowReceiver::zeroJoystickInputs() {
        atmt::JoystickState new_state{ };

        return new_state;
    };

};
