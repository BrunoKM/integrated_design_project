// Go until second intersection and align with it (axis over intersection), turn around, repeat

#include <iostream>
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

  detector = Colour_Detector(colour_sensor_1_port, colour_sensor_2_port);

  int delay_time = 3000;
  int counter = 0;

	while (true) {
		int red_reading = detector.read_red_sensor();
    int blue_reading = detector.read_blue_sensor();
		counter ++;
    cout << counter << " - Reading is: BLUE: " << blue_reading << "   RED: "
    << red_reading << endl;
		delay(1000);
	}

  cout << "Finished" << endl;

  return 1;
}
