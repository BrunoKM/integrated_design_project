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

  Components components(pcb1_port, pcb2_port);

  stopwatch watch;
  watch.start();
  int rear_state_reading;
  while (watch.read() < 10000) {
    components.microswitches.update_state();
    rear_state_reading = components.microswitches.rear_state;
    cout << "Reading is: "
    << rear_state_reading << endl;
    delay(100);
  }

  cout << "Finished" << endl;

  return 1;
}
