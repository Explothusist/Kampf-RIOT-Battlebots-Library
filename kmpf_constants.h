
#ifndef KAMPF_CONSTANTS_
#define KAMPF_CONSTANTS_

#include <stdint.h>

namespace kmpf {
    namespace consts {

        namespace ESPNow {
            constexpr uint8_t k_ESPNowBroadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
            // constexpr const char* k_ESPNowId = "Spenc";
            constexpr double k_ESPNowDisableTimeout = 10.0; // Seconds
        }
        
        namespace LeftDriveMotor {
            constexpr int k_Pin1_Forward = 2;
            constexpr int k_Pin2_Backward = 3;
        }
        namespace RightDriveMotor {
            constexpr int k_Pin1_Forward = 8;
            constexpr int k_Pin2_Backward = 9;
        }
        namespace WeaponMotor {
            // constexpr int k_Pin1_Forward = 0;
            constexpr int k_Pin2_Backward = 1;

            constexpr int k_MinPWMPulseWidthUs = 1000; // Microseconds
            constexpr int k_MaxPWMPulseWidthUs = 1800;
        }
        // namespace Servo {
        //     constexpr int k_Pin1_Forward = 10;
        //     constexpr int k_Pin2_Backward = 7;

        //     constexpr int k_IdleAngle = 0;
        //     constexpr int k_ArmAngle = 90;
        //     constexpr int k_AttackAngle = 180;
        // }
        // namespace PWM {
        //     constexpr int k_Pin1_Forward = 20;
        //     constexpr int k_Pin2_Backward = 21;
        // }
        namespace LED {
            constexpr int k_LED1_Transmit = 4;
            constexpr int k_LED2_Received = 5;
            constexpr int k_LED3_Armed = 6;
        }

    };
};

#endif