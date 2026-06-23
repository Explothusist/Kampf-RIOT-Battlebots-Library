
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
        m_max_power{ max_power }
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
    void Drivetrain::systemPeriodic() {};
    void Drivetrain::disabledPeriodic() {};
    void Drivetrain::autonomousPeriodic() {};
    void Drivetrain::teleopPeriodic() {};

    void Drivetrain::driveTank(double left_percent, double right_percent) {

    };
    void Drivetrain::stopDrive() {

    };

    void Drivetrain::setMaxPower(double max_power) {
        m_max_power = max_power;
    };

};
