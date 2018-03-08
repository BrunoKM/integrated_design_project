#include "components.h"
#include <delay.h>




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
  // The default for microswitch input is 1:
  write_default = write_default bitor inst_microswitch_bits;
  // The default value for IR LED is 0

// TODO: See what's actually connected here
  // The default for IR Input sensor i 1:
  // write_default = write_default bitor ir_input_bit;
};

PCB1::PCB1(int& port): PCB(port) {
  initialise_write_default();
  read_initialise();
}

// TODO: Put this into the IR class instead

/*
int PCB1::read_ir_input() {
  int ir_reading = read_state();
  if ((ir_reading bitand ir_input_bit) > 0) {
    return 1;
  } else {
  return 0;
  }
}
*/

void PCB1::read_initialise() {
  command_write_default();
}

/* No longer necessary
void PCB1::reset_microswitches() {
  command_write_default();
}
*/



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


void PCB2::write(int byte) {
  int write_byte = write_default bitor byte;
  rlink.command(write_instruction, write_byte);
  return;
} */

void PCB2::write_scoop(int scoop_val) {
  // TODO: finish this
}

Line_Sensor_Reading Line_Sensors::get_sensor_reading() {
  int sensor_state = pcb.read_state() bitand pcb.line_sensor_bits;

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
  int state = pcb.read_state() bitand (pcb.inst_microswitch_bits);
  if (state bitand front_switch_bit) {
    front_state = false; // Microswitch on when unpressed for some reason
  } else {
    front_state = true;
  }
  if (state bitand rear_switch_bit) {
    rear_state = false;
  } else {
    rear_state = true;
  }
  return;
}


// TODO: Finish LEDs once the pin allocation is known



void LEDs::off() {
  // Set all the LEDs to 0
  int out_byte = 0;
  pcb.write(out_byte);
  return;
}

void LEDs::write_leds(int led1_val, int led2_val) {
  int led1_byte_val = led1_bit * led1_val;
  int led2_byte_val = led2_bit * led2_val;
  int out_byte = led1_byte_val bitor led2_byte_val;
  pcb.write(out_byte);
  return;
}

void LEDs::display_egg(Egg egg) {
  // Set the LEDs to represent the egg
  int delay_time = 100;
  int short_delay_time = 50;
  if ((egg.size == 0) && (egg.colour == "b")) {
    // Write 00
    write_leds(1, 0);
    delay(delay_time);
    off();
  } else if ((egg.size == 0) && (egg.colour == "y")) {
    // Write 01
    write_leds(0, 1);
    delay(delay_time);
    off();
  } else if ((egg.size == 1) && (egg.colour == "p")) {
	  // For big eggs, blink
    // Write 10
    write_leds(1, 0);
    delay(short_delay_time);
    off();
    delay(short_delay_time);
    write_leds(1, 0);
    delay(short_delay_time);
    off();
  } else if ((egg.size == 1) && (egg.colour == "y")) {
    // Write 11
    write_leds(0, 1);
    delay(short_delay_time);
    off();
    delay(short_delay_time);
    write_leds(0, 1);
    delay(short_delay_time);
    off();
  }
  return;
}


Eggs::Eggs() {}

void Eggs::add_egg(Egg egg) {

}

void Eggs::clear() {
  // Clears all the eggs, i.e. no eggs in the eggs vector after this
  // function is called.
}



// TODO: FINISH IR_communications class when we understand whats going on...
/*IR_communication::IR_communication(PCB1 &pcb1) {
}

int IR_communication::get_delivery_point() {
  // Gets start sensor reading from the IR beacon
  int IR_state = pcb1.read_IR_input(); // returns a 1 or 0 from the infrared sensor


}

void IR_communication::rotate_turntable(int degrees) {
  // Rotates turn table by a fixed number of degrees (degrees will be fixed to the set interval between eggs)
}

void IR_communication::rotate_turntable_start_position() {
  // Rotates turn table all the way to one side (e.g all the way clockwise in order to start tracking eggs)
}*/
