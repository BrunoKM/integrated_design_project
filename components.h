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

  int read_state;
  int write_state;
public:
  PCB(int port);

  void read();
  void write();

  void set(int bit, bool value);
  bool get(int bit);
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
  void set_position(int position); // Turn the turntable to n-th egg position
};

struct Line_Sensor_Reading {
  bool front_left;
  bool front_right;
  bool back_left;
  bool back_right;
};

class Line_Sensors {
private:
  PCB pcb;
  // Define the order of line sensors connections:
  static const int front_left_bit = 0;
  static const int front_right_bit = 1;
  static const int back_left_bit = 2;
  static const int back_right_bit = 3;
public:
  Line_Sensors(PCB &pcb) : pcb(pcb){};
  Line_Sensor_Reading get_sensor_reading();
};

class Microswitches {
private:
  PCB pcb;
  // Define the order of microswitch bits:
  static const int rear_switch_bit = 4;
  static const int front_switch_bit = 5;

public:
  bool rear_state;
  bool front_state;

  Microswitches(PCB &pcb) : pcb(pcb){};
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
// Class used for storing information about eggs
public:
  Eggs();
  std::vector<Egg> eggs;
  void add_egg(Egg egg);
  bool check_if_in(Egg egg);
  void clear();
};


// TODO: Finish class LEDs
class LEDs {
  PCB pcb;
  static const int led1_bit = 6;
  static const int led2_bit = 7;
public:
    LEDs(PCB &pcb) : pcb(pcb){};
    void off();
    void display_egg(Egg egg);
    void write_leds(bool led1_val, bool led2_val);
};

class Scoop {
private:
  PCB pcb;
  static const int scoop_bit = 4;
public:
  Scoop(PCB &pcb) : pcb(pcb){};
  void contract();
  void release();

  // The word violent is redundant, but it sounds soo cool
  void violent_shock();
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

class Rotating_Compartment {
private:
  PCB pcb; // for the microswitches
  static const int left_flop_bit = 1;
  static const int right_flop_bit = 0;

  static const int  reset_left_flop_bit = 3;
  static const int  reset_right_flop_bit = 2;

  static const int speed = 1.0;

  Motor motor;

  // Position is:
  // 1 - for basket delivery
  // 2 - for recycling classification
  // 3 - default (everything closed)
  // 4 - for basket classification
  // 5 - for recycling delivery
  int current_position;

public:
  Rotating_Compartment(PCB pcb);
  void turn_exactly(int degrees, bool stop_after);
  void turn_to_position(int position);
  void return_to_default();

  // Helper functions for getting info from the flops.
  bool read_left_flop();
  bool read_right_flop();
  void reset_flops();
};


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
  PCB pcb1;
  PCB pcb2;
public:
  Turntable_Comms turntable_comms;
  Beacon_Reader beacon_reader;
  Line_Sensors line_sensors;
  Microswitches microswitches;
  Scoop scoop;
  Rotating_Compartment compartment;
  Colour_Detector colour_detector;

  Components(int pcb1_port, int pcb2_port, int turntable_comms_port,
    int input_ir_port, int colour_sensor_1_port, int colour_sensor_2_port):
  pcb1(PCB(pcb1_port)),
  pcb2(PCB(pcb2_port)),
  turntable_comms(Turntable_Comms(turntable_comms_port)),
  beacon_reader(input_ir_port),
  line_sensors(pcb1),
  microswitches(pcb2),
  scoop(pcb1),
  compartment(pcb2),
  colour_detector(colour_sensor_1_port, colour_sensor_2_port)
  {};

};




#endif
