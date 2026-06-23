
#ifndef KAMPF_ESPNOW_RECEIVER_
#define KAMPF_ESPNOW_RECEIVER_

#include <string>
#include "Automat/automat.h"
#include "kmpf_utils.h"

namespace kmpf {

    class ESPNowReceiver : public atmt::Subsystem {
        public:
            ESPNowReceiver(std::string receiver_name, atmt::Joystick* linked_joystick);
            ~ESPNowReceiver() override;

            void init() override;
            void systemPeriodic() override;
            void disabledPeriodic() override;
            void autonomousPeriodic() override;
            void teleopPeriodic() override;

            void broadcastLocateController();
            void instanceReceivePacket(const uint8_t* mac_info, const uint8_t* incoming_data, int data_length);
            static void onReceivePacket(const uint8_t* mac_info, const uint8_t* incoming_data, int data_length);
            static atmt::JoystickState mapPacketToJoystickState(ESPNowPacket* packet);
            static atmt::JoystickState zeroJoystickInputs();

            static ESPNowReceiver* Instance;
        private:
            atmt::TimeoutManager m_broadcast_loop;
            atmt::TimeoutManager m_led_1_transmit_loop;
            atmt::TimeoutManager m_led_2_receive_loop;
            ESPNowPacket m_last_packet;
            atmt::Timestamp m_last_packet_timestamp;
            bool m_is_connected_to_controller;
            bool m_pending_packet;

            std::string m_receiver_name;
            uint8_t m_receiver_mac_address[6];

            atmt::Joystick* m_linked_joystick;

            bool m_initialized_without_error;
    };

};

#endif