#include <iostream>
#include <robot_instr.h>
#include "robot_initialise.h"

#define ROBOT_NUM 50   // The id number for wifi card
robot_link rlink;      // datatype for the robot link

void initialise_robot (void) {
  #ifdef __arm__
    if (!rlink.initialise ("127.0.0.1")) {          // setup for local hardware
  #else
    if (!rlink.initialise (ROBOT_NUM)) {            // setup the link
  #endif
    if (rlink.fatal_err()) { // check for errors
      rlink.print_errs(); // print them out
      rlink.reinitialise(); // flush the link
      std::cout << "Robot re-initialised" << std::endl;
      return;
    }
  } else {
    std::cout << "Robot initialised succesfully" << std::endl;
    return;
  }
}
