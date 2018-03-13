// Declares the class Robot which holds several parameters for the robot such
// as position, basket composition, e.t.c. , and defines high-level methods
// to provide an interface for executing general tasks such as moving from
// A to B, or dropping eggs off.
//
// Also contains helper class Eggs and struct Egg
#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <vector>
#include "components.h"
#include "line_following.h"

const int PCB1_ADDRESS = 0;
const int PCB2_ADDRESS = 1;
const int INPUT_IR_PORT = 0;
const int COLOUR_DETECTION_1_PORT = 1;
const int COLOUR_DETECTION_2_PORT = 2;


class Robot {
private:
  // Eggs recycling_eggs;
  // Eggs delivery_eggs; // Eggs stored in the delivery conpartment
  
  Components components;
  Line_Following line_following;
  
  // The types of eggs to be delivered
  Egg basket_egg1;
  Egg basket_egg2;
  char delivery_zone;

  // Counters for contents of the containers.
  int eggs1_onboard;
  int eggs2_onboard;
  int num_recycling_eggs;

  // Number of baskets delivered.
  int baskets_delivered;



  float speed;
  float turn_speed;

  void scoop();
  int sort_egg(bool is_large);
  Egg classify_egg(bool is_large);
  void update_onboard_eggs(Egg egg, int reservoir);
  void invoke_move(char destination);
public:
  Robot();
  Robot(char starting_junction, int starting_direction);

  // std::string position;
  char current_junction; // Only update when on intersections.
  int direction; // Either 0, 90, 180 or 270

  // Methods
  void input_restart_parameters(int baskets_delivered, std::string delivery_zone);
  void move(char destination);
  void turn(int degrees, float speed); // Wrapper for line_following.turn() that also updates this->direction
  void turn_rear_align(int degrees, float speed);
  void align_for_pickup();
  void pick_up_eggs(int num_to_recycle);
  void pick_up_all_eggs();
  void read_beacon(); // Read the instructions from the starting beacon.
  void deliver_basket();
  void recycle_eggs();
};

#endif
