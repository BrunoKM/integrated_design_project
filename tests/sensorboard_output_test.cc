#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include "robot_initialise.h"

int main () {
  initialise_robot();
  int val;
  int delay_time = 500;

  for (int i=0; i < 8; i++) {
    val = 1 << i;
    rlink.command(WRITE_PORT_1, val);
    delay(delay_time);
  }
  
  if (rlink.any_errs()) {
	  rlink.print_errs(" ");
	  return -1;
  } else {
	  cout << "No Errors" << endl;
      cout << "Done" << endl;
  }
  return 0;                          // Test succesful
}
