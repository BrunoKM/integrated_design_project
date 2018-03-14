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

void blink_leds() {
  rlink.command(WRITE_PORT_1, (1<<6) + (1<<7));
  delay(100);
  rlink.command(WRITE_PORT_1, 0);
}

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
	cout << "Components initialised" << endl;
  // stopwatch watch;
  // watch.start();
  // cout << "Set the angle to 0 to begin with." << endl;
  // components.turntable_comms.set_angle(0);

  int delay_time = 3000;
  int counter = 0;
  int write_byte = 0;

	while (true) {
    blink_leds(); // Just to know there is a connection
    // Ask user for the angle:
    cout << "Input the desired control signal (between 0 and 255)" << endl;
    cin >> write_byte;

		components.turntable_comms.write(255);
    cout << counter << "- Sent signal is: " << write_byte << endl;
		// rlink.command(WRITE_PORT_2, write_byte);

		counter ++;
		delay(delay_time);
	}

  cout << "Finished" << endl;

  return 1;
}
