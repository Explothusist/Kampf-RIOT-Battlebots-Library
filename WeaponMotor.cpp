
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
        m_pwm_width_minimum_us{ pwm_min_us },
        m_pwm_width_range_us{ pwm_max_us - pwm_min_us },
        m_pwm_target_width_us{ pwm_min_us },
        m_is_armed{ false },
        m_last_command{ atmt::Timestamp{0} }
    {
        
    }
    WeaponMotor::~WeaponMotor() {

    };

    void WeaponMotor::init() {
        pinMode(consts::LED::k_LED3_Armed, OUTPUT);

        digitalWrite(consts::LED::k_LED3_Armed, LOW);
    };
    void WeaponMotor::systemPeriodic() {
        if (!m_is_armed) return;

        if (m_last_command.getTimeDifference(atmt::getSystemTime()) > consts::WeaponMotor::k_SpindownTimeoutSec) {
            m_pwm_target_width_us = m_pwm_width_minimum_us;
        }
        m_motor.writeMicroseconds(m_current_pwm_width.calculateFiltered(m_pwm_target_width_us));
    };
    void WeaponMotor::disabledPeriodic() {
        disarmMotor();
    };
    void WeaponMotor::autonomousPeriodic() {};
    void WeaponMotor::teleopPeriodic() {};

    void WeaponMotor::armMotor() { // May need a delay in the arming sequence, depending on hardware?
        if (m_is_armed) return;

        digitalWrite(consts::LED::k_LED3_Armed, HIGH);
        atmt::platform_printf("Arming Weapon Motor\n");
        // m_motor.detach();
        m_motor.attach(m_pin);
        m_pwm_target_width_us = m_pwm_width_minimum_us;
        m_current_pwm_width.resetValue(m_pwm_width_minimum_us);
        m_motor.writeMicroseconds(m_pwm_width_minimum_us);
        m_is_armed = true;
        m_last_command = atmt::getSystemTime();
    };
    void WeaponMotor::disarmMotor() {
        if (!m_is_armed) return;

        digitalWrite(consts::LED::k_LED3_Armed, LOW);
        atmt::platform_printf("Disarming Weapon Motor\n");
        m_pwm_target_width_us = m_pwm_width_minimum_us;
        m_current_pwm_width.resetValue(m_pwm_width_minimum_us);
        m_motor.writeMicroseconds(m_pwm_width_minimum_us);
        m_motor.detach();
        m_is_armed = false;
        m_last_command = atmt::getSystemTime();
    };
    void WeaponMotor::spinMotor(double percent_power) {
        if (!m_is_armed) return;

        percent_power = std::max(0.0, std::min(percent_power, 1.0));
        m_pwm_target_width_us = m_pwm_width_minimum_us + m_pwm_width_range_us * percent_power;
        m_last_command = atmt::getSystemTime();
    };
    void WeaponMotor::spinDown() {
        if (!m_is_armed) return;

        spinMotor(0.0);
    };
    void WeaponMotor::cutoff() {
        if (!m_is_armed) return;

        spinMotor(0.0);
        m_current_pwm_width.resetValue(m_pwm_width_minimum_us);
    };

};
