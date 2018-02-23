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
  float test_time = 5000; //test for test_time ms
  stopwatch watch;

  rlink.command(WRITE_PORT_1, 0xFF); // Write 11111111 to activate all 8 inputs

  watch.start();
  // Allow user to set different at the input and display them
  while (watch.read() < test_time) {
    val = rlink.request(READ_PORT_1)
    bitset<8> val_bit(val);
    cout << "\rCurrent input reads: " << val_bit << flush;
  }
  return 0;                          // Test succesful
}
