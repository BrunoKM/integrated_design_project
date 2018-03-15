
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include "line_following.h"

#define DEBUG // TODO: Comment out when running for competition!
#define DEBUG2 // Even more detailed info
#define DEBUG3 // Even more detailed info

// Parameters to tune:
const int TURN_DELAY_TIME=200; // Time in msto wait for during turning to make sure sensors are off the line
const float TURN_ALIGN_SPEED_FRAC = 0.1; // Fraction of speed to apply to align with line after turned
const int NUM_TO_CONFIRM_ALIGNMENT = 4; // Number of iterations to confirm that robot is on the line

Line_Following::Line_Following(Components &components) : current_speed(0),
current_ramp_time(-1),
left_motor(Motor(1, 1.0, 1.0)),
right_motor(Motor(2, 1.0, -1.0)),
line_sensors(components.line_sensors),
microswitches(components.microswitches) {
  set_ramp(default_ramp_time);
}

bool Line_Following::adjust_speeds_from_reading(float speed, float reduced_speed, Line_Sensor_Reading reading) {
  // Basic component for any of the line following functions
  // Return True if an intersection was detected, return False otherwise
  if ((reading.front_left == 1) and (reading.front_right == 0)) {
    // Need to go more towards left
    left_motor.drive(reduced_speed);
    right_motor.drive(speed); // Right wheel going faster
  } else if ((reading.front_left == 0) and (reading.front_right == 1)) {
    // Need to go more towards right
    left_motor.drive(speed);
    right_motor.drive(reduced_speed); // Left wheel going faster
  } else if ((reading.front_left == 0) and (reading.front_right == 0)) {
    // Make both motors go at the same speed.
    left_motor.drive(speed);
    right_motor.drive(speed);
  } else if ((reading.front_left == 1) and (reading.front_right == 1)) {
    // Make motors go at equal speeds (don't want to go
    // in a curved path over the intersection)
    left_motor.drive(speed);
    right_motor.drive(speed);
    return true;
  }
  return false;
}

bool Line_Following::adjust_reverse_speeds_from_reading(float speed, float reduced_speed, Line_Sensor_Reading reading) {
  // Adjusts speeds of the motors to stay on the line, but for following the line in reverse.
  if ((reading.back_left == 1) and (reading.back_right == 0)) {
    // Need to go more towards left
    left_motor.drive(-reduced_speed);
    right_motor.drive(-speed); // Right wheel going faster
  } else if ((reading.back_left == 0) and (reading.back_right == 1)) {
    // Need to go more towards right
    left_motor.drive(-speed);
    right_motor.drive(-reduced_speed); // Left wheel going faster
  } else if ((reading.back_left == 0) and (reading.back_right == 0)) {
    // Make both motors go at the same speed.
    left_motor.drive(-speed);
    right_motor.drive(-speed);
  } else if ((reading.back_left == 1) and (reading.back_right == 1)) {
    // Make motors go at equal speeds (don't want to go
    // in a curved path over the intersection)
    left_motor.drive(-speed);
    right_motor.drive(-speed);
    return true;
  }
  return false;
}


// TODO: Add '11' reading counter to be more certain of junctions
void Line_Following::follow_line_until_intersection(float speed, float speed_delta) {

  left_motor.drive(speed);
  right_motor.drive(speed);

  float reduced_speed = speed - speed * speed_delta;
  bool intersection_detected = false;

  Line_Sensor_Reading reading;

  #ifdef DEBUG3
    std::cout << " + Starting to line_follow (with adjustment)" << std::endl;
  #endif
  // Keep correcting until an intersection is reached
  while (intersection_detected == false) {
    reading = line_sensors.get_sensor_reading();

    #ifdef DEBUG3
    Line_Sensor_Reading last_reading;
    if ((last_reading.front_left != reading.front_left) or (last_reading.front_right != reading.front_right)) {
      // If the reading changes, print it out.
      std::cout << " + Current line sensor reading: " << reading.front_left << " "
      << reading.front_right << std::endl;
      last_reading = reading;
    }
    #endif
    /*
    // TODO : Change this
    if ((reading.front_left == 1) and (reading.front_right == 0)) {
    // Need to go more towards left
    left_motor.drive(reduced_speed);
    right_motor.drive(speed); // Right wheel going faster
  } else if ((reading.front_left == 0) and (reading.front_right == 1)) {
    // Need to go more towards right
    left_motor.drive(speed);
    right_motor.drive(reduced_speed); // Left wheel going faster
  } else if ((reading.front_left == 0) and (reading.front_right == 0)) {
    // Make both motors go at the same speed.
    left_motor.drive(speed);
    right_motor.drive(speed);
  } else if ((reading.front_left == 1) and (reading.front_right == 1)) {
    // Make motors go at equal speeds (don't want to go
    // in a curved path over the intersection)
    left_motor.drive(speed);
    right_motor.drive(speed);
    intersection_detected = true;
  }
    // TODO: Change this */
    intersection_detected = adjust_speeds_from_reading(speed, reduced_speed, reading);

  }
  std::cout << " + Intersection detected." << std::endl;
  return;
}

