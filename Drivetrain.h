
#ifndef KAMPF_DRIVETRAIN_
#define KAMPF_DRIVETRAIN_

#include "Automat/automat.h"

namespace kmpf {

    class Drivetrain : public atmt::Subsystem {
        public:
            Drivetrain();
            Drivetrain(int left_pin1_forward, int left_pin2_backward, int right_pin1_forward, int right_pin2_backward);
            Drivetrain(int left_pin1_forward, int left_pin2_backward, int right_pin1_forward, int right_pin2_backward, double max_power);
            ~Drivetrain() override;

            void init() override;
            void systemPeriodic() override;
            void disabledPeriodic() override;
            void autonomousPeriodic() override;
            void teleopPeriodic() override;

            void driveTank(double left_percent, double right_percent);
            void stopDrive();

            void setMaxPower(double max_power);
        private:
            int m_left_pin1_forward;
            int m_left_pin2_backward;
            int m_right_pin1_forward;
            int m_right_pin2_backward;
            double m_max_power;
    };

};

#endif