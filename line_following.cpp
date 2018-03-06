
#include <iostream>
#include <stdexcept>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include "line_following.h"

#define DEBUG // TODO: Comment out when running for competition!
#define DEBUG2 // Even more detailed info

// Parameters to tune:
const int TURN_DELAY_TIME=200; // Time in msto wait for during turning to make sure sensors are off the line
const float TURN_ALIGN_SPEED_FRAC = 0.1; // Fraction of speed to apply to align with line after turned
const int NUM_TO_CONFIRM_ALIGNMENT = 4; // Number of iterations to confirm that robot is on the line



// TODO: Potentially have integral control
// TODO: Add '11' reading counter to be more certain of junctions
void Line_Following::follow_line_until_intersection(float speed, float ramp, float speed_delta) {
  // Assumes motors already started
  float reduced_speed = speed - speed * speed_delta;
  bool intersection_detected = false;
  
  Line_Sensor_Reading reading;

  while (intersection_detected == false) {
    reading = line_sensors.get_sensor_reading();
    
    #ifdef DEBUG2
    std::cout << "\rLine Sensors reading: " << reading.front_left << " "
    << reading.front_right << std::flush;
    #endif
    
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
      intersection_detected = true;
      #ifdef DEBUG
      std::cout << "Junction detected" << std::endl;
      #endif
      // Make motors go at equal speeds (don't want to go
      // in a curved path over the intersection)
      left_motor.drive(speed);
      right_motor.drive(speed);
    }
  }
  return;
}

void Line_Following::follow_line(float speed, float ramp, float speed_delta, int num_intersections_to_ignore,
                                 bool keep_driving_after_last) {
  // Start
  left_motor.drive(speed);
  right_motor.drive(speed);

  int intersections_crossed = 0;
  while (intersections_crossed <= num_intersections_to_ignore) {
    follow_line_until_intersection(speed, ramp, speed_delta);
    intersections_crossed += 1;
    // wait until intersection crossed
    Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
    // TODO: May have to add counter to avoid error
    while ((reading.front_left == 1) and (reading.front_right == 1)) {
      reading = line_sensors.get_sensor_reading();
    }
  }
  if (keep_driving_after_last == 0) {
    left_motor.drive(0.0);
    right_motor.drive(0.0);
  }

  return;
}

void Line_Following::align_with_intersection(int time_duration, float speed, float speed_delta) {
// Keep driving for a pre-calibrated amount until wheel axis is over intersection
  stopwatch watch;
  watch.start();
  
  float reduced_speed = speed - speed * speed_delta;
  
  while (watch.read() < time_duration) {
    static Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
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
      std::cout << "An unexpected detection of junction occured" << std::endl;
    }
  }
  // Stop the motors
  left_motor.drive(0);
  right_motor.drive(0);
}


// TODO: Add rotation estimation capabilities (to avoid wrong turns)!!
void Line_Following::turn(int degrees, float speed) {
  // degrees can be either 90, 180, -90, or -180 (clockwise)

  #ifdef DEBUG
  if ((degrees !=  90) and (degrees !=  180) and (degrees !=  -90) and (degrees !=  -180)) {
    std::cout << "Invalide value of degrees: " << degrees << std::endl;
    throw std::invalid_argument( "received negative value");
  }
  #endif

  if (degrees > 0) {
    // Rotate in clockwise direction
    left_motor.drive(speed);
    right_motor.drive(-speed);
    delay(TURN_DELAY_TIME);

    Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
    while (!((reading.front_left == 0) and (reading.front_right == 1))) {
      // Continue rotating until the right sensor picks up the line.
      reading = line_sensors.get_sensor_reading();
    }
    if (degrees == 180) {
      // Call the function recursively:
      turn(90, speed);
    } else {
      // align the robot with the line

      // Reduce the speed
      float alignment_speed = TURN_ALIGN_SPEED_FRAC * speed;
      left_motor.drive(alignment_speed);
      right_motor.drive(-alignment_speed);

      align_after_turn(alignment_speed);
    }
  }
  else if (degrees < 0) {
    // Rotate in counter-clockwise direction
    left_motor.drive(-speed);
    right_motor.drive(speed);
    delay(TURN_DELAY_TIME);

    Line_Sensor_Reading reading = line_sensors.get_sensor_reading();
    while (!((reading.front_left == 1) and (reading.front_right == 0))) {
      // Continue rotating until the right sensor picks up the line.
      reading = line_sensors.get_sensor_reading();
    }
    if (degrees == -180) {
      // Call the function recursively:
      turn(-90, speed);
    } else {
      // align the robot with the line

      // Reduce the speed
      float alignment_speed = TURN_ALIGN_SPEED_FRAC * speed;
      left_motor.drive(-alignment_speed);
      right_motor.drive(alignment_speed);

      align_after_turn(alignment_speed);
    }
  }
}

void Line_Following::align_after_turn(float alignment_speed) {
  int n_iter_on_line = 0;
  
  Line_Sensor_Reading reading;
  while (n_iter_on_line < NUM_TO_CONFIRM_ALIGNMENT) {
	// Read the light sensors
	reading = line_sensors.get_sensor_reading();
	
    if ((reading.front_left == 1) and (reading.front_right == 0)) {
      // Need to go more towards left
      left_motor.drive(-alignment_speed);
      right_motor.drive(alignment_speed);
      n_iter_on_line = 0;
    } else if ((reading.front_left == 0) and (reading.front_right == 1)) {
      // Need to go more towards right
      left_motor.drive(alignment_speed);
      right_motor.drive(-alignment_speed);
      n_iter_on_line = 0;
    } else if ((reading.front_left == 0) and (reading.front_right == 0)) {
      // Make both motors go at the same speed.
      left_motor.drive(0);
      right_motor.drive(0);
      n_iter_on_line += 1;
    } else if ((reading.front_left == 1) and (reading.front_right == 1)) {
      std::cout << "An unexpected detection of junction occured" << std::endl;
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

void Line_Following::follow_line_reverse(float speed, float ramp) {

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