void Line_Following::follow_line(float speed, float speed_delta, int num_intersections_to_ignore, bool keep_driving_after_last) {
  #ifdef DEBUG2
    std::cout << " + Call to follow_line(), starting motors at equal speeds." << std::endl;
  #endif

  // Start
  left_motor.drive(speed);
  right_motor.drive(speed);

  int intersections_crossed = 0;
  while (intersections_crossed <= num_intersections_to_ignore) {
    follow_line_until_intersection(speed, speed_delta);
    intersections_crossed += 1;
    // wait until intersection crossed
    Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
    // TODO: May have to add counter to avoid error

    #ifdef DEBUG3
      std::cout << " + Going forward blindly" << std::endl;
    #endif

    // Keep driving forwards while over the junction:
    while ((reading.front_left == 1) and (reading.front_right == 1)) {
      reading = line_sensors.get_sensor_reading();
    }
  }
  if (keep_driving_after_last == 0) {

    #ifdef DEBUG3
      std::cout << " + keep_driving_after_last=0, stopping both motors." << std::endl;
    #endif

    left_motor.drive(0.0);
    right_motor.drive(0.0);
  }

  return;
}

void Line_Following::follow_line_timed(float speed, float speed_delta, int time_duration) {
  // Keep line-following for a prespecified amount of time

  left_motor.drive(speed);
  right_motor.drive(speed);

  stopwatch watch;
  watch.start();

  float reduced_speed = speed - speed * speed_delta;
  Line_Sensor_Reading reading;

  // Keep line-following until timer runs out.
  while (watch.read() < time_duration) {
    reading = line_sensors.get_sensor_reading();
    adjust_speeds_from_reading(speed, reduced_speed, reading);

  }
  // Up to the caller to even out the speeds (he may wish to stop after calling this function)
  return;
}

void Line_Following::reverse_timed(float speed, float speed_delta, int time_duration) {
  // Keep line-following for a prespecified amount of time

  left_motor.drive(-speed);
  right_motor.drive(-speed);

  stopwatch watch;
  watch.start();

  float reduced_speed = speed - speed * speed_delta;
  Line_Sensor_Reading reading;

  // Keep line-following until timer runs out.
  while (watch.read() < time_duration) {
    reading = line_sensors.get_sensor_reading();
    adjust_reverse_speeds_from_reading(speed, reduced_speed, reading);

  }
  // Up to the caller to even out the speeds (he may wish to stop after calling this function)
  return;
}

void Line_Following::align_with_intersection(float speed, float speed_delta) {
  // Keep driving for a pre-calibrated amount until wheel axis is over intersection

  left_motor.drive(speed);
  right_motor.drive(speed);

  stopwatch watch;
  watch.start();

  #ifdef DEBUG3
    std::cout << " + Aligning with intersection (line following and adjusting)." << std::endl;
  #endif

  float time_duration_constant = 1080; //TODO: Adjust for actual chassis

  int time_duration = time_duration_constant / speed;

  // Follow line for the time required to align with the junction.
  follow_line_timed(speed, speed_delta, time_duration);

  // Stop the motors
  left_motor.drive(0);
  right_motor.drive(0);
  #ifdef DEBUG3
    std::cout << " + Aligned. Motors stopped." << std::endl;
  #endif
}


