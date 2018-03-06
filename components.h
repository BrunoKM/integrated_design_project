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
  int write_default; // used for and'ing with the values to be sent to write'able bits.
  void initialise_write_default();
  void command_write_default();
  int read_state();
public:
  PCB(int port);
};


class PCB1 : public PCB {
private:
  // Line following sensors (read only)
  const int num_line_sensors = 4;
  const int line_sensor_bits = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3);
  //  IR LED for beacon communication (write only)
  const int ir_led_bit = 1 << 4;
  //  IR Phototransistor for beacon communication (read only)
  const int ir_input_bit = 1 << 5;

public:
  PCB1(int port);
  void read_initialise();
  int read_line_sensors(); // The int value (binary) of the line sensors reading
  int read_ir_input();
};


class PCB2 : public PCB {
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

public:
  PCB2(int port);
  void write_leds(int led1_val, int led2_val); //TODO: May potentially have to apply a timer to make sure leds stay on for long enough.
  // Wrappers for command_write_default:
  void read_initialise();
  void reset_microswitches();

  void write_scoop(int scoop_val);
};


struct Line_Sensor_Reading {
  bool front_left;
  bool front_right;
  bool back_left;
  bool back_right;
}

class Line_Sensors {
private:
  PCB1 pcb;
  // Define the order of line sensors connections:
  const int front_left_bit = 1 << 0;
  const int front_right_bit = 1 << 1;
  const int back_left_bit = 1 << 2;
  const int back_right_bit = 1 << 3;
public:
  Line_Sensors(PCB1 pcb) : pcb(pcb){};
  Line_Sensor_Reading get_sensor_reading();
}


// TODO: Finish class LEDs
class LEDs {
private:
  const int led1_pin;
  const int led2_pin;
public:
  LEDs(int led1_pin, int led2_pin);
  void off();
  void display_egg(Egg egg);
};

// TODO: Finnish classes below.
// class Scoop {
// const int address - the pin address of the scoop actuator
// Public Methods
// void scoop()
// }
// class Reservoir:
// const int left_switch_address
// const int right_switch_address
// Public Methods:
// void put_egg_into_delivery() - Rotate the reservoir to put the egg into delivery sub-reservoir and return to starting position.
// void put_egg_into_recycling()- Rotate the reservoir to put the egg into recycling sub-reservoir and return to starting position.
// void dispose_delivery() - Rotate to let out the delivery eggs and rotate back to starting position.
// void dispose_recycling() - Rotate to let out the recycling eggs and rotate back to starting position.
// void return_to_starting(char which_side) - Returns to starting position either from left (which_side='l') or from the right (which_side='r').

#endif
