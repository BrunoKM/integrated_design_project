// Defines several classes for each major component of the Robot which offer
// methods that hide the implementation of the tasks each of the components
// is expected to execute.

// Also defines all the pin allocations for the PCB.
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <robot_link>
#include "robot_initialise.h"

class PCB {
private:
  int port;
  request_instruction read_instruction;
  command_instruction write_instruction;
};

class PCB1 : public PCB {
private:
  // Persistent microswitches; stay ON after microswitch triggered, have to be reset by software (read and write).
  const int num_persistent_microswitches = 2;
  const int persistent_microswitch_bits[num_persistent_microswitches] = {0, 1};
  // Instantaneous microswitches; do not have to be reset in software (read only).
  const int num_inst_microswitches = 3;
  const int inst_microswitch_bits[num_inst_microswitches] = {2, 3, 4};
  // Scoop actuator control (write only)
  const int scoop_actuator_bit = 5;
  // LEDs to signal colour detection (write only)
  const int num_leds = 2;
  const int leds_bits[num_leds] = {6, 7};
  int write_default; // used for and'ing with the values to be sent to write'able bits.
  void initialise_write_default();
public:
  PCB1(int port);
  void read_initialise();
};


class LEDs {
private:
  const int led1_pin;
  const int led2_pin;
public:
  LEDs(int led1_pin, int led2_pin);
  void off();
  void display_egg(Egg egg);
};

class Scoop {
const int address - the pin address of the scoop actuator
Public Methods
void scoop()
}
class Reservoir:
const int left_switch_address
const int right_switch_address
Public Methods:
void put_egg_into_delivery() - Rotate the reservoir to put the egg into delivery sub-reservoir and return to starting position.
void put_egg_into_recycling()- Rotate the reservoir to put the egg into recycling sub-reservoir and return to starting position.
void dispose_delivery() - Rotate to let out the delivery eggs and rotate back to starting position.
void dispose_recycling() - Rotate to let out the recycling eggs and rotate back to starting position.
void return_to_starting(char which_side) - Returns to starting position either from left (which_side='l') or from the right (which_side='r').

#endif
