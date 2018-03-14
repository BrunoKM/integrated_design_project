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
  int rear_state_reading;
  while (watch.read() < 10000) {
    components.microswitches.update_state();
    rear_state_reading = components.microswitches.rear_state;
    front_state_reading = components.microswitches.front_state;
    cout << "Reading is:\n" << "Rear: "
    << rear_state_reading << "  ; Front: " << front_state_reading << endl;
    delay(100);
  }

  cout << "Finished" << endl;

  return 1;
}
