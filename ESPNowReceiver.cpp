
#include "ESPNowReceiver.h"
#include "kmpf_constants.h"

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

namespace kmpf {

    ESPNowReceiver::ESPNowReceiver(std::string receiver_name, atmt::Joystick* linked_joystick):
        m_broadcast_loop{ atmt::TimeoutManager(consts::ESPNow::k_BroadcastDelaySec) },
        m_led_1_transmit_loop{ atmt::TimeoutManager(consts::LED::k_BlinkRateSec) },
        m_led_2_receive_loop{ atmt::TimeoutManager(consts::LED::k_BlinkRateSec) },
        m_last_packet_timestamp{ atmt::Timestamp(0) },
        m_is_connected_to_controller{ false },
        m_pending_packet{ false },
        m_receiver_name{ receiver_name },
        m_receiver_mac_address{ },
        m_linked_joystick{ linked_joystick },
        m_initialized_without_error{ false }
    {
        if (ESPNowReceiver::Instance != nullptr) {
            atmt::platform_printf("ESPNowReceiver Already Initialized, Replacing Default Instance");
        }
        ESPNowReceiver::Instance = this;
    }
    ESPNowReceiver::~ESPNowReceiver() {

    };

    void ESPNowReceiver::init() {
        pinMode(consts::LED::k_LED1_Transmit, OUTPUT);
        pinMode(consts::LED::k_LED2_Received, OUTPUT);
        
        digitalWrite(consts::LED::k_LED1_Transmit, LOW);
        digitalWrite(consts::LED::k_LED2_Received, LOW);

        WiFi.mode(WIFI_MODE_STA);
        WiFi.disconnect();
        WiFi.macAddress(m_receiver_mac_address);
        WiFi.setTxPower(WIFI_POWER_19dBm);

        int result = esp_now_init();
        if (result != ESP_OK) {
            atmt::platform_printf("ESPNow Init Failed with Error");
            return;
        }
        esp_now_register_recv_cb(ESPNowReceiver::onReceivePacket);
        esp_now_peer_info_t broadcast_peer_info = {};
        memcpy(broadcast_peer_info.peer_addr, consts::ESPNow::k_ESPNowBroadcastAddress, 6);
        broadcast_peer_info.channel = 0;
        broadcast_peer_info.encrypt = false;
        result = esp_now_add_peer(&broadcast_peer_info);
        if (result != ESP_OK) {
            atmt::platform_printf("ESPNow Add Peer Failed with Error");
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

        if (m_pending_packet) {
            m_pending_packet = false;
            m_linked_joystick->updateState(mapPacketToJoystickState(&m_last_packet));
        }else if (!m_is_connected_to_controller || m_last_packet_timestamp.getTimeDifference(atmt::getSystemTime()) > consts::ESPNow::k_ESPNowDisableTimeoutSec) {
            m_is_connected_to_controller = false;
            m_linked_joystick->updateState(zeroJoystickInputs());
            broadcastLocateController();
        }
    };
    void ESPNowReceiver::disabledPeriodic() {};
    void ESPNowReceiver::autonomousPeriodic() {};
    void ESPNowReceiver::teleopPeriodic() {};

    void ESPNowReceiver::broadcastLocateController() {
        if (!m_initialized_without_error) return;

        if (m_broadcast_loop.checkTimeout()) { // Limits to once per k_BroadcastDelaySec (0.5 seconds)
            int result = esp_now_send(consts::ESPNow::k_ESPNowBroadcastAddress, reinterpret_cast<const uint8_t*>(m_receiver_name.data()), m_receiver_name.size());
            if (result == ESP_OK) {
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
    void ESPNowReceiver::instanceReceivePacket(const uint8_t* mac_info, const uint8_t* incoming_data, int data_length) {
        if (data_length != sizeof(ESPNowPacket)) {
            atmt::platform_printf("Unexpected Packet Length: %d\n", data_length);
            return;
        }
        atmt::platform_printf("ESPNow Packet Received\n");
        memcpy(&m_last_packet, incoming_data, sizeof(ESPNowPacket));
        if (m_led_2_receive_loop.checkTimeout()) {
            digitalWrite(consts::LED::k_LED2_Received, HIGH);
        }
        m_last_packet_timestamp = atmt::getSystemTime();
        m_is_connected_to_controller = true;

        // m_linked_joystick->updateState(mapPacketToJoystickState(m_last_packet));
        m_pending_packet = true;

        // if (!esp_now_is_peer_exist(m_last_packet->sender_mac_address)) {
        //     atmt::platform_printf("Adding Peer\n");
        //     // ESPNow.add_peer(m_last_packet->sender_mac_address);    
        //     esp_now_add_peer(m_last_packet->sender_mac_address);
        // }
        // if (memcmp(m_last_packet->sender_mac_address, m_receiver_mac_address, 6) == 0) {
        //     digitalWrite(consts::LED::k_LED3_Armed, HIGH);
        // }
    };
    void ESPNowReceiver::onReceivePacket(const uint8_t* mac_info, const uint8_t* incoming_data, int data_length) {
        ESPNowReceiver::Instance->instanceReceivePacket(mac_info, incoming_data, data_length);
    };
    atmt::JoystickState ESPNowReceiver::mapPacketToJoystickState(ESPNowPacket* packet) {
        atmt::JoystickState new_state{ };
        new_state.buttons[atmt::AButton] = packet->arm_weapon_motor;
        new_state.buttons[atmt::BButton] = packet->run_weapon_motor;

        switch (packet->joystick_direction[0]) {
            case JoystickDirection::Forward:
                new_state.axes[atmt::LXAxis] = 0;
                new_state.axes[atmt::LYAxis] = 100;
                break;
            case JoystickDirection::Backward:
                new_state.axes[atmt::LXAxis] = 0;
                new_state.axes[atmt::LYAxis] = -100;
                break;
            case JoystickDirection::Left:
                new_state.axes[atmt::LXAxis] = -100;
                new_state.axes[atmt::LYAxis] = 0;
                break;
            case JoystickDirection::Right:
                new_state.axes[atmt::LXAxis] = 100;
                new_state.axes[atmt::LYAxis] = 0;
                break;
            case JoystickDirection::Centered:
                new_state.axes[atmt::LXAxis] = 0;
                new_state.axes[atmt::LYAxis] = 0;
                break;
        }
        new_state.axis_range[0] = -100;
        new_state.axis_range[1] = 100;

        return new_state;
    };
    atmt::JoystickState ESPNowReceiver::zeroJoystickInputs() {
        atmt::JoystickState new_state{ };

        return new_state;
    };

};
