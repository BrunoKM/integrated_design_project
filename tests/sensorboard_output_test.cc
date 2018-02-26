#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <bitset>
#include "robot_initialise.h"

int main () {
  initialise_robot();
  int val;
  int read_val;
  int delay_time = 500; // Delay in ms between changing output

  for (int i=0; i < 8; i++) { // Cycle over 8 outputs
    val = 1 << i; // Write 1 to the i-th bit, 0 to others
    rlink.command(WRITE_PORT_1, val);
    delay(10);
    read_val = rlink.request(READ_PORT_1);
    bitset<8> val_bit(read_val);
    cout << "Current input reads: " << val_bit << endl;
    delay(delay_time);
    
  }

  // Print errors if there are any.
  if (rlink.any_errs()) {
	  rlink.print_errs(" ");
	  return -1;
  } else {
	  cout << "No Errors" << endl;
  }
  return 0;                          // Test succesful
}
