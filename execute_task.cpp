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

  robot.move_s_to_j();

  for (int i = 0; i <= 4; ++i) {
    robot.align_for_pickup();
    robot.pick_up_all_eggs();

    robot.move_c_to_j();
    robot.move_j_to_l();

    robot.move_l_to_delivery();
    robot.deliver_basket();

    robot.move_delivery_to_l();
    robot.move_l_to_f();

    robot.recycle_eggs();
    robot.move_f_to_j();
  }
}
