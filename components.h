// Defines several classes for each major component of the Robot which offer
// methods that hide the implementation of the tasks each of the components
// is expected to execute.

// Also defines all the pin allocations for the PCB.
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <vector>
#include <robot_link.h>
#include <robot_instr.h>
#include "robot_initialise.h"

class PCB {
protected:
  int port;
  request_instruction read_instruction;
  command_instruction write_instruction;
  int write_default; // used for and'ing with the values to be sent to write'able bits.
  void initialise_write_default();
  void command_write_default();
public:
  PCB(int port);
  int read_state();
  void write(int byte);
};


class PCB1 : public PCB {
private:
  void initialise_write_default();
public:
  PCB1(int &port);
  void read_initialise();

  // Line following sensors (read only)
  static const int num_line_sensors = 4;
  static const int line_sensor_bits = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3);

  // Instantaneous microswitches; do not have to be reset in software (read only).
  static const int num_inst_microswitches = 2;
  static const int inst_microswitch_bits = (1 << 4) + (1 << 5);
  //  IR LED for beacon communication (write only)
  static const int ir_led_bit = 1 << 6;
  //  IR Phototransistor for beacon communication (read only)
  static const int ir_input_bit = 1 << 7;

};


class PCB2 : public PCB {
private:
  void initialise_write_default();

public:
  PCB2(int port);
  // Wrapper for command_write_default:
  void read_initialise();

  // Persistent microswitches; stay ON after microswitch triggered, have to be reset by software (read and write).
  static const int num_persistent_microswitches = 2;
  static const int persistent_microswitch_bits = (1 << 0) + (1 << 1);
  // Instantaneous microswitches; do not have to be reset in software (read only).
  static const int num_inst_microswitches = 3;
  static const int inst_microswitch_bits = (1 << 2) + (1 << 3) + (1 << 4);
  // Scoop actuator control (write only)
  static const int scoop_actuator_bit = 1 << 5;
  // LEDs to signal colour detection (write only)
  static const int led1_bit = (1 << 6);
  static const int led2_bit = (1 << 7);
};


class ADC {
protected:
  int port;
  request_instruction read_instruction;
public:
  ADC(int port);
  int read_state();
}




struct Line_Sensor_Reading {
  bool front_left;
  bool front_right;
  bool back_left;
  bool back_right;
};

class Line_Sensors {
private:
  PCB1 pcb;
  // Define the order of line sensors connections:
  static const int front_left_bit = 1 << 0;
  static const int front_right_bit = 1 << 1;
  static const int back_left_bit = 1 << 2;
  static const int back_right_bit = 1 << 3;
public:
  Line_Sensors(PCB1 &pcb1) : pcb(pcb1){};
  Line_Sensor_Reading get_sensor_reading();
};

class Microswitches {
private:
  PCB1 pcb;
  // Define the order of microswitch bits:
  static const int front_switch_bit = 1 << 5;
  static const int rear_switch_bit = 1 << 4;

public:
  bool rear_state;
  bool front_state;

  Microswitches(PCB1 &pcb1) : pcb(pcb1){};
  void update_state();
};

struct Egg {
  const int size;
  const char colour;
};

class Eggs {
public:
  Eggs();
  std::vector<Egg> eggs;
  void add_egg(Egg egg);
  void clear();
};


// TODO: Finish class LEDs
class LEDs {
private:
  PCB2 pcb;
  static const int led1_bit = 1 << 4;
	static const int led2_bit = 1 << 5;
public:
    LEDs(PCB2 &pcb2) : pcb(pcb2){};
    void off();
    void display_egg(Egg egg);
    void write_leds(int led1_val, int led2_val);
};

class Beacon_Reader : ADC {
    // Class for communication with both the start beacon
private:
  static const int reading_threshold = 100;
public:
    // Public Methods
    int get_beacon_code(); // Returns delivery point which is based on beacon code
};



// A master class to rule them all
class Components {
private:
  // TODO: Add the rest of the necessary classes
  PCB1 pcb1;
  PCB2 pcb2;
public:
  Line_Sensors line_sensors;
  Microswitches microswitches;
  Beacon_Reader beacon_reader;

  Components(int pcb1_port, int pcb2_port, int input_ir_port):
  pcb1(PCB1(pcb1_port)),
  pcb2(PCB2(pcb2_port)),
  line_sensors(pcb1),
  microswitches(pcb1),
  beacon_reader(input_ir_port){};

};

// TODO: Finish classes below.
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
