// Go until second intersection and align with it (axis over intersection), turn around, repeat

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include "../robot_initialise.h"
#include "../line_following.h"
#include "../components.h"

int main () {
  int num_repetitions = 20;

  initialise_robot();
  cout << "Running line_following_test between intersections ("
  << num_repetitions << " repetitions)." << endl;

  int pcb1_port = 1; // Port number for PCB 1

  Components components(pcb1_port);
  Line_Following lf(components.line_sensors);

  // Set parameters for the test
  float speed = 0.5; // Set speed of motors in range 0-1
  float ramp =  120;// Set acceleration of motors in range 0-254
  lf.set_ramp(ramp);

  float speed_delta = 0.1;
  int num_intersections_to_ignore = 1;
  bool keep_driving_after_last = 1; // Decides whether to stop motors when the final intersection is reached

  for (int i = 0; i < num_repetitions; i++) {
    lf.follow_line(speed, ramp, speed_delta,
                num_intersections_to_ignore, keep_driving_after_last);
    lf.align_with_intersection(300);
    lf.turn(180);
  }


  cout << "Finished" << endl;

  return 1;
}
