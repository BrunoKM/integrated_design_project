// Test which motor is connected to which wire (helps label the wires)

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <bitset>
#include <string>
#include "robot_initialise.h"

int main () {
  initialise_robot();

  const float test_time = 2000; // Duration in seconds of each test
  const int cmd_speed = 127;

  for (int i = 0; i < 4; i++) {
    cout << "Currently testing motor: " << i;
    switch(i) {
      case 0:
        rlink.command(MOTOR_0_GO, cmd_speed);
        delay(test_time);
        rlink.command(MOTOR_0_GO, 0);
        break;
      case 1:
        rlink.command(MOTOR_1_GO, cmd_speed);
        delay(test_time);
        rlink.command(MOTOR_1_GO, 0);
        break;
      case 2:
        rlink.command(MOTOR_2_GO, cmd_speed);
        delay(test_time);
        rlink.command(MOTOR_2_GO, 0);
        break;
      case 3:
        rlink.command(MOTOR_3_GO, cmd_speed);
        delay(test_time);
        rlink.command(MOTOR_3_GO, 0);
        break;
  }
