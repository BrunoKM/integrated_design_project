#include <vector>
#include <iostream>
#include <stdlib.h>
#include <delay.h>
#include <stopwatch.h>
#include "components.h"





PCB::PCB(int port) :
port(port), read_state(0), write_state(~0) {
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
  // Send write state to initialise the read inputs
  write();
}

void PCB::read() {
  // Update the read state
  read_state = rlink.request(read_instruction);
}

void PCB::write() {
  // Write the write state to the PCB
  rlink.command(write_instruction, write_state);
}

bool PCB::get(int bit) {
  // Return the value of the requested bit from the read state
	int mask = 1 << bit;
	return (read_state & mask) == mask;
}

void PCB::set(int bit, bool value) {
  // Alter the value of bit on the write state
	if (value) {
		write_state |= 1 << bit;
	} else {
		write_state &= ~(1 << bit);
	}
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
  // TODO: Put some conversion code in here.
  int write_byte = degrees;

  write(write_byte);
};

void Turntable_Comms::set_position(int position) {
  // Position 1 - 8 for eggs. Odd for small, even for large ones.
  int write_byte;
  int divide_by = 8;
  write_byte = ((220.0 * position) / divide_by) + 15;
  for (int i; i < 4; i++) {
		write(write_byte);
		delay(400);
	}
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
  // Update the read state with current values
  pcb.read();

  Line_Sensor_Reading reading;
  reading.front_left = pcb.get(front_left_bit);
  reading.front_right = pcb.get(front_right_bit);
  reading.back_left = pcb.get(back_left_bit);
  reading.back_right = pcb.get(back_right_bit);
  return reading;
}


void Microswitches::update_state() {
  // Update the read state with current values
  pcb.read();

  front_state = pcb.get(front_switch_bit);
  rear_state = pcb.get(rear_switch_bit);
}



void LEDs::off() {
  // Set all the LEDs to 0
  write_leds(false, false);
}

void LEDs::write_leds(bool led1_val, bool led2_val) {
  // Change the write_state to the desired values
  pcb.set(led1_bit, led1_val);
  pcb.set(led2_bit, led2_val);
  // Send the write command
  pcb.write();
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
  pcb.set(scoop_bit, 1);
  // Send the write command
  pcb.write();
}

void Scoop::release() {
  pcb.set(scoop_bit, 0);
  // Send the write command
  pcb.write();
};

void Scoop::violent_shock() {
  int impulse_delay = 40;
  int num_shocks = 15;

  for (int i=1; i <= num_shocks; i++) {
    contract();
    delay(impulse_delay);
    release();
    if (i != num_shocks) {
      // Do not delay on last iter.
      delay(impulse_delay);
    }
  }
  return;
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
  int red_reading = read_red_sensor();
  int blue_reading = read_blue_sensor();

  char colour;
  switch (size) {
    case 0:
      // Decide between blue and yellow
      if ((blue_reading < 200) or (red_reading < 200)) {
		  colour = 'b';
	  } else {
		  colour = 'y';
	  }
      break;
    case 1:
      // Decide between yellow and pink
      if ((blue_reading < 200) or (red_reading < 200)) {
		  colour = 'p';
	  } else {
		  colour = 'y';
	  }
      break;
  }
  return Egg(size, colour);
}

Rotating_Compartment::Rotating_Compartment(PCB pcb) :
pcb(pcb),
motor(Motor(3, 1.0, 1.0)),
current_position(3) {
  reset_flops();
}

void Rotating_Compartment::turn_exactly(int degrees, bool stop_after) {
  float rotate_time_360 = 2900; // TODO: Recalibrate

  int rotate_for = rotate_time_360 * float(abs(degrees)) / (speed * 360.0);
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
    std::cout << "Motor stopped" << std::endl;
  }
  return;
}

bool Rotating_Compartment::read_left_flop() {
  pcb.read();
  return pcb.get(left_flop_bit);
}
bool Rotating_Compartment::read_right_flop() {
  pcb.read();
  return pcb.get(right_flop_bit);
}
void Rotating_Compartment::reset_flops() {
	// Need to send a zero pulse to reset the flops
	pcb.set(reset_left_flop_bit, false);
	pcb.set(reset_right_flop_bit, false);

	pcb.write();

	pcb.set(reset_left_flop_bit, true);
	pcb.set(reset_right_flop_bit, true);

	pcb.write();
}

void Rotating_Compartment::turn_to_position(int position) {
  // Assumes starting at position 3 (the centre), and turns to one of the desired positions
  switch (position) {
    case 1:
      turn_exactly(-100, true);
      break;
    case 2:
      turn_exactly(-60, true);
      break;
    case 4:
      turn_exactly(60, true);
      break;
    case 5:
      turn_exactly(100, true);
      break;
    default:
      std::cout << "Wrong input " << position << " to function turn_to_position" << std::endl;
  }
  current_position = position;
}

void Rotating_Compartment::return_to_default() {
  switch (current_position) {
    case 1:
      turn_exactly(100, true);
      break;
    case 2:
      turn_exactly(60, true);
      break;
    case 4:
      turn_exactly(-60, true);
      break;
    case 5:
      turn_exactly(-100, true);
      break;
  }
  //reset_flops();
  // Turn until the rotor makes first contact
  //while (read_right_flop() == 0) {}
  //motor.drive(0);
  current_position = 3;
}
