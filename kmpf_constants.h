
#ifndef KAMPF_CONSTANTS_
#define KAMPF_CONSTANTS_

#include <stdint.h>

namespace kmpf {
    namespace consts {

        namespace ESPNow {
            constexpr uint8_t k_ESPNowBroadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
            constexpr double k_BroadcastDelaySec = 0.5; // Seconds 
            constexpr double k_HeartbeatDelaySec = 0.2; // Seconds 
            // constexpr const char* k_ESPNowId = "Spenc";
            constexpr double k_ESPNowDisableTimeoutSec = 1.0; // Seconds
        }
        
        namespace Drivetrain {
            constexpr int k_Left_Pin1_Forward = 2;
            constexpr int k_Left_Pin2_Backward = 3;
            constexpr int k_Right_Pin1_Forward = 8;
            constexpr int k_Right_Pin2_Backward = 9;

            constexpr double k_NoPWMPower = 0.0;
            constexpr double k_MaxPWMPower = 125.0;
            constexpr double k_PWMPowerSecondsToMax = 1.0; // Seconds
            constexpr double k_PWNPowerSlewRate = (k_MaxPWMPower - k_NoPWMPower) / k_PWMPowerSecondsToMax;
        }
        namespace WeaponMotor {
            // constexpr int k_Pin1_Forward = 0;
            // constexpr int k_Pin2_Backward = 1;
            constexpr int k_Pin = 1;

            constexpr double k_MinPWMPulseWidthUs = 1000.0; // Microseconds
            constexpr double k_MaxPWMPulseWidthUs = 1800.0;
            constexpr double k_PWMPulseWidthSecondsToMax = 3.0; // Seconds
            constexpr double k_MaxPWMPulseSlewRate = (k_MaxPWMPulseWidthUs - k_MinPWMPulseWidthUs) / k_PWMPulseWidthSecondsToMax;
            
            constexpr double k_SpindownTimeoutSec = 0.5; // Seconds
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

            constexpr double k_BlinkRateSec = 0.25;
        }

    };
};

#endif