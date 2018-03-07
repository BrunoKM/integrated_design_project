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

struct Egg {
  const int size;
  const char colour;
};

class Eggs {
public:
  std::vector<Egg> eggs;
  void add_egg(Egg egg);
  void clear();
};

class Robot {
private:
  Eggs recycling_eggs;
  Eggs delivery_eggs;
  int baskets_delivered;
  string delivery_zone;

  Components components;
  Line_Following line_following;

  void scoop();
  int sort_egg(bool large_egg);
  Egg classify_egg(bool large_egg);
  void update_onboard_eggs(Egg egg, int reservoir);
public:
  Robot();
  Robot(string starting_junction, int starting_direction);

  string position;
  string current_junction; // Only update when on intersections.
  int direction; // Either 0, 90, 180 or 270

  // Methods
  void input_restart_parameters(int baskets_delivered, string delivery_zone);
  void move(string b);
  void turn(int degrees, float speed); // Wrapper for line_following.turn() that also updates this->direction
  void align_for_pickup();
  void pick_up_eggs(int num_to_recycle);
  void pick_up_all_eggs();
  void deliver_basket();
  void recycle_eggs();
};

#endif
