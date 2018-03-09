// Go until first intersection and align with it (axis over intersection)

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"

int main () {
  initialise_robot();
  cout << "Running test for turn exactly" << endl;

  int pcb1_port = 0; // Port number for PCB 1
  int pcb2_port = 1; // Port number for PCB 2

  Components components(pcb1_port, pcb2_port);
  Line_Following lf(components);

  float speed = 1.0;
  // float speed_delta = 0.5;

  int turn_by = -180;
  bool stop_after = true;

  lf.turn_exactly(turn_by, speed, stop_after);

  cout << "Finished" << endl;

  return 1;
}
