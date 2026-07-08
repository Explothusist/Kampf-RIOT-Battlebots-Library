
#ifndef KAMPF_ESPNOW_TRANSMITTER_
#define KAMPF_ESPNOW_TRANSMITTER_

#include <string>
#include <vector>
#include "Automat/automat.h"
#include "kmpf_utils.h"

namespace kmpf {

    struct KnownReceiver {
        uint8_t mac_address[6];
        uint8_t receiver_address;
        std::string name;
    };

    class ESPNowTransmitter : public atmt::Subsystem {
        public:
            ESPNowTransmitter(atmt::ESPNowHandler* linked_handler, std::function<atmt::JoystickState()> state_getter);
            ~ESPNowTransmitter() override;

            void init() override;
            void systemPeriodic() override;
            void disabledPeriodic() override;
            void autonomousPeriodic() override;
            void teleopPeriodic() override;

            const std::vector<KnownReceiver>& getKnownReceivers();
            // void pairWithReceiver(const uint8_t mac_address[]);
            void pairWithReceiver(KnownReceiver receiver);
            void acknowledgePair();
            void sendState();

        private:
            atmt::TimeoutManager m_send_state_loop;
            atmt::Timestamp m_last_packet_timestamp;
            bool m_attempting_connect_to_receiver;
            bool m_is_connected_to_receiver;
            uint8_t m_connected_receiver_address;

            std::vector<KnownReceiver> m_known_receivers;

            atmt::ESPNowHandler* m_linked_espnow_handler;
            std::function<atmt::JoystickState()> m_state_getter;

            bool m_initialized_without_error;
    };

};

#endif