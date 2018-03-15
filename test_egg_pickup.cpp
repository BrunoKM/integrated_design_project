
#include <iostream>
#include <string>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <delay.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"
#include "Robot.h"

int main () {

  Robot robot;
  cout << "--- Subsystems Demo ---" << endl;
  cout << "Running Task 1:" << endl;
  robot.pick_up_all_eggs();


}
