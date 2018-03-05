#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include "robot_initialise.h"
#include "line_following.h"

int main () {
  initialise_robot();
  cout << "Running line_following_test_first_intersection" << endl;
  // Set parameters for the test
  float speed = 0.5; // Set speed of motors in range 0-1
  float ramp = 0.5; // Set acceleration of motors in range 0-1
  float speed_delta = 0.1; // NOT SURE WHAT THIS IS???
  int num_intersections_to_ignore = 0;
  bool keep_driving_after_last = 1; // Decides whether to stop motors when the final intersection is reached
  
  follow_line(speed, ramp, speed_delta,
              num_intersections_to_ignore, keep_driving_after_last);

  cout << "Finished" << endl;

  // Print errors if there are any.
  if (rlink.any_errs()) {
    rlink.print_errs(" ");
    return -1;
  } else {
    cout << "No Errors" << endl;
  }
  return 0;
}