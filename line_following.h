// Declares the class Line_Following
#ifndef LINE_FOLLOWING_H
#define LINE_FOLLLOWING_H
#include "Driving_Motor.h"
#include "robot_initialise.h"

class Line_Following {
private:

    float current_speed;
    float current_ramp;
    char direction; // either f or b
    char back_last_to_the; // either l or r
    char front_last_to_the; // either l or r
    Driving_Motor left_motor;
    Driving_Motor right_motor;

    // Methods
    int is_on_the_line(); // return 1 if true, 0 if false, 2 if intersection encountered
    int wiggle_to_line(); // return 1 if successful, 0 if failed
    int reiterate_in_reverse(float reiterate_for); // return 1 if successful, 0 if failed

public:
    Line_Following();

    // Methods
    void follow_line(float speed, float ramp, int num_intersections_to_ignore, bool keep_driving_after_last);
    void align_with_intersection();
    void turn(int degrees);
    void follow_line_reverse(float speed, float ramp);

};


class Line_Sensor_Reading {
public:
    Line_Sensor_Reading();

    int sensor_reading;

    // Sensor readings return 1 or 0
    int front_left();
    int front_right();
    int back_left();
    int back_right();
};

class Line_Sensors {

private:
    int address;

public:
    Line_Sensors();

    // Methods
    Line_Sensor_Reading read_sensor_input();


};


#endif