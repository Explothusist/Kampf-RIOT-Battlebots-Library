
#ifndef KAMPF_CONSTANTS_
#define KAMPF_CONSTANTS_

#include <stdint.h>

namespace kmpf {
    namespace consts {

        namespace ESPNow {
            constexpr uint8_t k_ESPNowBroadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
            constexpr double k_BroadcastDelaySec = 0.5; // Seconds 
            constexpr double k_HeartbeatDelaySec = 0.2; // Seconds 
            constexpr double k_SendStateDelaySec = 0.05; // Seconds 
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

        namespace Controller {
            namespace GPIO {
                constexpr int k_LeftTrigger = 7;
                constexpr int k_RightTrigger = 21;
                constexpr int k_BottomButton = 20;
                constexpr int k_TopButton = 10;

                constexpr int k_AxisLX = 1;
                constexpr int k_AxisLY = 2;
                constexpr int k_AxisRX = 3;
                constexpr int k_AxisRY = 4;

                constexpr int k_AnalogBatteryPower = 5;
            }

            namespace U8g2 {
                constexpr int k_DirectionUp = 1;
                constexpr int k_DirectionDown = 2;
                constexpr int k_DirectionLeft = 3;
                constexpr int k_DirectionRight = 4;
                constexpr int k_DirectionCenter = 0;
                
                constexpr int k_GlyphUp = 1;
                constexpr int k_GlyphDown = 2;
                constexpr int k_GlyphLeft = 3;
                constexpr int k_GlyphRight = 4;
                constexpr int k_NoGlyph = 0;
                
                constexpr int k_Battary_0_25 = 0;
                constexpr int k_Battary_25_50 = 1;
                constexpr int k_Battary_50_75 = 2;
                constexpr int k_Battary_75_100 = 3;
            }

            constexpr double k_DisplayLogoTimeSec = 2.0; // Seconds
            constexpr double k_DrawScreenDelaySec = 0.05; // Seconds
            constexpr int k_MACsDrawnPerScreen = 2;

            // constexpr uint8_t k_SDAPin = 8;
            // constexpr uint8_t k_SCLPin = 9;
        }

    };
};

#endif