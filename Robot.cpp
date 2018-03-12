// Defines the class Robot which holds several parameters for the robot such
// as position, basket composition, e.t.c. , and defines high-level methods
// to provide an interface for executing general tasks such as moving from
// A to B, or dropping eggs off.
//
// Also contains helper class Eggs and struct Egg

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <delay.h>
#include "Robot.h"
#include "robot_initialise.h"

#define DEBUG
#define DEBUG2

// The Robot class methods
Robot::Robot() :
components(PCB1_ADDRESS, PCB2_ADDRESS),
line_following(components),
speed(1),
turn_speed(0.93),
current_junction('s'),
direction(0) {
  initialise_robot();
}

Robot::Robot(char starting_junction, int starting_direction) :
components(PCB1_ADDRESS, PCB2_ADDRESS),
line_following(components),
speed(1),
turn_speed(0.93),
current_junction(starting_junction),
direction(starting_direction) {
  initialise_robot();
}

void Robot::input_restart_parameters(int baskets_delivered, std::string delivery_zone) {
  // Allows for key parameters to be reentered post-restart.
}

void Robot::turn(int degrees, float speed) {
  line_following.turn(degrees, speed);
  direction = (direction + degrees) % 360;
  return;
}

void Robot::turn_rear_align(int degrees, float speed) {
  line_following.turn_rear_align(degrees, speed);
  direction = (direction + degrees) % 360;
  return;
}

void Robot::invoke_move(char destination) {
  // Actual implementation of move between nearby junctions
  std::cout << "  >   Moving from " << current_junction << " to " << destination << std::endl;
  int turn_by;
  int desired_direction;

  switch (current_junction) {
    case 's':
      switch (destination) {
        case 'i':
          desired_direction = 0;
          turn_by = (desired_direction - direction) % 360;
          // Make sure the robot is facing the right direction
          turn(turn_by, turn_speed);
          line_following.follow_line(speed, 0.5, 3, 1);
          line_following.align_with_intersection(1.0, 0.5);
          break;
      }
      break;
  case 'i':
    switch (destination) {
      case 'j':
        desired_direction = 270;
        turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, turn_speed);
        line_following.follow_line(speed, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);
        break;
    }
    break;
  case 'j':
    switch (destination) {
      case 'l':
        desired_direction = 270;
        turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, turn_speed);

        line_following.follow_line_blind_curve(speed); // Increased speed_delta
        line_following.align_with_intersection(1.0, 0.5);

        direction = 180; // Direction changed due to curved path.

        turn(90, turn_speed);
        line_following.follow_line(speed, 0.5, 0, 1); // TODO: Replace name speed with line_speed
        line_following.align_with_intersection(1.0, 0.5);
        break;
      case 'e':
        desired_direction = 270;
        turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, turn_speed);

        line_following.follow_line(speed, 0.7, 0, 1); // Increased speed_delta
        line_following.align_with_intersection(1.0, 0.5);

        direction = 180; // Direction changed due to curved path.

        turn(90, turn_speed);
        line_following.follow_line(speed, 0.5, 0, 1);
        // Do not stop. TODO: put in align with delivery.
        break;
      case 'i':
		    desired_direction = 90;
        turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, turn_speed);
        line_following.follow_line(speed, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);
        break;
    }
    break;
  case 'c':
    switch (destination) {
      case 'j':
        // Only one direction possible at "c"
        line_following.follow_line_timed(1.0, 0.5, 900);
        delay(200); // TODO: Reduce when not debugging
        break;
    }
    break;
  case 'l':
    switch (destination) {
      case 'k':
        desired_direction = 0;
        turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, turn_speed);

        line_following.follow_line(speed, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);
        break;
    }
    break;
  }
  // Update current_junction
  current_junction = destination;
}

void Robot::move(char destination) {
  // Path to take to arrive at destination.
  std::cout << "<.> Moving from " << current_junction << " to " << destination << std::endl;

  switch (current_junction) {
    case 's':
      switch (destination) {
        case 'i':
          invoke_move('i');
          break;
        case 'j':
          invoke_move('i');
          invoke_move('j');
          break;
      }
      break;
  case 'i':
    switch (destination) {
      case 'j':
        invoke_move('j');
        break;
    }
    break;
  case 'j':
    switch (destination) {
      case 'l':
        invoke_move('l');
        break;
      case 'd':
        break;
      case 'e':
        invoke_move('e');
        break;
      case 'f':
        break;
      case 'i':
		    invoke_move('i');
      case 'k':
      //  invoke_move('k');
      break;
    }
    break;
  case 'c':
    switch (destination) {
      case 'j':
        invoke_move('j');
        break;
      case 'l':
        invoke_move('j');
        invoke_move('l');
        break;
      case 'k':
        invoke_move('j');
        invoke_move('l');
        invoke_move('k');
        break;
      case 'd':
        break;
      case 'e':
        invoke_move('j');
        invoke_move('l');
        break;
      case 'f':
        break;
      case 'i':
    		invoke_move('j');
    		invoke_move('i');
		break;
    }
    break;
  }
  // Update current_junction
  current_junction = destination;
  #ifdef DEBUG
  std::cout << "Current position is: " << current_junction << std::endl;
  #endif
  return;
}

void Robot::align_for_pickup() {
std::cout << "<.> Aligning for pickup." << std::endl;
  if (current_junction != 'j') {
    std::cout << "Robot is at the wrong junction for alignment: junction "
    << current_junction << std::endl;
    throw std::invalid_argument( "Wrong junction for alignment." );
  }

  // Turn to be facing the pickup with the rear of the vehicle
  int desired_direction = 180;
  int turn_by = desired_direction - direction;

  #ifdef DEBUG2
  std::cout << "Turning by " << turn_by << " to align to direction "
  << desired_direction << " from the current direction " << direction << std::endl;
  #endif
  // Turn and align the front
  turn(turn_by, turn_speed);
  // Go forward a bit to align the rear
  int time_to_align = 1500;
  line_following.follow_line_timed(speed, 0.5, time_to_align);
  line_following.stop_motors();

  line_following.reverse_until_switch(0.5, 1.0); // Low speed and very high speed_delta
  current_junction = 'c';
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
  return 1;
}

//Egg Robot::classify_egg(bool large_egg) {
  // Use the IR sensor and diode to classify the egg
//  return;
//}

void Robot::update_onboard_eggs(Egg egg, int reservoir) {
  // Updates the delivery_eggs and recycling_eggs parameters of the object.
  // Updates the LEDs displaying the composition.
}
