#ifndef MOTOR_H
#define MOTOR_H

#include "robot_initialise.h"

class Motor {
private:
  const int motor_number;
  const float speed_calibration_factor;
  int orientation;
  float requested_speed;

  void go_command(int speed_cmd);
  int request_speed_command();
public:
  Motor(int motor_number, float speed_cal_factor, int orientation);
  void drive(float speed);
  float get_current_speed();
  float get_requested_speed();
};

#endif
