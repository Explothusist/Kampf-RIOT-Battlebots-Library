
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

            bool broadcastLocateController();
            void onReceivePacket();
            atmt::JoystickState* mapPacketToJoystickState(ESPNowPacket* packet);

        private:
            atmt::Timestamp m_last_packet_received;
            bool m_is_connected_to_controller;

            std::string m_receiver_name;
            uint8_t m_receiver_mac_address[6];

            atmt::Joystick* m_linked_joystick;
    };

};