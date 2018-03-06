// Declares the class Line_Following

// Also, change the parameters for the motors here.
#ifndef LINE_FOLLOWING_H
#define LINE_FOLLLOWING_H
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

    // Methods
    void align_after_turn(float alignment_speed);
    // TODO: these need to be written
    int is_on_the_line(); // return 1 if true, 0 if false, 2 if intersection encountered
    int wiggle_to_line(); // return 1 if successful, 0 if failed
    int reiterate_in_reverse(float reiterate_for); // return 1 if successful, 0 if failed

public:
    Line_Following(Line_Sensors &line_sensors) : current_speed(0),
    current_ramp_time(-1), left_motor(Driving_Motor(1, 1.0, 1.0)),
    right_motor(Driving_Motor(2, 1.0, -1.0)), line_sensors(line_sensors) {}

    // Methods
    void follow_line_until_intersection(float speed, float ramp, float speed_delta);
    void follow_line(float speed, float ramp, float speed_delta,
      int num_intersections_to_ignore, bool keep_driving_after_last);
    void align_with_intersection(int time_duration, float speed, float speed_delta);
    void turn(int degrees, float speed);
    void follow_line_reverse(float speed, float ramp);
    void set_ramp(int ramp_time);
    void stop_motors();

};




#endif
