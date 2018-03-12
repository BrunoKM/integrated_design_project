// Go until second intersection and align with it (axis over intersection), turn around, repeat

#include <iostream>
#include <string>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <delay.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"
#include "Robot.h"

int main () {

  int starting_direction = 180;
  char starting_junction = 'j';

  Robot robot(starting_junction, starting_direction);
  int num_repeat = 20;
  cout << "Running curve blind test." << endl;

  for (int i = 1; i <= num_repeat; i++) {
    cout << "Repetition:  " << i << endl;
    robot.move('l');
    robot.move('j')
  }
  }
}
