
#ifndef KAMPF_WEAPON_MOTOR_
#define KAMPF_WEAPON_MOTOR_

#include "Automat/automat.h"
#include "ESP32Servo/src/ESP32Servo.h"

namespace kmpf {

    class WeaponMotor : public atmt::Subsystem {
        public:
            WeaponMotor();
            WeaponMotor(int pin);
            WeaponMotor(int pin, double pwm_min_us, double pwm_max_us, double pwm_slew_rate_us);
            ~WeaponMotor() override;

            void init() override;
            void systemPeriodic() override;
            void disabledPeriodic() override;
            void autonomousPeriodic() override;
            void teleopPeriodic() override;

            void armMotor();
        private:
            Servo m_motor;
            int m_pin;
            // int m_pin1_forward;
            // int m_pin2_backward;
            // int m_pwm_min_us; // Incorporate these three into SlewRateLimiter
            // int m_pwm_max_us;
            // int m_pwm_slew_rate_us;
            atmt::SlewRateLimiter m_current_pwm_width;

            bool m_is_armed;
    };

};

#endif