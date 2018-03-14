// Very simple test for the scoop

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"

int main () {
  cout << "Running the actuator test." << endl;
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

  int delay_time = 1000;
  stopwatch watch;
  watch.start();
  while (watch.read() < 15000) {
    components.scoop.contract();
    delay(delay_time);
    components.scoop.release();
    delay(delay_time);
  }

  cout << "Finished" << endl;

  return 1;
}
