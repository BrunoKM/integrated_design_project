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


class Robot {
private:
  Eggs recycling_eggs;
  Eggs delivery_eggs;
  int baskets_delivered;
  std::string delivery_zone;

  Components components;
  Line_Following line_following;

  float speed;
  float turn_speed;

  void scoop();
  int sort_egg(bool large_egg);
  Egg classify_egg(bool large_egg);
  void update_onboard_eggs(Egg egg, int reservoir);
  void invoke_move(char destination);
public:
  Robot();
  Robot(char starting_junction, int starting_direction);

  std::string position;
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
  void deliver_basket();
  void recycle_eggs();
};

#endif
