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

  int colour_sensor_1_port = 1; // Port number for first colour detection ADC
  int colour_sensor_2_port = 2; // Port number for second colour detection ADC

  Colour_Detector detector(colour_sensor_1_port, colour_sensor_2_port);

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
  		red_reading += detector.read_red_sensor();
  		blue_reading += detector.read_blue_sensor();
    }

    red_reading /= num_reps;
    blue_reading /= num_reps;

    cout << counter << "\n - Reading is:\n    BLUE: " << blue_reading << "\n    RED: "
      << red_reading << "\n" << endl;
		delay(delay_time);

    red_reading = 0;
    blue_reading = 0;
    counter ++;
	}

  cout << "Finished" << endl;

  return 1;
}
