// Go until second intersection and align with it (axis over intersection), turn around, repeat

#include <iostream>
#include <string>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"
#include "Robot.h"

int main () {

  Robot robot;
  int num_repeat = 20;
  cout << "Running Alignment for pickup test" << endl;

  for (int i = 0; i < num_repeat; i++) {
    robot.align_for_pickup();
    robot.move("j")
  }