void Line_Following::follow_line_blind_curve(float speed) {

  left_motor.drive(speed);
  right_motor.drive(speed);

  float speed_delta = 0.7; // A higher prespecified speed_delta for staying on the line
  float reduced_speed = speed - speed * speed_delta;
  float slow_turn_speed = 0.76 * speed; // A slower speed for the left motor so that the robot curves by itself.
  bool intersection_detected = false;

  Line_Sensor_Reading reading;

  #ifdef DEBUG3
    std::cout << " + Starting to line_follow (with adjustment)" << std::endl;
  #endif
  // Keep correcting until an intersection is reached
  while (intersection_detected == false) {
    reading = line_sensors.get_sensor_reading();

    if ((reading.front_left == 1) and (reading.front_right == 0)) {
      // Need to go more towards left
      left_motor.drive(reduced_speed);
      right_motor.drive(speed); // Right wheel going faster
    } else if ((reading.front_left == 0) and (reading.front_right == 1)) {
      // Need to go more towards right
      left_motor.drive(speed);
      right_motor.drive(reduced_speed); // Left wheel going faster
    } else if ((reading.front_left == 0) and (reading.front_right == 0)) {
      // Make both motors go forwards, but keep turning left ever so slightly
      left_motor.drive(slow_turn_speed);
      right_motor.drive(speed);
    } else if ((reading.front_left == 1) and (reading.front_right == 1)) {
      intersection_detected = true;
      #ifdef DEBUG2
        std::cout << " + Junction detected. Motors moving and at equal speeds." << std::endl;
      #endif
      // Make motors go at equal speeds (don't want to go
      // in a curved path over the intersection)
      left_motor.drive(speed);
      right_motor.drive(speed);
    }
  }
  // Keep driving forwards while over the junction:
  while ((reading.front_left == 1) and (reading.front_right == 1)) {
    reading = line_sensors.get_sensor_reading();
  }
  return;
}

void Line_Following::turn(int degrees, float speed) {
  // degrees can be either 90, 180, -90, or -180 (clockwise)
  // if 270, -270 call itself with -90 or 90 respectively

  if (degrees == 270) {
    turn(-90, speed);
    return;
  } else if (degrees == -270) {
    turn(90, speed);
    return;
  } else if (degrees == 0) {
    return;
  }

  #ifdef DEBUG3
    std::cout << " + Turning by: " << degrees << std::endl;
  #endif

  #ifdef DEBUG
    if ((degrees !=  90) and (degrees !=  180) and (degrees !=  -90) and (degrees !=  -180)) {
      std::cout << "Invalid value of degrees: " << degrees << std::endl;
      throw std::invalid_argument( "received negative value");
    }
  #endif


  // Turn by some amount without monitoring the sensors
  int turn_blindly_by = 45;
  switch (degrees) {
    case 90:
      turn_exactly(turn_blindly_by, speed, false);
      break;
    case 180:
      turn_exactly(90 + turn_blindly_by, speed, false);
      break;
    case -90:
      turn_exactly(-turn_blindly_by, speed, false);
      break;
    case -180:
      turn_exactly(-90 - turn_blindly_by, speed, false);
      break;
  }

  #ifdef DEBUG3
    std::cout << " + Looking for line." << degrees << std::endl;
  #endif

  // Start monitoring the sensors to detect the incoming line:
  Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
  if (degrees > 0) { // If clockwise
    while (!((reading.front_left == 0) and (reading.front_right == 1))) {
      reading = line_sensors.get_sensor_reading();
    }
  }
  else if (degrees < 0) { // If counter-clockwise
    while (!((reading.front_left == 1) and (reading.front_right == 0))) {
      reading = line_sensors.get_sensor_reading();
    }
  }

  #ifdef DEBUG3
    std::cout << " + Line found. Motors starting to go at alignment speed. Aligning." << degrees << std::endl;
  #endif

  // Align with the line
  // TODO: See if necessary. On second thoughts probably
  float alignment_speed = TURN_ALIGN_SPEED_FRAC * speed;

  if (degrees > 0) {// If clockwise
    left_motor.drive(alignment_speed);
    right_motor.drive(-alignment_speed);
  }
  else if (degrees < 0){ // If counter-clockwise
    left_motor.drive(-alignment_speed);
    right_motor.drive(alignment_speed);
  }
  align_after_turn(alignment_speed);

  #ifdef DEBUG3
    std::cout << " + Aligned." << degrees << std::endl;
  #endif

  return;
}

