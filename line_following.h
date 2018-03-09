// Declares the class Line_Following

// Also, change the parameters for the motors here.
#ifndef LINE_FOLLOWING_H
#define LINE_FOLLOWING_H
#include "Driving_Motor.h"
#include "components.h"
#include "robot_initialise.h"

class Line_Following {
private:
    float current_speed;
    int current_ramp_time; // Init to a value out of range (default not known atm)
    char direction; // either f or b
    char back_last_to_the; // either l or r
    char front_last_to_the; // either l or r
    // Parameters for the motors:
    Driving_Motor left_motor;
    Driving_Motor right_motor;

    Line_Sensors line_sensors;
    Microswitches microswitches;

    // Methods
    void align_after_turn(float alignment_speed);
    // TODO: these need to be written
    int is_on_the_line(); // return 1 if true, 0 if false, 2 if intersection encountered
    int wiggle_to_line(); // return 1 if successful, 0 if failed
    int reiterate_in_reverse(float reiterate_for); // return 1 if successful, 0 if failed

public:
    Line_Following(Components &components);

    // Methods
    void follow_line_until_intersection(float speed, float speed_delta);
    void follow_line(float speed, float speed_delta,
      int num_intersections_to_ignore, bool keep_driving_after_last);
    void align_with_intersection(float speed, float speed_delta);
    void turn(int degrees, float speed); // Degrees can only be = 0 mod 90
    void turn_exactly(int degrees, float speed, bool stop_after); // Degrees can be any int < 360 in magnitude
    void reverse_until_switch(float speed, float speed_delta);
    void set_ramp(int ramp_time);
    void motors_go(float left_speed, float right_speed);

};




#endif
