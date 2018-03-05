// Defines several classes for each major component of the Robot which offer
// methods that hide the implementation of the tasks each of the components
// is expected to execute.
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "robot_initialise.h"

class PCB {
private:
  int port;

};

class PCB1 : public PCB {
private:
  int address; // Define the pcb address here
  int inst_microswitch_bits[2] = 
  int inst_microswitch_bits(2)
  : Persistent microswitches; stay ON after microswitch triggered, have to be reset by software (read and write).
Bit 2 - 4 : Instantaneous microswitches; do not have to be reset in software (read only).
Bit 5 : Scoop actuator control (write only)
Bit 6 - 7 : LEDs to signal colour detection (write only)
public:
  PCB1();
}


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
