// Defines the class Robot which holds several parameters for the robot such
// as position, basket composition, e.t.c. , and defines high-level methods
// to provide an interface for executing general tasks such as moving from
// A to B, or dropping eggs off.
//
// Also contains helper class Eggs and struct Egg

#include <string>
#include <vector>
#include <stdexcept>
#include "robot.h"
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

Robot::Robot(int string starting_junction, int starting_direction) :
components(PCB1_ADDRESS, PCB2_ADDRESS),
line_following(components),
current_junction(starting_junction),
direction(starting_direction) {
  initialise_robot();
}

void Robot::input_restart_parameters(int baskets_delivered, string delivery_zone) {
  // Allows for key parameters to be reentered post-restart.
}

void Robot::turn(int degrees, float speed) {
  line_following.turn(degrees, speed);
  direction = (direction + degrees) % 360;
  return;
}

void Robot::move(string destination) {
  switch (current_junction) {
    case "s":
      switch (destination) {
        case "i":
          int desired_direction = 0;
          int turn_by = (desired_direction - direction) % 360;
          // Make sure the robot is facing the right direction
          turn(turn_by, 1.0);
          line_following.follow_line(1.0, 0.5, 3, 1);
          line_following.align_with_intersection(1.0, 0.5);
          break;
        case "j":
          move("i");
          move("j");
          break;
      }
  case "i":
    switch (b) {
      case "j":
        int desired_direction = 270;
        int turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, 1.0);
        line_following.follow_line(1.0, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);
        break;
    }
  case "j":
    switch (b) {
      case "l":
        int desired_direction = 270;
        int turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, 1.0);

        line_following.follow_line(1.0, 0.7, 0, 1); // Increased speed_delta
        line_following.align_with_intersection(1.0, 0.5);

        direction = 180; // Direction changed due to curved path.

        turn(90, 1.0);
        line_following.follow_line(1.0, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);
        break;
      case "d2":
        break;
      case "k":
        // TODO: Potentially different route
        break;
      case "d1":
        break;
      case "d2":
        int desired_direction = 270;
        int turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, 1.0);

        line_following.follow_line(1.0, 0.7, 0, 1); // Increased speed_delta
        line_following.align_with_intersection(1.0, 0.5);

        direction = 180; // Direction changed due to curved path.

        turn(90, 1.0);
        line_following.follow_line(1.0, 0.5, 0, 1);
        // Do not stop. TODO: put in align with delivery.
        break;
      case "d3":
        break;
    }
  case "c2":
    switch (b) {
      case "j":
        // Only one direction possible at "j"
        line_following.follow_line(1.0, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);
        break;
      case "l":
        move("j");
        move("l");
        break;
      case "k":
        break;
      case "d1":
        break;
      case "d2":
        break;
      case "d3":
        break;
    }
  }
  // Update current_junction
  current_junction = destination;
  #ifdef DEBUG
  std::cout << "Current position is: " << current_junction << std::endl;
  #endif
}

void Robot::align_for_pickup() {
  if (current_junction != "j") {
    std::cout << "Robot is at the wrong junction for alignment: junction "
    << current_junction << std::endl;
    throw std::invalid_argument( "Wrong junction for alignment." );
  }

  // Turn to be facing the pickup with the rear of the vehicle
  int desired_direction = 180;
  int turn_by = desired_direction - direction;
  line_following.turn(turn_by, 1.0);

  line_following.reverse_until_switch(1.0, 0.5);
  current_junction = "c2";
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
