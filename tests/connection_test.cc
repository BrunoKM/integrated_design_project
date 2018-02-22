#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#define ROBOT_NUM 50   // The id number for wifi card
robot_link rlink;      // datatype for the robot link

int main () {
  int val;
  #ifdef __arm__
    cout << "Running code on MICROCONTROLLER" << endl;
    if (!rlink.initialise ("127.0.0.1")) {          // setup for local hardware
  #else
    cout << "Running code REMOTELY" << endl;
    if (!rlink.initialise (ROBOT_NUM)) {            // setup the link
  #endif
    cout << "Cannot initialise link" << endl;
    rlink.print_errs("  ");
    return -1;
  }
  val = rlink.request (TEST_INSTRUCTION); // send test instruction
  if (val == TEST_INSTRUCTION_RESULT) {   // check result
    cout << "Test passed" << endl;
    return 0;                            // all OK, finish
  }
  else if (val == REQUEST_ERROR) {
    cout << "Fatal errors on link:" << endl;
    rlink.print_errs();
  }
  else
    cout << "Test failed (bad value returned)" << endl;
  return -1;                          // error, finish
}
