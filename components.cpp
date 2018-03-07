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

int PCB::read_state() {
  int state;
  state = rlink.request(read_instruction);
  return state;
}


void PCB1::initialise_write_default() {
  write_default = 0;

  // The default values for line sensors are 1:
  write_default = write_default bitor line_sensor_bits;
  // The default value for IR LED is 0

  // The default for IR Input sensor i 1:
  write_default = write_default bitor ir_input_bit;
};

PCB1::PCB1(int& port): PCB(port) {
  initialise_write_default();
  read_initialise();
}

int PCB1::read_line_sensors() {
   // Returns the int value (binary) of the line sensors reading
   int sensor_reading = read_state() bitand line_sensor_bits;
   return sensor_reading;
}

int PCB1::read_ir_input() {
  int ir_reading = read_state();
  if ((ir_reading bitand ir_input_bit) > 0) {
    return 1;
  } else {
  return 0;
  }
}

void PCB1::read_initialise() {
  command_write_default();
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
  write_default = write_default bitor inst_microswitch_bits;
  // The default value for scoop is 0
  // The default value for leds is 0
};

void PCB2::read_initialise() {
  command_write_default();
}

void PCB2::reset_microswitches() {
  command_write_default();
}

int PCB2::read_microswitches() {
  int switches_reading = read_state() bitand (inst_microswitch_bits bitor persistent_microswitch_bits);
  return switches_reading;
}

void PCB2::write_leds(int led1_val, int led2_val) {
  int led1_byte_val = led1_bit * led1_val;
  int led2_byte_val = led2_bit * led2_val;
  int write_byte = write_default bitor led1_byte_val bitor led2_byte_val;

  rlink.command(write_instruction, write_byte);
}

void PCB2::write_scoop(int scoop_val) {
  // TODO: finish this
}

Line_Sensor_Reading Line_Sensors::get_sensor_reading() {
  int sensor_state = pcb1.read_line_sensors();
  Line_Sensor_Reading reading;
  // Assign to each value of the reading:
  if (sensor_state bitand front_left_bit) {
    reading.front_left = true;
  } else {
    reading.front_left = false;
  }
  if (sensor_state bitand front_right_bit) {
    reading.front_right = true;
  } else {
    reading.front_right = false;
  }
  if (sensor_state bitand back_left_bit) {
    reading.back_left = true;
  } else {
    reading.back_left = false;
  }
  if (sensor_state bitand back_right_bit) {
    reading.back_right = true;
  } else {
    reading.back_right = false;
  }
  return reading;
}


void Microswitches::update_state() {
  int state = pcb2.read_microswitches();
  if (state bitand front_switch_bit) {
    front_state = true;
  } else {
    front_state = false;
  }
  if (state bitand rear_switch_bit) {
    rear_state = true;
  } else {
    rear_state = false;
  }
}


// class LEDs {
// private:
//   const int leds_port;
//   const int led1_pin;
//   const int led2_pin;
// public:
//   LEDs(int ledsport, int led1_pin, int led2_pin);
//   void off();
//   void display_egg(Egg egg);
// };

// LEDs::LEDs(int leds_port, int led1_pin, int led2_pin):
// port(leds_port), led1_pin(led1_pin), led2_pin(led2_pin) {
//
// }
//
// LEDs::off() {
// }