// TODO: finish and replace turn for reversing.
void Line_Following::turn_rear_align(int degrees, float speed) {
  // Turn and align with rear sensors instead of front.

  // degrees can be either 90, 180, -90, or -180 (clockwise)
  // if 270, -270 call itself with -90 or 90 respectively

  if (degrees == 270) {
    turn(-90, speed);
  } else if (degrees == -270) {
    turn(90, speed);
  } else if (degrees == 0) {
    return;
  }

  #ifdef DEBUG3
    std::cout << " + Turning by: " << degrees << std::endl;
  #endif

  #ifdef DEBUG
    if ((degrees !=  90) and (degrees !=  180) and (degrees !=  -90) and (degrees !=  -180)) {
      std::cout << "Invalide value of degrees: " << degrees << std::endl;
      throw std::invalid_argument( "received negative value");
    }
  #endif


  // Turn by some amount without monitoring the sensors
  int turn_blindly_by = 45;
  switch (degrees) {
    case 90:
      turn_exactly(turn_blindly_by, speed, false);
      break;
    case 180:
      turn_exactly(90 + turn_blindly_by, speed, false);
      break;
    case -90:
      turn_exactly(-turn_blindly_by, speed, false);
      break;
    case -180:
      turn_exactly(-90 - turn_blindly_by, speed, false);
      break;
  }

  #ifdef DEBUG3
    std::cout << " + Looking for line." << degrees << std::endl;
  #endif

  // Start monitoring the sensors to detect the incoming line:
  Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
  if (degrees > 0) { // If clockwise
    while (!((reading.back_left == 1) and (reading.back_right == 0))) {
      reading = line_sensors.get_sensor_reading();
    }
  }
  else if (degrees < 0) { // If counter-clockwise
    while (!((reading.back_left == 0) and (reading.back_right == 1))) {
      reading = line_sensors.get_sensor_reading();
    }
  }

  #ifdef DEBUG3
    std::cout << " + Line found. Motors starting to go at alignment speed. Aligning." << degrees << std::endl;
  #endif

  // Align with the line
  // TODO: See if necessary. On second thoughts probably
  float alignment_speed = TURN_ALIGN_SPEED_FRAC * speed;

  if (degrees > 0) {// If clockwise
    left_motor.drive(alignment_speed);
    right_motor.drive(-alignment_speed);
  }
  else if (degrees < 0){ // If counter-clockwise
    left_motor.drive(-alignment_speed);
    right_motor.drive(alignment_speed);
  }
  align_rear_after_turn(alignment_speed);

  #ifdef DEBUG3
    std::cout << " + Aligned." << degrees << std::endl;
  #endif
  return;
}

void Line_Following::align_after_turn(float alignment_speed) {
  stopwatch watch;
  int time_to_confirm_alignment = 30;

  Line_Sensor_Reading reading;
  while (watch.read() >= time_to_confirm_alignment) {
	// Read the light sensors
	reading = line_sensors.get_sensor_reading();

	#ifdef DEBUG3
    Line_Sensor_Reading last_reading;
    if ((last_reading.front_left != reading.front_left) or (last_reading.front_right != reading.front_right)) {
      // If the reading changes, print it out.
      std::cout << " + Current line sensor reading: " << reading.front_left << " "
      << reading.front_right << std::endl;
      last_reading = reading;
    }
    #endif

    if ((reading.front_left == 1) and (reading.front_right == 0)) {
      // Need to go more towards left
      left_motor.drive(-alignment_speed);
      right_motor.drive(alignment_speed);
      watch.stop();
    } else if ((reading.front_left == 0) and (reading.front_right == 1)) {
      // Need to go more towards right
      left_motor.drive(alignment_speed);
      right_motor.drive(-alignment_speed);
      watch.stop();
    } else if ((reading.front_left == 0) and (reading.front_right == 0)) {
      // Make both motors go at the same speed.
      left_motor.drive(0);
      right_motor.drive(0);
      watch.start();
    } else if ((reading.front_left == 1) and (reading.front_right == 1)) {
      std::cout << "An unexpected detection of junction occured!" << std::endl;
    }
  }
}

