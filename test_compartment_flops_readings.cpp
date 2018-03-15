// Go until first intersection and align with it (axis over intersection)

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"

int main () {
  initialise_robot();
  cout << "Running test for turn exactly" << endl;

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

  components.compartment.reset_flops();

  stopwatch watch;
  watch.start();

  while (watch.read() < 10000) {
    bool left_reading = components.compartment.read_left_flop();
    bool right_reading = components.compartment.read_right_flop();
    cout << "The readings are: Left: " << left_reading << "Right: " << right_reading << endl;
  }

  cout << "Finished" << endl;

  return 1;
}
