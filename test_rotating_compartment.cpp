// Go until first intersection and align with it (axis over intersection)

#include <iostream>
#include <string>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <delay.h>
#include "Robot.h"
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"

int main () {
  initialise_robot();
  cout << "Testing movement of the compartment." << endl;

  // Initialise an instance of objet to set the motor ramps
  Robot robot;

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

  int position;
  while (true) {
    cout << "Which position do you want to move the compartment to?" << endl;
    cin >> position;
    cout << "Turning to position: " << position << endl;
    components.compartment.turn_to_position(position);
    cout << "Compartment at position " << position << endl;
    delay(1000);
    cout << "Turning to the default position" << endl;
    components.return_to_default();
  }

  cout << "Finished" << endl;

  return 1;
}