void Line_Following::align_rear_after_turn(float alignment_speed) {
  stopwatch watch;
  int time_to_confirm_alignment = 30;

  Line_Sensor_Reading reading;
  while (watch.read() >= time_to_confirm_alignment) {
	// Read the light sensors
	reading = line_sensors.get_sensor_reading();

	#ifdef DEBUG3
    Line_Sensor_Reading last_reading;
    if ((last_reading.back_left != reading.back_left) or (last_reading.back_right != reading.back_right)) {
      // If the reading changes, print it out.
      std::cout << " + Current line sensor reading: " << reading.front_left << " "
      << reading.front_right << std::endl;
      last_reading = reading;
    }
    #endif

    if ((reading.back_left == 1) and (reading.back_right == 0)) {
      // Need to go more towards left
      left_motor.drive(alignment_speed);
      right_motor.drive(-alignment_speed);
      watch.stop();
    } else if ((reading.back_left == 0) and (reading.back_right == 1)) {
      // Need to go more towards right
      left_motor.drive(-alignment_speed);
      right_motor.drive(alignment_speed);
      watch.stop();
    } else if ((reading.back_left == 0) and (reading.back_right == 0)) {
      // Make both motors go at the same speed.
      left_motor.drive(0);
      right_motor.drive(0);
      watch.start();
    } else if ((reading.back_left == 1) and (reading.back_right == 1)) {
      std::cout << "An unexpected detection of junction occured!" << std::endl;
    }
  }
}

void Line_Following::set_ramp(int ramp_time) {
  if (current_ramp_time != ramp_time) {
    rlink.command(RAMP_TIME, ramp_time);
    current_ramp_time = ramp_time;
  }
}

void Line_Following::stop_motors() {
  left_motor.drive(0.0);
  right_motor.drive(0.0);
}

void Line_Following::reverse_until_switch(float speed, float speed_delta) {
  #ifdef DEBUG2
  std::cout << " + Making motors go in reverse. Current speed sent to .drive(): "
  << -speed << std::endl;
  #endif
  left_motor.drive(-speed);
  right_motor.drive(-speed);

  float reduced_speed = speed - speed * speed_delta;

  microswitches.update_state();
  while (microswitches.rear_state == 0) {
	Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
    adjust_reverse_speeds_from_reading(speed, reduced_speed, reading);
    microswitches.update_state();
  }

  left_motor.drive(0);
  right_motor.drive(0);
  #ifdef DEBUG
  std::cout << " + Rear switch triggered" << std::endl;
  #endif
  return;
}

void Line_Following::follow_line_until_switch(float speed, float speed_delta) {
  left_motor.drive(speed);
  right_motor.drive(speed);

  float reduced_speed = speed - speed * speed_delta;

  microswitches.update_state();
  while (microswitches.front_state == 0) {
	  Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
    adjust_speeds_from_reading(speed, reduced_speed, reading);
    microswitches.update_state();
  }

  left_motor.drive(0);
  right_motor.drive(0);
  return;
}

void Line_Following::turn_exactly(int degrees, float speed, bool stop_after) {
	// TODO: calculate rotate_for by using a constant
  float rotate_time_360 = 5700; // TODO: Recalibrate for proper chasis.

  int rotate_for = rotate_time_360 * float(abs(degrees)) / 360.0;
  #ifdef DEBUG3
    std::cout << " + Turning blindly by " << degrees << " degrees for "
    << rotate_for << "ms. Turning speed: " << speed << std::endl;
  #endif
  // Start rotating
  if (degrees < 0) {
    left_motor.drive(-speed);
    right_motor.drive(speed);
  } else if (degrees > 0) {
    left_motor.drive(speed);
    right_motor.drive(-speed);
  }

  #ifdef DEBUG3
	std::cout << " + Current requested motor speeds: left:"  << left_motor.get_requested_speed()
     << "    right: " << right_motor.get_requested_speed() << std::endl;
	std::cout << " + Current actual motor speeds: left:"  << left_motor.get_current_speed()
     << "    right: " << right_motor.get_current_speed() << std::endl;

  #endif




  // Wait
  delay(rotate_for);
  // Stop the motors
  if (stop_after) {
    left_motor.drive(0);
    right_motor.drive(0);
  }
  #ifdef DEBUG3
    std::cout << " + Turning blindly delay finished." << std::endl;
    std::cout << " + Current requested motor speeds: left:"  << left_motor.get_requested_speed()
     << "    right: " << right_motor.get_requested_speed() << std::endl;
	std::cout << " + Current actual motor speeds: left:"  << left_motor.get_current_speed()
     << "    right: " << right_motor.get_current_speed() << std::endl;

  #endif
  return;
}

int Line_Following::is_on_the_line() {
	return 1;
}

int Line_Following::wiggle_to_line() {
	return 1;
}

int Line_Following::reiterate_in_reverse(float reiterate_for) {
	return 1;
}

void Line_Following::motors_go(float left_speed, float right_speed) {
  left_motor.drive(left_speed);
  right_motor.drive(right_speed);
};
