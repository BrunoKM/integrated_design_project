#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
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
    return false;
  } else {
    cout << "Robot initialised succesfully" << endl;
    return true;
  }
}

int main () {
  initialise_robot();
  int val;
  int delay_time = 500;

  for (int i=0; i < 8; i++) {
    val = 1 << i;
    rlink.command(WRITE_PORT_1, val);
    delay(delay_time);
  }
  return 0;                          // Test succesful
}
