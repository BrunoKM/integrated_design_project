#ifndef DRIVING_MOTOR_H
#define DRIVING_MOTOR_H

#include "robot_initialise.h"

class Driving_Motor {
private:
  const int motor_number;
  const float speed_calibration_factor;
  float orientation;

  void go_command(int speed_cmd);
  int request_speed_command();
public:
  Driving_Motor(int motor_num, float speed_cal_factor, int orient);
  void drive_forward(float speed);
  void drive_backward(float speed);
  float get_current_speed();
}

#endif
