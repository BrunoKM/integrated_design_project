#include <robot_link>
#include "line_following.h"


Line_Following::Line_Following(): current_speed(0) {

}

void Line_Following::follow_line(float speed, float ramp, float speed_delta, int num_intersections_to_ignore,
                                 bool keep_driving_after_last) {
  
};

void Line_Following::align_with_intersection() {
// Keep driving for a pre-calibrated amount until drive axis is over intersection
};

void Line_Following::turn(int degrees) {
// Activate motors in opposite directions to turn by 90, 180, -90, -180 until line-following sensor detects desired line
};

void Line_Following::follow_line_reverse(float speed, float ramp) {

};

int Line_Following::is_on_the_line() {

};

int Line_Following::wiggle_to_line() {
// Start the wiggle in front_last_to_the or back_last_to_the depending on driving direction, if no line found, return to original orientation
};

int Line_Following::reiterate_in_reverse(float reiterate_for) {
// Drive in opposite direction until the direction-leading sensors detect the line again for reiterate_for amount of time
};





Line_Sensor_Reading::Line_Sensor_Reading(int sensor_reading){}

int Line_Sensor_Reading::back_left() {
// Return reading of back_left sensor  (1 or 0)
};

int Line_Sensor_Reading::back_right() {

};

int Line_Sensor_Reading::front_left() {

};

int Line_Sensor_Reading::front_right(){

};





Line_Sensors::Line_Sensors(int address){}

Line_Sensor_Reading Line_Sensors::read_sensor_input() {
// Read sensor input and return line_sensor_reading
};
