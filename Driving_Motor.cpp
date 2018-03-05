#include <robot_link>
#include "Driving_Motor.h"

Driving_Motor::Driving_Motor (int motor_number, float speed_cal_factor, int orientation):
motor_number(motor_num), speed_calibration_factor(speed_cal_factor), orientation(orient)
{

}


void Driving_Motor::drive_forward(float speed) {
  int speed_cmd = int((speed * speed_calibration_factor) * 127.0);
  if (orientation == -1) speed_cmd += 128;
  go_command(speed_cmd);
}
void Driving_Motor::drive_backward(float speed) {
  int speed_cmd = int((speed * speed_calibration_factor) * 127.0);
  if (orientation == 1) speed_cmd += 128;
  go_command(speed_cmd);
}

float get_current_speed() {
  int speed_127scale = request_speed_command();
  if (speed_127scale >= 128) {
    speed_127scale = (speed_127scale - 127) * (-1);
  }
  speed_127scale *= orientation;

  float current_speed = float(speed_127scale) / (speed_calibration_factor * 127);
  return current_speed
}

void Driving_Motor::go_command(int speed_cmd) {
  switch(motor_number) {
    case 1:
      rlink.command(MOTOR_1_GO, speed_cmd);
      break;
    case 2:
      rlink.command(MOTOR_2_GO, speed_cmd);
      break;
    case 3:
      rlink.command(MOTOR_3_GO, speed_cmd);
      break;
    case 4:
      rlink.command(MOTOR_4_GO, speed_cmd);
      break;
  }
}

int request_speed_command() {
  switch(motor_number) {
    int measured_speed;
    case 1:
      measured_speed = rlink.request(MOTOR_1);
      break;
    case 2:
      measured_speed = rlink.request(MOTOR_2);
      break;
    case 3:
      measured_speed = rlink.request(MOTOR_3);
      break;
    case 4:
      measured_speed = rlink.request(MOTOR_4);
      break;
  }
  return measured_speed
}
