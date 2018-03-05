#include "components.h"


PCB::PCB(int port) : port(port) {

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
}

void PCB::command_write_default() {
  rlink.command(write_instruction, write_default);
}

void PCB::read_state() {
  int state;
  state = rlink.request(read_instruction);
  return state
}

int PCB1::init_line_sensor_mask() {
  for (int i = 0; i < num_line_sensors; i++) {
    line_sensor_mask = line_sensor_mask bitor (1 << line_sensor_bits[i]);
  }
}

void PCB1::initialise_write_default() {
  write_default = 0;
  // The default values for line sensors are 1:
  write_default = write_default bitor line_sensor_mask;
  // The default value for IR LED is 0
  // The default for IR Input sensor i 1;
  write_default = write_default bitor (1 << ir_input_bit);
  // The default value for scoop is 0
  // The default value for leds is 0
};

PCB1::PCB1(int port): PCB(port) {
  init_line_sensor_mask();
  initialise_write_default();
  read_initialise();
}

int PCB1::read_line_sensors() {
   // Returns the int value (binary) of the line sensors reading
   int sensor_reading = read_state() bitand line_sensor_mask;
   return sensor_reading;
}

int PCB1::read_ir_input() {
  int ir_reading = read_state() bitand (1 << ir_input_bit);
  return ir_reading;
}

PCB2::PCB2(int port): PCB(port) {
  initialise_write_default();
  read_initialise();
}

void PCB2::initialise_write_default() {
  write_default = 0;
  // The default values for persistent microswitches are 0
  // The default values for instantaneous microswitches have to be 1 (to allow for reading)
  // Have to alter write default:
  for (int i = 0; i < num_inst_microswitches; i++) {
    write_default = write_default bitor (1 << inst_microswitch_bits[i]);
  }
  // The default value for scoop is 0
  // The default value for leds is 0
};

void PCB2::read_initialise() {
  command_write_default();
}

void PCB2::reset_microswitches() {
  command_write_default();
}

void PCB2::write_leds(int led0_val, int led1_val) {
  int led0_byte_val = led0_val << leds_bits[0];
  int led1_byte_val = led1_val << leds_bits[1];
  int write_byte = write_default bitor led0_byte_val bitor led1_byte_val;

  rlink.command(write_instruction, write_byte);
}

void write_scoop(int scoop_val) {
  // TODO: finish this
}


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
