#include <vector>
#include <iostream>
#include <stdlib.h>
#include <delay.h>
#include <stopwatch.h>
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

void PCB::write(int byte) {
  int write_byte = write_default bitor byte;
  rlink.command(write_instruction, write_byte);
  return;
}

void PCB1::initialise_write_default() {
  write_default = 0;

  // The default values for line sensors are 1:
  write_default = write_default bitor line_sensor_bits;
  // The default value for scoop actuator is 0
  return;
};

PCB1::PCB1(int& port): PCB(port) {
  initialise_write_default();
  read_initialise();
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
  // The default value for contact flip-flops readings is 1:
  write_default = write_default bitor contact_flops_bits;
  // The default value for contact flip-flops reset is 0.
  // The default value for instantaneous microswitches is 1:
  write_default = write_default bitor inst_microswitch_bits;
  // The default value for the egg display LEDS is 0.
};

void PCB2::read_initialise() {
  command_write_default();
}

ADC::ADC(int port) : port(port) {

  // Assign the right write, read instructions:
  switch (port) {
    case 0:
      read_instruction = ADC0;
      break;
    case 1:
      read_instruction = ADC1;
      break;
    case 2:
      read_instruction = ADC2;
      break;
    case 3:
      read_instruction = ADC3;
      break;
    case 4:
      read_instruction = ADC4;
      break;
    case 5:
      read_instruction = ADC5;
      break;
    case 6:
      read_instruction = ADC6;
      break;
    case 7:
      read_instruction = ADC7;
      break;
  }
}

int ADC::read_state() {
  int state;
  state = rlink.request(read_instruction);
  return state;
}

