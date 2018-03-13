// Test and print out the inputs from each line sensor in the order specified in the Line_Sensors class
#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"

int main () {
  cout << "Running the line sensor output test" << endl;
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
  while (watch.read() < 15000) {
    Line_Sensor_Reading reading;
    reading = components.line_sensors.get_sensor_reading();
    cout << "Reading is: Front: " << reading.front_left << " "
    << reading.front_right
    << " Back: " << reading.back_left << " " << reading.back_right << endl;
  }

  cout << "Finished" << endl;

  return 1;
}
