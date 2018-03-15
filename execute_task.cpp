// Go until second intersection and align with it (axis over intersection), turn around, repeat

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
  cout << "--- Execution ---" << endl;
  robot.read_beacon();
  robot.move('j');

  for (int i = 0; i <= 4; ++i) {
    robot.align_for_pickup();
    robot.pick_up_all_eggs();
    robot.move(robot.delivery_zone);
    robot.deliver_basket();

    robot.move('f');
    robot.recycle_eggs();

    robot.move('j');
  }
}
