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
components(PCB1_ADDRESS, PCB2_ADDRESS, INPUT_IR_PORT, COLOUR_SENSOR_1_PORT, COLOUR_SENSOR_2_PORT, TURNTABLE_COMMS_ADDRESS),
line_following(components),
speed(1),
turn_speed(0.93),
current_junction('s'),
direction(0) {
  initialise_robot();
}

Robot::Robot(char starting_junction, int starting_direction) :
components(PCB1_ADDRESS, PCB2_ADDRESS, INPUT_IR_PORT, COLOUR_SENSOR_1_PORT, COLOUR_SENSOR_2_PORT, TURNTABLE_COMMS_ADDRESS),
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
  case 'k':
    switch (destination) {
      case 'j':
        desired_direction = 90;
        turn_by = (desired_direction - direction) % 360;
        // Make sure the robot is facing the right direction
        turn(turn_by, turn_speed);

        // Follow line for a short period of time until the line (roughly) ends
        int time_to_line_end = 2500;
        line_following.follow_line_timed(speed, 0.5, time_to_line_end);

        // Go blindly as to ignore the curved line (should go to the side of it anyways, just for safety)
        line_following.motors_go(speed, speed);
        int time_to_pass_curved = 4000;
        delay(time_to_pass_curved);

        // Start line-following, or in this case looking for an intersections
        line_following.follow_line(speed, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);

        // Turn left towards j
        turn(-90, turn_speed);
        line_following.follow_line(speed, 0.5, 0, 1);
        line_following.align_with_intersection(1.0, 0.5);

        break;
    }
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
          invoke_move('l');
          invoke_move('k');
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
    case 'k':
      switch (destination) {
        case 'j':
          invoke_move('j');
          break;
        }
        break;
    case 'l':
      switch (destination) {
        case 'j':
          invoke_move('k');
          invoke_move('j');
          break;
      }
      break;
    default : //Optional
      throw std::invalid_argument("Invalid current location");
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

  // Contract the actuator before approaching the turntable.
  components.scoop.contract();

  line_following.reverse_until_switch(0.5, 1.0); // Low speed and very high speed_delta
  current_junction = 'c';
}

void Robot::pick_up_all_eggs() {
  components.scoop.release();

  int delay_time = 800; // Time to wait for the table to turn.
  int is_large;

  for (int position=1; position <= 8; position++) {
    components.turntable_comms.set_position(position);
    delay(delay_time);
    // Odd eggs are small, even are large
    is_large = (position + 1) % 2;
    sort_egg(is_large);
  }
  components.turntable_comms.set_position(0); // Return the table to default
  delay(400); // Make sure that the turntable position command was sent.

  return;
}
// void Robot::set_up_turntable() {
//
// }

void Robot::deliver_basket() {
  // Assumes at the correct delivery zone with front switch pressed in.
  // Turn by a set number of degrees and release the eggs.
  int degrees = 10; // TODO: Change to match the actual setup.
  switch (baskets_delivered) {
    case 0:
      line_following.turn_exactly(-3 * degrees);
      // Turn the compartment to delivery position.
      components.compartment.turn_to_position(1);
      // Shake the eggs out
      components.scoop.violent_shock();
      components.compartment.return_to_default();
      // Turn back to the position parallel with line
      line_following.turn_exactly(3 * degrees);
      break;
    case 1:
      line_following.turn_exactly(-1 * degrees);
      // Turn the compartment to delivery position.
      components.compartment.turn_to_position(1);
      // Shake the eggs out
      components.scoop.violent_shock();
      components.compartment.return_to_default();
      // Turn back to the position parallel with line
      line_following.turn_exactly(1 * degrees);
      break;
    case 2:
      line_following.turn_exactly(1 * degrees);
      // Turn the compartment to delivery position.
      components.compartment.turn_to_position(1);
      // Shake the eggs out
      components.scoop.violent_shock();
      components.compartment.return_to_default();
      // Turn back to the position parallel with line
      line_following.turn_exactly(-1 * degrees);
      break;
    case 3:
      line_following.turn_exactly(3 * degrees);
      // Turn the compartment to delivery position.
      components.compartment.turn_to_position(1);
      // Shake the eggs out
      components.scoop.violent_shock();
      components.compartment.return_to_default();
      // Turn back to the position parallel with line
      line_following.turn_exactly(-3 * degrees);
      break;
  }
  return;
}

void Robot::recycle_eggs() {

}

void Robot::read_beacon() {
  // Read the instructions from the starting beacon.
  int pulse_count = components.beacon_reader.get_beacon_code();
  switch (pulse_count) {
    case 1:
      basket_egg1 = Egg(0, 'b');
      basket_egg2 = Egg(1, 'y');
      delivery_zone = 'd'; // i.e. d1
      break;
    case 3:
      basket_egg1 = Egg(0, 'b');
      basket_egg2 = Egg(1, 'y');
      delivery_zone = 'e'; // i.e. d2
      break;
    case 4:
      basket_egg1 = Egg(0, 'y');
      basket_egg2 = Egg(1, 'p');
      delivery_zone = 'd'; // i.e. d1
      break;
    case 6:
      basket_egg1 = Egg(0, 'y');
      basket_egg2 = Egg(1, 'p');
      delivery_zone = 'e'; // i.e. d2
      break;
    default:
      std::cout << "Wrong beacone code " << pulse_count << ". Reading again.";
      read_beacon();
      return;
  }
  std::cout << "Beacon code " << pulse_count << " received." << std::endl;
  // TODO: Save this info to file
  return;
}

// Private Methods:

void Robot::sort_egg(bool is_large) {
  // Pull the egg in, classify the colour, and put it in the right compartment.
  components.scoop.contract();
  //TODO: See if delay needed
  components.scoop.release();

  // TODO: Maybe a shake here?
  Egg egg = components.colour_detector.classify_egg(is_large);
  if ((eggs1_onboard < 2)) and (basket_egg1 == egg)) {
    // Put into the delivery compartment
    put_into_delivery();
    eggs1_onboard++;
  }
  else if ((eggs2_onboard < 2)) and (basket_egg2 == egg)) {
    // Put into the delivery compartment
    put_into_delivery();
    eggs2_onboard++;
  } else {
    put_into_recycling();
    recycling_eggs_onboard++;
  }
  return;
}

void Robot::put_into_delivery() {
  // Turn to the delivery position
  components.compartment.turn_to_position(4);
  // Shake the egg down violently
  components.scoop.violent_shock();
  components.compartment.return_to_default();
  return;
}

void Robot::put_into_recycling() {
  // Turn to the delivery position
  components.compartment.turn_to_position(2);
  // Shake the egg down violently
  components.scoop.violent_shock();
  components.compartment.return_to_default();
  return;
}


void Robot::update_onboard_eggs(Egg egg, int reservoir) {
  // Updates the delivery_eggs and recycling_eggs parameters of the object.
  // Updates the LEDs displaying the composition.
}
