// Go until second intersection and align with it (axis over intersection), turn around, repeat

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

  int pcb1_port = 0; // Port number for PCB 1

  Components components(pcb1_port);

  stopwatch watch;
  watch.start();
  while (watch.read() < 10000) {
    Line_Sensor_Reading reading;
    reading = components.line_sensors.get_sensor_reading();
    cout << "Reading is: Front: " << reading.front_left << " " 
    << reading.front_right
    << " Back: " << reading.back_left << " " << reading.back_right << endl;
  }

  cout << "Finished" << endl;

  return 1;
}
