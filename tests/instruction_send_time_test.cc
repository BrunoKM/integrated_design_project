#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch>
#define ROBOT_NUM 50   // The id number for wifi card
robot_link rlink;      // datatype for the robot link

bool initialise_robot (void) {
  #ifdef __arm__
    if (!rlink.initialise ("127.0.0.1")) {          // setup for local hardware
  #else
    if (!rlink.initialise (ROBOT_NUM)) {            // setup the link
  #endif
    cout << "Cannot initialise link" << endl;
    rlink.print_errs("  ");
    return -1;
  } else {
    cout << "Robot initialised succesfully" << endl;
  }
}

int main () {
  int val;

  initialise_robot()

  int n_repetetions = 10000;

  stopwatch watch;
  watch.start()
  for (int i = 0; i < n_repetitions; i++) {
    val = rlink.request (TEST_INSTRUCTION); // send test instruction
    if (val != TEST_INSTRUCTION_RESULT) {
      cout << "Test failed (bad value returned)" << endl;
      return -1;        // Finish with error
    }
  }
  total_time = watch.stop();
  instruction_time = total_time / n_repetitions;
  cout << "Total time of test: " << total_time << "per " << n_repetetions << "iterations\n"
  << "Time taken per instruction: " << instruction_time << endl; // Print the results

  return 0;                          // Test succesful
}
