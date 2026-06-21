
#include "ESPNowReceiver.h"
#include "kmpf_constants.h"

#include <esp_now.h>

namespace kmpf {

    ESPNowReceiver::ESPNowReceiver(std::string receiver_name, atmt::Joystick* linked_joystick):
        m_last_packet_received{ atmt::Timestamp(0) },
        m_is_connected_to_controller{ false },
        m_receiver_name{ receiver_name },
        m_receiver_mac_address{ },
        m_linked_joystick{ linked_joystick }
    {

    }

    void ESPNowReceiver::init() {
        
    };
    void ESPNowReceiver::systemPeriodic() {

    };
    void ESPNowReceiver::disabledPeriodic() {};
    void ESPNowReceiver::autonomousPeriodic() {};
    void ESPNowReceiver::teleopPeriodic() {};

    bool ESPNowReceiver::broadcastLocateController() {
        int result = esp_now_send(consts::ESPNow::k_ESPNowBroadcastAddress, reinterpret_cast<uint8_t>(m_receiver_name.c_str()), sizeof(m_receiver_name.c_str()));
    };
    void ESPNowReceiver::onReceivePacket() {

    };
    atmt::JoystickState* ESPNowReceiver::mapPacketToJoystickState(ESPNowPacket* packet) {

    };

};
