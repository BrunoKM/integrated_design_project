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
  int read_val;
  int delay_time = 5000; // Delay in ms between changing output
  
  rlink.command(WRITE_PORT_1, 0xFF);
  
  read_val = rlink.request(READ_PORT_1);
  bitset<8> val_bit0(read_val);
  cout << "Input before delay reads: " << val_bit0 << endl;
  
  delay(delay_time);
  cout << "Delay completed" << endl;
  
  read_val = rlink.request(READ_PORT_1);
  bitset<8> val_bit1(read_val);
  cout << "Input after 1st delay reads: " << val_bit1 << endl;
  
  delay(delay_time);
  cout << "Delay 2 completed" << endl;
  
  read_val = rlink.request(READ_PORT_1);
  bitset<8> val_bit2(read_val);
  cout << "Input after 1st delay reads: " << val_bit2 << endl;
 }
