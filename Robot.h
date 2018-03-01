// Declares the class Robot which holds several parameters for the robot such
// as position, basket composition, e.t.c. , and defines high-level methods
// to provide an interface for executing general tasks such as moving from
// A to B, or dropping eggs off.
//
// Also contains helper class Eggs and struct Egg
#ifndef ROBOT_H
#define ROBOT_H

#include <string>

class Robot
{
  public:
    string position[2];
    // Array holding the current position information. It holds two characters
    // corresponding to the two junctions that the robot is currently in-between.
    // To see which character corresponds to which junction, see the picture at
    // the end of this section.

    int direction;
    Rough direction the robot is facing. Can be 0, 90, 180, 270.
    Directions defined in the picture below (clockwise).

Eggs recycling_eggs - the recycling eggs combination on the robot. The Eggs class is defined later on in this section.

Eggs delivery_eggs - the delivery eggs combination on the robot.

int baskets_delivered - number of baskets delivered.

string delivery_zone - either "d1" or "d2".

Reservoir reservoir

Scoop scoop

LEDs leds

Line_Following line_following
}

#endif
