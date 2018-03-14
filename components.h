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
#include "Motor.h"

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

  // Scoop actuator control (write only)
  static const int scoop_actuator_bit = 1 << 4;

  // Instantaneous microswitches; do not have to be reset in software (read only).
};


class PCB2 : public PCB {
private:
  void initialise_write_default();

public:
  PCB2(int port);
  // Wrapper for command_write_default:
  void read_initialise();

  // Persistent flip-flops; stay ON after contact triggered, have to be reset in software.
  // Flips flops read bits (READ only)
  static const int num_contact_flops = 2;
  static const int contact_flops_bits = (1 << 0) + (1 << 1); // Bits for the contact flops
  // Flip flops reset (WRITE only)
  static const int contact_flops_reset_bits = (1 << 2) + (1 << 3);

  // Instantaneous microswitches; do not have to be reset in software (READ only).
  static const int num_inst_microswitches = 2;
  static const int inst_microswitch_bits = (1 << 4) + (1 << 5);

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
};

class Turntable_Comms {
private:
  int address;
  command_instruction write_instruction;
  request_instruction read_instruction;
public:
  Turntable_Comms(int address);
  void write(int write_byte);
  void set_angle(int degrees);
};

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
  Line_Sensors(PCB1 &pcb) : pcb(pcb){};
  Line_Sensor_Reading get_sensor_reading();
};

class Microswitches {
private:
  PCB2 pcb;
  // Define the order of microswitch bits:
  static const int front_switch_bit = 1 << 5;
  static const int rear_switch_bit = 1 << 4;

public:
  bool rear_state;
  bool front_state;

  Microswitches(PCB2 &pcb) : pcb(pcb){};
  void update_state();
};

struct Egg {
  int size;
  char colour;
  Egg(int size, char colour) : size(size), colour(colour) {};
  Egg() {};
  bool operator==(const Egg& egg2){
    if ((size == egg2.size) and (colour == egg2.colour)) {
      return true;
    }
    return false;
  };
};

class Eggs {
// Class used for storing the information about eggs
public:
  Eggs();
  std::vector<Egg> eggs;
  void add_egg(Egg egg);
  bool check_if_in(Egg egg);
  void clear();
};


// TODO: Finish class LEDs
class LEDs {
  PCB2 pcb;
  static const int led1_bit = 1 << 6;
	static const int led2_bit = 1 << 7;
public:
    LEDs(PCB2 &pcb) : pcb(pcb){};
    void off();
    void display_egg(Egg egg);
    void write_leds(int led1_val, int led2_val);
};

class Scoop {
private: //TODO: What pcb and what bit?
  PCB1 pcb;
  static const int scoop_bit = 1 << 4;
public:
  Scoop(PCB1 &pcb) : pcb(pcb){};
  void contract();
  void release();
};

class Beacon_Reader : public ADC {
    // Class for communication with both the start beacon
private:
  static const int reading_threshold = 120;
public:
	Beacon_Reader(int port): ADC(port) {};
    // Public Methods
    int get_beacon_code(); // Returns delivery point which is based on beacon code
};

// class Rotating_Compartment {
// private:
//   // TODO: PCBx pcb; // for the microswitches
//   static const int switch_bit; //TODO: or bits
//   Motor motor;
// public:
//   Rotating_Compartment(PCBx pcb);
//
// }


class Colour_Detector {
public:
  ADC red_sensor;
  ADC blue_sensor;

  // Constructor
  Colour_Detector(int colour_sensor1_port, int colour_sensor2_port) :
  red_sensor(colour_sensor1_port),
  blue_sensor(colour_sensor2_port) {};

  int read_red_sensor();
  int read_blue_sensor();

  Egg classify_egg(int size);
};


// A master class to rule them all
class Components {
private:
  // TODO: Add the rest of the necessary classes
  PCB1 pcb1;
  PCB2 pcb2;
public:
  Turntable_Comms turntable_comms;
  Beacon_Reader beacon_reader;
  Line_Sensors line_sensors;
  Microswitches microswitches;
  Scoop scoop; // TODO: Add the right PCB in the constructor

  Components(int pcb1_port, int pcb2_port, int turntable_comms_port,
    int input_ir_port, int colour_sensor_1_port, int colour_sensor_2_port):
  pcb1(PCB1(pcb1_port)),
  pcb2(PCB2(pcb2_port)),
  turntable_comms(Turntable_Comms(turntable_comms_port)),
  beacon_reader(input_ir_port),
  line_sensors(pcb1),
  microswitches(pcb2),
  scoop(pcb1)
  {};

};



// TODO: Finish classes below.
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
