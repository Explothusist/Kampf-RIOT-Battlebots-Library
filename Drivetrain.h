
#ifndef KAMPF_DRIVETRAIN_
#define KAMPF_DRIVETRAIN_

#include "Automat/automat.h"

namespace kmpf {

    class Drivetrain : public atmt::Subsystem {
        public:
            Drivetrain();
            Drivetrain(bool invert_left, bool invert_right);
            Drivetrain(bool invert_left, bool invert_right, int left_pin1_forward, int left_pin2_backward, int right_pin1_forward, int right_pin2_backward);
            Drivetrain(bool invert_left, bool invert_right, int left_pin1_forward, int left_pin2_backward, int right_pin1_forward, int right_pin2_backward, double max_power);
            ~Drivetrain() override;

            void init() override;
            void systemPeriodic() override;
            void disabledPeriodic() override;
            void autonomousPeriodic() override;
            void teleopPeriodic() override;

            void driveTank(double left_percent, double right_percent);
            void driveTank(double left_percent, double right_percent, double deadband);
            void driveArcade(double forward_percent, double turn_percent);
            void driveArcade(double forward_percent, double turn_percent, double deadband);
            void stopDrive();

            void setMaxPower(double max_power);
        private:
            bool m_invert_left;
            bool m_invert_right;
            int m_left_pin1_forward;
            int m_left_pin2_backward;
            int m_right_pin1_forward;
            int m_right_pin2_backward;
            double m_max_power;

            atmt::SlewRateLimiter m_left_motor_current_power;
            double m_left_motor_target_power;
            atmt::SlewRateLimiter m_right_motor_current_power;
            double m_right_motor_target_power;
    };

};

#endif