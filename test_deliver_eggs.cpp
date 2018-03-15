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

  char starting_junction = 'd';
  int starting_direction = 0;
  Robot robot(starting_junction, starting_direction);
  cout << "Test putting eggs in the baskets" << endl;

  string usr_response;
  int which_basket;
  cout << "Which basket to put eggs in?" << endl;
  cin >> which_basket;
  robot.move();// TODO: Finish off later on.
  delay(2000);

  usr_response = "";
  while (usr_response != "yes"){
    cout << "   Ready for Task 3?" << endl;
    cin >> usr_response;
  }
  cout << "Running Task 3:" << endl;
  robot.move('l');


}
