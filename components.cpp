#include "components.h"


class PCB1 : public PCB {
private:
  // Persistent microswitches; stay ON after microswitch triggered, have to be reset by software (read and write).
  int persistent_microswitch_bits[2] = {0, 1};
  // Instantaneous microswitches; do not have to be reset in software (read only).
  int inst_microswitch_bits[3] = {2, 3, 4};
  // Scoop actuator control (write only)
  int scoop_actuator_bit = 5;
  // LEDs to signal colour detection (write only)
  int leds_bits[2] = {6, 7};
  int default_write;
public:
  PCB1(int port);
  void read_initialise();
};

PCB1::PCB1(int port) : port(port) {
  initialise_write_default();

  // Assign the right write, read instructions:
  switch (port) {
    case 0:
      write_instruction = WRITE_PORT_0;
      read_instruction = READ_PORT_0;
      break;
    case 1:
      write_instruction = WRITE_PORT_1;
      read_instruction = READ_PORT_1;
      break;
    case 2:
      write_instruction = WRITE_PORT_2;
      read_instruction = READ_PORT_2;
      break;
    case 3:
      write_instruction = WRITE_PORT_3;
      read_instruction = READ_PORT_3;
      break;
    case 4:
      write_instruction = WRITE_PORT_4;
      read_instruction = READ_PORT_4;
      break;
    case 5:
      write_instruction = WRITE_PORT_5;
      read_instruction = READ_PORT_5;
      break;
    case 6:
      write_instruction = WRITE_PORT_6;
      read_instruction = READ_PORT_6;
      break;
    case 7:
      write_instruction = WRITE_PORT_7;
      read_instruction = READ_PORT_7;
      break;
  }
  read_initialise();
}

void PCB1::initialise_write_default() {
  write_default = 0;
  // The default values for persistent microswitches are 0
  // The default values for instantaneous microswitches have to be 1 (to allow for reading)
  // Have to alter write default:
  for (int i; i < num_inst_microswitches; i++) {
    write_default = write_default bitor (1 << inst_microswitch_bits[i]);
  }
  // The default value for scoop is 0
  // The default value for leds is 0
};

void PCB1::read_initialise() {
  command_write_default();
}

void reset_microswitches() {
  command_write_default();
}

void PCB1::command_write_default() {
  rlink.command(write_instruction, write_default);
}

void write_leds(int led0_val, int led1_val) {
  int led0_byte_val = led0_val << leds_bits[0];
  int led1_byte_val = led1_val << leds_bits[1];
  int write_byte = write_default bitor led0_byte_val bitor led1_byte_val;

  rlink.command(write_instruction, write_byte);
}

// Wrappers for command_write_default:
void read_initialise();
void reset_microswitches();
void write_scoop(int scoop_val);


class LEDs {
private:
  const int leds_port;
  const int led1_pin;
  const int led2_pin;
public:
  LEDs(int ledsport, int led1_pin, int led2_pin);
  void off();
  void display_egg(Egg egg);
};

LEDs::LEDs(int leds_port, int led1_pin, int led2_pin):
port(leds_port), led1_pin(led1_pin), led2_pin(led2_pin) {

}

LEDs::off() {
}
