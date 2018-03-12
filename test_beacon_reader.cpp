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

  int pcb1_port = 0; // Port number for PCB 1
  int pcb2_port = 1; // Port number for PCB 1
  int beacon_reader_port = 0;

  Components components(pcb1_port, pcb2_port, beacon_reader_port);

  stopwatch watch;
  watch.start();
  int state;
  bool reading;

  int threshold = 100;

  while (watch.read() < 3000) {
    state = components.beacon_reader.read_state();
    reading = (state >= threshold);
    cout << "Reading is: " << state << "   Classified as: " << reading << endl;
    delay(50);
  }

  cout << "Continue with pulse count testing" << endl;

  int pulse_count = components.beacon_reader.get_beacon_code();
  cout << "Number of pulses: " << pulse_count << endl;

  cout << "Finished" << endl;

  return 1;
}
