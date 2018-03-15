// Very simple test for the scoop

#include <iostream>
#include <string>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <delay.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"

int main () {
  cout << "Testing the violent shock to shake the eggs down the ramp. (thanks mechanical)" << endl;
  initialise_robot();

  // I2C addresses:
  int pcb1_port = 0; // Port number for PCB 1
  int pcb2_port = 1; // Port number for PCB 1
  int turntable_comms_port = 2; // Port number
  // ADC addresses:
  int beacon_reader_port = 0; // Port number for the ADC beacon reader input
  int colour_detection_1_port = 1; // Port number for first colour detection ADC
  int colour_detection_2_port = 2; // Port number for second colour detection ADC

  Components components(pcb1_port, pcb2_port, turntable_comms_port,
    beacon_reader_port, colour_detection_1_port, colour_detection_2_port);

  int usr_input;

  while (true) {
    cout << "Type anything when ready to VIOLENTLY SHAKE THE ACTUATOR." << endl;
    cin >> usr_input;

    components.scoop.violent_shock();
    cout << "Shocked violently." << endl;
  }

  cout << "Finished" << endl;

  return 1;
}
