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
  int read_state();
public:
  PCB(int port);
};


class PCB1 : public PCB {
private:
  // Line following sensors (read only)
  static const int num_line_sensors = 4;
  static const int line_sensor_bits = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3);
  //  IR LED for beacon communication (write only)
  static const int ir_led_bit = 1 << 4;
  //  IR Phototransistor for beacon communication (read only)
  static const int ir_input_bit = 1 << 5;

  void initialise_write_default();
public:
  PCB1(int &port);
  void read_initialise();
  int read_line_sensors(); // The int value (binary) of the line sensors reading
  int read_ir_input();
};

// TODO: Change to the same format as PCB1
class PCB2 : public PCB {
private:
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

  void initialise_write_default();

public:
  PCB2(int port);
  void write_leds(int led1_val, int led2_val); //TODO: May potentially have to apply a timer to make sure leds stay on for long enough.
  // Wrappers for command_write_default:
  void read_initialise();
  void reset_microswitches();

  void write_scoop(int scoop_val);
  int read_microswitches();

};


struct Line_Sensor_Reading {
  bool front_left;
  bool front_right;
  bool back_left;
  bool back_right;
};

class Line_Sensors {
private:
  PCB1 pcb1;
  // Define the order of line sensors connections:
  static const int front_left_bit = 1 << 0;
  static const int front_right_bit = 1 << 1;
  static const int back_left_bit = 1 << 2;
  static const int back_right_bit = 1 << 3;
public:
  Line_Sensors(PCB1 &pcb1) : pcb1(pcb1){};
  Line_Sensor_Reading get_sensor_reading();
};

class Microswitches {
private:
  PCB2 pcb2;
  // Define the order of microswitch bits:
  static const int front_switch_bit = 1 << 2;
  static const int rear_switch_bit = 1 << 3;

public:
  bool rear_state;
  bool front_state;
  
  Microswitches(PCB2 &pcb2) : pcb2(pcb2){};
  void update_state();
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

  Components(int pcb1_port, int pcb2_port): pcb1(PCB1(pcb1_port)), pcb2(PCB2(pcb2_port)), // TODO: Get rid of PCB1 explicit constructor
    line_sensors(pcb1), microswitches(pcb2){};

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
    PCB2 pcb2;
    const int leds_port;
    const int led1_pin;
    const int led2_pin;
public:
    LEDs(int leds_port, int led1_pin, int led2_pin);
    void off();
    void display_egg(Egg egg);
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

// TODO: FINISH IR_communications class when we understand whats going on...
/*class IR_communication {
    // Class for communication with both the start beacon and the turn table sensor
private:
    PCB1 pcb1;
    static const int ir_led_bit = 1 << 4; //  IR LED for beacon communication (write only)
    static const int ir_input_bit = 1 << 5;//  IR Phototransistor for beacon communication (read only)
public:
    IR_communication(PCB1 &pcb1) : pcb1(pcb1){};
    // Public Methods
    int get_delivery_point(); // Returns delivery point which is based on beacon code
    void rotate_turntable_start_position(); // Rotates turntable to the starting position (e.g. fully clockwise)
    void rotate_turntable(int degrees); // Rotates turntable the correct number of degrees
};
*/
#endif
