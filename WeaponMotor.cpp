
#include "WeaponMotor.h"
#include "kmpf_constants.h"
#include <Arduino.h>

namespace kmpf {

    WeaponMotor::WeaponMotor():
        WeaponMotor(
            consts::WeaponMotor::k_Pin
        )
    {
        
    }
    WeaponMotor::WeaponMotor(int pin):
        WeaponMotor(
            pin,
            consts::WeaponMotor::k_MinPWMPulseWidthUs,
            consts::WeaponMotor::k_MaxPWMPulseWidthUs,
            consts::WeaponMotor::k_MaxPWMPulseSlewRate
        )
    {
        
    }
    WeaponMotor::WeaponMotor(int pin, double pwm_min_us, double pwm_max_us, double pwm_slew_rate_us):
        m_motor{ },
        m_pin{ pin },
        m_current_pwm_width( pwm_slew_rate_us, -pwm_slew_rate_us, pwm_min_us, pwm_max_us, pwm_min_us ),
        m_is_armed{ false }
    {
        
    }
    WeaponMotor::~WeaponMotor() {

    };

    void WeaponMotor::init() {
        pinMode(consts::LED::k_LED3_Armed, OUTPUT);

        digitalWrite(consts::LED::k_LED3_Armed, LOW);
    };
    void WeaponMotor::systemPeriodic() {};
    void WeaponMotor::disabledPeriodic() {};
    void WeaponMotor::autonomousPeriodic() {};
    void WeaponMotor::teleopPeriodic() {};

};
