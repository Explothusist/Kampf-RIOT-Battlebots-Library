
#ifndef KAMPF_ESPNOW_RECEIVER_
#define KAMPF_ESPNOW_RECEIVER_

#include <string>
#include "Automat/automat.h"
#include "kmpf_utils.h"

namespace kmpf {

    class ESPNowReceiver : public atmt::Subsystem {
        public:
            ESPNowReceiver(std::string receiver_name, atmt::Joystick* linked_joystick, atmt::ESPNowHandler* linked_handler);
            ~ESPNowReceiver() override;

            void init() override;
            void systemPeriodic() override;
            void disabledPeriodic() override;
            void autonomousPeriodic() override;
            void teleopPeriodic() override;

            void broadcastLocateController();
            void acknowledgePair();
            void sendHeartbeat();
            static atmt::JoystickState zeroJoystickInputs();

        private:
            atmt::TimeoutManager m_broadcast_loop;
            atmt::TimeoutManager m_heartbeat_loop;
            atmt::TimeoutManager m_led_1_transmit_loop;
            atmt::TimeoutManager m_led_2_receive_loop;
            atmt::Timestamp m_last_packet_timestamp;
            bool m_is_connected_to_controller;

            std::string m_receiver_name;

            atmt::Joystick* m_linked_joystick;
            atmt::ESPNowHandler* m_linked_espnow_handler;

            bool m_initialized_without_error;
    };

};

#endif