// Go until second intersection and align with it (axis over intersection), turn around, repeat

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <delay.h>
#include "robot_initialise.h"
#include "components.h"

int main () {
  cout << "Running the microswitch input test" << endl;
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

  stopwatch watch;
  watch.start();
  int state;
  bool reading;

  // int threshold = 100;

  // while (watch.read() < 3000) {
  //   state = components.beacon_reader.read_state();
  //   reading = (state >= threshold);
  //   cout << "Reading is: " << state << "   Classified as: " << reading << endl;
  //   delay(50);
  // }

  cout << "Continue with pulse count testing" << endl;

  int pulse_count = components.beacon_reader.get_beacon_code();
  cout << "Number of pulses: " << pulse_count << endl;

  cout << "Finished" << endl;

  return 1;
}
