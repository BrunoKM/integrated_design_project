#include "robot_initialise.h"
#include <iostream>
using namespace std;
#include <robot_instr.h>

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
