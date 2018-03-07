// Go until second intersection and align with it (axis over intersection), turn around, repeat

#include <iostream>
#include <string>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include "robot_initialise.h"
#include "line_following.h"
#include "components.h"
#include "Robot.h"

int main () {

  Robot robot;
  cout << "--- Subsystems Demo ---" << endl;
  cout << "Running Task 1:" << endl;
  robot.move("j");

  string usr_reponse;
  while (usr_response != "yes"){
    cout << "   Ready for Task 2?" << endl;
    cin >> usr_reponse;
  }
  cout << "Running Task 2:" << endl;
  robot.align_for_pickup();

  usr_reponse = "";
  while (usr_response != "yes"){
    cout << "   Ready for Task 3?" << endl;
    cin >> usr_reponse;
  }
  cout << "Running Task 3:" << endl;
  robot.move("l");


}
