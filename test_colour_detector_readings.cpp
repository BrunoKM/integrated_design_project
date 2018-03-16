// Go until second intersection and align with it (axis over intersection), turn around, repeat

#include <iostream>
#include <string>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <delay.h>
#include "robot_initialise.h"
#include "components.h"
#include "Robot.h"

int main () {
  cout << "Running the turntable angle control test." << endl;
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

  int delay_time = 400;
  int num_reps = 5;
  int counter = 0;
  string usr_response;

  int red_reading = 0;
  int blue_reading = 0;

	while (true) {
    cout << "Type 'yes' and press 'enter' when egg is ready." << endl;
    cin >> usr_response;
    for (int i = 0; i < num_reps; i++) {
  		red_reading += components.colour_detector.read_red_sensor();
  		blue_reading += components.colour_detector.read_blue_sensor();
    }

    red_reading /= num_reps;
    blue_reading /= num_reps;

    cout << counter << "\n - Reading is:\n    BLUE: " << blue_reading << "\n    RED: "
      << red_reading << "\n" << endl;
		delay(delay_time);

    red_reading = 0;
    blue_reading = 0;

    components.compartment.turn_to_position(2);
    delay(300);
    components.compartment.return_to_default();
    counter ++;
	}

  cout << "Finished" << endl;

  return 1;
}
