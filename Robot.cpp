// Defines the class Robot which holds several parameters for the robot such
// as position, basket composition, e.t.c. , and defines high-level methods
// to provide an interface for executing general tasks such as moving from
// A to B, or dropping eggs off.
//
// Also contains helper class Eggs and struct Egg

#include <string>
#include <vector>
#include "Robot.h"
#include "robot_initialise.h"

Eggs::Eggs() {}

void Eggs::add_egg(Egg egg) {

}

void Eggs::clear() {
  // Clears all the eggs, i.e. no eggs in the eggs vector after this
  // function is called.
}


// The Robot class methods
Robot::Robot() :
components(PCB1_ADDRESS, PCB2_ADDRESS),
line_following(components.line_sensors),
current_junction("s"),
direction(0) {
  initialise_robot();
}

Robot::Robot(string starting_junction, int starting_direction) :
components(PCB1_ADDRESS, PCB2_ADDRESS),
line_following(components.line_sensors),
current_junction(starting_junction),
direction(starting_direction) {
  initialise_robot();
}

void Robot::input_restart_parameters(int baskets_delivered, string delivery_zone) {
  // Allows for key parameters to be reentered post-restart.
}

void Robot::move(string a, string b) {
  switch (a) {
    case "s":
      switch (b) {
        case "i":
          // Assume in the right direciton TODO: remove that dependency
          line_following.follow_line(1.0, 0.5, 3, 1);
          line_following.align_with_intersection(1.0, 0.5);

        case "j":
          move("s", "i");
          move("i", "j");

      }
  case "i":
    switch (b) {
      case "j":
        int desired_direction = 270;
        int turn_by = desired_direction - direction;
        line_following.turn(turn_by, 1.0);
        line_following.follow_line(1.0, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);
    }
  }
}

void Robot::align_for_pickup() {

}

void Robot::pick_up_eggs(int num_to_recycle) {

}

void Robot::pick_up_all_eggs() {

}
void Robot::deliver_basket() {

}

void Robot::recycle_eggs() {

}

// Private Methods:

int Robot::sort_egg(bool large_egg) {
  // Returns 1 for egg sorted into delivery reservoir and 0 for egg sorted into recycling.
}

Egg Robot::classify_egg(bool large_egg) {
  // Use the IR sensor and diode to classify the egg
}

void Robot::update_onboard_eggs(Egg egg, int reservoir) {
  // Updates the delivery_eggs and recycling_eggs parameters of the object.
  // Updates the LEDs displaying the composition.
}
