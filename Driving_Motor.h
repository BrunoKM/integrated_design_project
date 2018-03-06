#ifndef DRIVING_MOTOR_H
#define DRIVING_MOTOR_H

#include "robot_initialise.h"

class Driving_Motor {
private:
  const int motor_number;
  const float speed_calibration_factor;
  float orientation;
  float requested_speed;

  void go_command(int speed_cmd);
  int request_speed_command();
public:
  Driving_Motor(int motor_number, float speed_cal_factor, int orientation);
  void drive(float speed);
  float get_current_speed();
};

#endif
