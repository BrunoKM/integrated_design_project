#include <robot_link>
#include "line_following.h"


Line_Following::Line_Following(): current_speed(0) {

}

void Line_Following::follow_line(float speed, float ramp, float speed_delta, int num_intersections_to_ignore,
                                 bool keep_driving_after_last) {
  
};

void Line_Following::align_with_intersection() {
// Keep driving for a pre-calibrated amount until
};

void Line_Following::turn(int degrees) {

};

void Line_Following::follow_line_reverse(float speed, float ramp) {

};

int Line_Following::is_on_the_line() {

};

int Line_Following::wiggle_to_line() {

};

int Line_Following::reiterate_in_reverse(float reiterate_for) {

};





Line_Sensor_Reading::Line_Sensor_Reading(int sensor_reading){}

int Line_Sensor_Reading::back_left() {

};

int Line_Sensor_Reading::back_right() {

};

int Line_Sensor_Reading::front_left() {

};

int Line_Sensor_Reading::front_right(){

};





Line_Sensors::Line_Sensors(int address){}

Line_Sensor_Reading Line_Sensors::read_sensor_input() {

};
