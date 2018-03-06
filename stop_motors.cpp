
#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"

int main () {
  initialise_robot();
  cout << "Stopping both motors" << endl;

  int pcb1_port = 0; // Port number for PCB 1

  Components components(pcb1_port);
  Line_Following lf(components.line_sensors);
  
  lf.stop_motors();
  return 1;
 }