Turntable_Comms::Turntable_Comms(int address) : address(address) {
  switch (address) {
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

void Turntable_Comms::write(int write_byte) {
  rlink.command(write_instruction, write_byte);
  return;
}

void Turntable_Comms::set_angle(int degrees) {
  // Put some conversion code in here.
  int write_byte = degrees;

  write(write_byte);
};

void Turntable_Comms::set_position(int position) {
  // Position 1 - 8 for eggs. Odd for small, even for large ones.
  int write_byte;
  switch (position) {
    case 0:
      // Position 0 is empty, choose for when first coming in
      write_byte = 0;
      break;
    case 1:
      write_byte = 30; // TODO: Figure out write_bytes for each position
      break;
    case 2:
      write_byte = 55;
      break;
    case 3:
      write_byte = 90;
      break;
    case 4:
      write_byte = 120;
      break;
    case 5:
      write_byte = 150;
      break;
    case 6:
      write_byte = 180;
      break;
    case 7:
      write_byte = 225;
      break;
    case 8:
      write_byte = 255;
      break;
  }
  write(write_byte);
  return;
}



int Beacon_Reader::get_beacon_code() {
  std::cout << " + Reading the beacon code." << std::endl;

  stopwatch watch;
  watch.start();

  int current_state = 0; // Current state of the ADC
  // Make sure that we don't start reading amid transmission
  while (watch.read() < 250) {
    current_state = read_state();
    if (current_state > reading_threshold) {
      // If a +ve voltage detected, restart the watch.
      watch.start();
    }
  }

  // Now use watch to read time between changes in readings:
  watch.start(); // Restart

  int pulse_count = 0; // Keep count of positive pulses

  bool reading = 0; // current reading
  bool last_reading = 0; //
  // If there hasn't been a reading for 200 ms, stop reading
  while ((pulse_count == 0) or (watch.read() < 160)) {
    current_state = read_state();
    reading = (current_state >= reading_threshold);
    if (reading != last_reading) {
      // Check that the duration of the last signal is of sufficient length
      if (watch.read() < 50) {
		std::cout << " ! pulse reading too short " << std::endl;
        // Try reading the code again if an error occurs (TODO: potentially less lazy solution?)
        // Infinite recursion possibilities, yeeet
        return get_beacon_code();
      }
      if (reading == 1) {
        pulse_count += 1;

      }
      last_reading = reading;
      watch.start();
    }
  }
  return pulse_count;
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
    front_state = true;
  } else {
    front_state = false;
  }
  if (state bitand rear_switch_bit) {
    rear_state = true;
  } else {
    rear_state = false;
  }
  return;
}



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
  if ((egg.size == 0) && (egg.colour == 'b')) {
    // Write 00
    write_leds(1, 0);
    delay(delay_time);
    off();
  } else if ((egg.size == 0) && (egg.colour == 'y')) {
    // Write 01
    write_leds(0, 1);
    delay(delay_time);
    off();
  } else if ((egg.size == 1) && (egg.colour == 'p')) {
	  // For big eggs, blink
    // Write 10
    write_leds(1, 0);
    delay(short_delay_time);
    off();
    delay(short_delay_time);
    write_leds(1, 0);
    delay(short_delay_time);
    off();
  } else if ((egg.size == 1) && (egg.colour == 'y')) {
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


void Scoop::contract() {
  pcb.write(scoop_bit);
}

void Scoop::release() {
    pcb.write(0);
};


Eggs::Eggs() {}

void Eggs::add_egg(Egg egg) {

}

void Eggs::clear() {
  // Clears all the eggs, i.e. no eggs in the eggs vector after this
  // function is called.
}

int Colour_Detector::read_red_sensor() {
  return red_sensor.read_state();
}

int Colour_Detector::read_blue_sensor() {
  return blue_sensor.read_state();
}

Egg Colour_Detector::classify_egg(int size) {
  // Some decision logic for colour:
  char colour;
  switch (size) {
    case 0:
      // Decide between pink and yellow
      break;
    case 1:
      // Decide between yellow and blue
      break;
  }
  colour = 'y'; // Remove
  return Egg(size, colour);
}

Rotating_Compartment::Rotating_Compartment(PCB2 pcb) :
pcb(pcb),
motor(Motor(3, 1.0, 1.0)),
current_position(3) {
}

void Rotating_Compartment::turn_exactly(int degrees, bool stop_after) {
  float rotate_time_360 = 6001; // TODO: Recalibrate

  int rotate_for = rotate_time_360 * float(abs(degrees)) / 360.0;
  // Start rotating
  if (degrees < 0) {
    motor.drive(-speed);
  } else if (degrees > 0) {
    motor.drive(speed);
  }
  delay(rotate_for);
  // Stop the motors
  if (stop_after) {
    motor.drive(0);
  }
  return;
}

bool Rotating_Compartment::read_left_flop() {
  int reading = pcb.read_state();
  reading = reading bitor left_flop_bit;
  if (reading > 0) {
    return true;
  } else {
    return false;
  }
}
bool Rotating_Compartment::read_right_flop() {
  int reading = pcb.read_state();
  reading = reading bitor right_flop_bit;
  if (reading > 0) {
    return true;
  } else {
    return false;
  }
}
void Rotating_Compartment::reset_flops() {
  pcb.write(reset_flops_bit);
}

void Rotating_Compartment::turn_to_position(int position) {
  // Assumes starting at position 3 (the centre), and turns to one of the desired positions.
  switch (position) {
    case 1:
      turn_to_position(2);

      turn_exactly(-10, false);
      reset_flops();
      
      while (read_right_flop() == 0) {
      }
      motor.drive(0);
      break;
    case 2:
      turn_exactly(-10, false);
      reset_flops();
      
      while (read_right_flop() == 0) {
      }
      motor.drive(0);
      break;
    case 4:
      turn_exactly(10, false);
      reset_flops();

      while (read_left_flop() == 0) {
      }
      motor.drive(0);
      break;
    case 5:
      turn_to_position(4);

      turn_exactly(10, false);
      reset_flops();

      while (read_left_flop() == 0) {
      }
      motor.drive(0);
      break;
    default:
      std::cout << "Wrong input " << position << " to function turn_to_position" << std::endl;
  }
  current_position = position;
}

void Rotating_Compartment::return_to_default() {
  switch (current_position) {
    case 1:
      turn_exactly(10, false);
      reset_flops();
      // Turn until the rotor makes first contact
      while (read_right_flop() == 0) {}
      
      // Turn until the rotor makes second contact and stop
      turn_exactly(10, false);
      reset_flops();
      while (read_right_flop() == 0) {}
      // Stop the motor
      motor.drive(0);
      break;
    case 2:
      turn_exactly(10, false);
      reset_flops();
      // Turn until the rotor makes first contact
      while (read_right_flop() == 0) {}
      // Stop the motor
      motor.drive(0);
      break;
    case 4:
      turn_exactly(-10, false);
      reset_flops();
      // Turn until the rotor makes first contact
      while (read_left_flop() == 0) {}
      // Stop the motor
      motor.drive(0);
      break;
    case 5:
      turn_exactly(-10, false);
      reset_flops();
      // Turn until the rotor makes first contact
      while (read_left_flop() == 0) {}
      // Turn until the rotor makes second contact and stop
      turn_exactly(-10, false);
      reset_flops();
      while (read_left_flop() == 0) {}
      // Stop the motor
      motor.drive(0);
      break;
  }
  current_position = 3;
}

// TODO: Delete this I think
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
