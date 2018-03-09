#include <iostream>
#include <robot_link.h>
#include "Driving_Motor.h"

#define DEBUG4

Driving_Motor::Driving_Motor (int motor_number, float speed_cal_factor, int orientation):
motor_number(motor_number), speed_calibration_factor(speed_cal_factor), orientation(orientation)
{

}


void Driving_Motor::drive(float speed) {
  // Only send command if the speed is different from requested speed (saves time)
  if (speed != requested_speed) {
    int speed_cmd = int((speed * speed_calibration_factor) * 127.0);
    
    #ifdef DEBUG4
      std::cout << "Cmd before conversion sent to motor: " << speed_cmd << std::endl;
     #endif
     
    if (speed > 0) {
      // If direction is forward:
      if (orientation == -1) speed_cmd += 128;
      go_command(speed_cmd);
    } else if (speed < 0) {
      // If direction is backwards:
      speed_cmd *= (-1);
      #ifdef DEBUG4
      std::cout << "Cmd after time -1 : " << speed_cmd << std::endl;
     #endif
      if (orientation == 1) {speed_cmd += 128; std::cout << "speed_cmd increased" << std::endl;}
      go_command(speed_cmd);
    } else {
	  go_command(0);
	}
	#ifdef DEBUG4
      std::cout << "Cmd sent to motor: " << speed_cmd << std::endl;
     #endif
    requested_speed = speed;
  }

}

float Driving_Motor::get_current_speed() {
  int speed_127scale = request_speed_command();
  if (speed_127scale >= 128) {
    speed_127scale = (speed_127scale - 127) * (-1);
  }
  speed_127scale *= orientation;

  float current_speed = float(speed_127scale) / (speed_calibration_factor * 127);
  return current_speed;
}

float Driving_Motor::get_requested_speed() {
	return requested_speed;
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

int Driving_Motor::request_speed_command() {
  int measured_speed;
  switch(motor_number) {
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
  return measured_speed;
}
