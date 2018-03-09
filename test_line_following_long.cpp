// Go until second intersection and align with it (axis over intersection), turn around, repeat

#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"

int main () {
  int num_repetitions = 20;

  initialise_robot();
  cout << "Running line_following_test between intersections ("
  << num_repetitions << " repetitions)." << endl;

  int pcb1_port = 0; // Port number for PCB 1
  int pcb2_port = 1;

  Components components(pcb1_port, pcb2_port);
  Line_Following lf(components);

  // Set parameters for the test
  float speed = 1.0; // Set speed of motors in range 0-1
  float turn_speed = 0.95;
  float ramp =  50;// Set acceleration of motors in range 0-254
  lf.set_ramp(ramp);

  float speed_delta = 0.5;
  int num_intersections_to_ignore = 1;
  bool keep_driving_after_last = 1; // Decides whether to stop motors when the final intersection is reached

  for (int i = 1; i <= num_repetitions; i++) {
    cout << " --- Going from start. Iter: " << i << endl;
    lf.follow_line(speed, speed_delta,
                num_intersections_to_ignore, keep_driving_after_last);
    lf.align_with_intersection(speed, speed_delta);
    lf.turn(180, turn_speed);
    cout << " --- Going towards start. Iter: " << i << endl;
    lf.follow_line(speed, speed_delta,
                num_intersections_to_ignore, keep_driving_after_last);
    lf.align_with_intersection(speed, speed_delta);
    lf.turn(-180, turn_speed);
  }


  cout << "Finished" << endl;

  return 1;
}
