// Defines the class Robot which holds several parameters for the robot such
// as position, basket composition, e.t.c. , and defines high-level methods
// to provide an interface for executing general tasks such as moving from
// A to B, or dropping eggs off.
//
// Also contains helper class Eggs and struct Egg

#include <string>
#include <vector>
#include "robot.h"

Eggs::Eggs() {}

void Eggs::add_egg(Egg egg) {

}

void Eggs::clear() {
  // Clears all the eggs, i.e. no eggs in the eggs vector after this
  // function is called.
}


// The Robot class methods
Robot::Robot() {

}

void Robot::input_restart_parameters(int baskets_delivered, string delivery_zone) {
  // Allows for key parameters to be reentered post-restart.
}

void Robot::move(string a, string b) {

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
