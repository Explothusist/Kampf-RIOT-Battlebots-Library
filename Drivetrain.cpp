
#include "Drivetrain.h"
#include "kmpf_constants.h"
#include <Arduino.h>

namespace kmpf {

    Drivetrain::Drivetrain():
        Drivetrain(
            consts::Drivetrain::k_Left_Pin1_Forward,
            consts::Drivetrain::k_Left_Pin2_Backward,
            consts::Drivetrain::k_Right_Pin1_Forward,
            consts::Drivetrain::k_Right_Pin2_Backward
        )
    {
        
    }
    Drivetrain::Drivetrain(int left_pin1_forward, int left_pin2_backward, int right_pin1_forward, int right_pin2_backward):
        Drivetrain(
            left_pin1_forward,
            left_pin2_backward,
            right_pin1_forward,
            right_pin2_backward,
            consts::Drivetrain::k_MaxPWMPower
        )
    {
        
    }
    Drivetrain::Drivetrain(int left_pin1_forward, int left_pin2_backward, int right_pin1_forward, int right_pin2_backward, double max_power):
        m_left_pin1_forward{ left_pin1_forward },
        m_left_pin2_backward{ left_pin2_backward },
        m_right_pin1_forward{ right_pin1_forward },
        m_right_pin2_backward{ right_pin2_backward },
        m_max_power{ max_power },
        m_left_motor_current_power{ consts::Drivetrain::k_PWNPowerSlewRate, -consts::Drivetrain::k_PWNPowerSlewRate, 0.0, max_power, -max_power },
        m_left_motor_target_power{ 0.0 },
        m_right_motor_current_power{ consts::Drivetrain::k_PWNPowerSlewRate, -consts::Drivetrain::k_PWNPowerSlewRate, 0.0, max_power, -max_power },
        m_right_motor_target_power{ 0.0 }
    {
        
    }
    Drivetrain::~Drivetrain() {

    };

    void Drivetrain::init() {
        pinMode(m_left_pin1_forward, OUTPUT);
        pinMode(m_left_pin2_backward, OUTPUT);
        pinMode(m_right_pin1_forward, OUTPUT);
        pinMode(m_right_pin2_backward, OUTPUT);

        digitalWrite(m_left_pin1_forward, LOW);
        digitalWrite(m_left_pin2_backward, LOW);
        digitalWrite(m_right_pin1_forward, LOW);
        digitalWrite(m_right_pin2_backward, LOW);
    };
    void Drivetrain::systemPeriodic() {
        double left_power = m_left_motor_current_power.calculateFiltered(m_left_motor_target_power);
        analogWrite(m_left_pin1_forward, static_cast<int>(std::max(0.0, left_power)));
        analogWrite(m_left_pin2_backward, static_cast<int>(std::max(0.0, -left_power)));
        double right_power = m_right_motor_current_power.calculateFiltered(m_right_motor_target_power);
        analogWrite(m_right_pin1_forward, static_cast<int>(std::max(0.0, right_power)));
        analogWrite(m_right_pin2_backward, static_cast<int>(std::max(0.0, -right_power)));
    };
    void Drivetrain::disabledPeriodic() {
        stopDrive();
    };
    void Drivetrain::autonomousPeriodic() {};
    void Drivetrain::teleopPeriodic() {};

    void Drivetrain::driveTank(double left_percent, double right_percent) {
        left_percent = std::max(-1.0, std::min(left_percent, 1.0));
        m_left_motor_target_power = m_max_power * left_percent;
        right_percent = std::max(-1.0, std::min(right_percent, 1.0));
        m_right_motor_target_power = m_max_power * right_percent;
    };
    void Drivetrain::driveArcade(double forward_percent, double turn_percent) {
        forward_percent = std::max(-1.0, std::min(forward_percent, 1.0));
        turn_percent = std::max(-1.0, std::min(turn_percent, 1.0));
        double left_percent = std::max(-1.0, std::min(forward_percent + turn_percent, 1.0));
        double right_percent = std::max(-1.0, std::min(forward_percent - turn_percent, 1.0));
        driveTank(left_percent, right_percent);
    };
    void Drivetrain::stopDrive() {
        m_left_motor_target_power = 0.0;
        m_left_motor_current_power.resetValue(0.0);
        m_right_motor_target_power = 0.0;
        m_right_motor_current_power.resetValue(0.0);
    };

    void Drivetrain::setMaxPower(double max_power) {
        m_max_power = std::max(max_power, 0.0);
        m_left_motor_current_power.setBounds(m_max_power, -m_max_power);
        m_right_motor_current_power.setBounds(m_max_power, -m_max_power);
    };

};
