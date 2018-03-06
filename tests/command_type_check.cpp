#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <bitset>
#include <typeinfo>

int main () {
	cout << READ_PORT_1 << endl;
	cout << READ_PORT_2 << endl;
	
	cout << READ_PORT_3 << endl;
	cout << WRITE_PORT_1 << endl;
	cout << "ORDER: rp1, rp2, rp3, wp1" << endl;
	string type_s = typeid(READ_PORT_1).name();
	cout << type_s <<endl;
	request_instruction x;
	x = READ_PORT_1;
	cout << x << endl;
	string type_s2 =typeid(WRITE_PORT_1).name();
	cout << "write instruction type" << type_s2 <<endl;
	string type_s3 =typeid(MOTOR_1_GO).name();
	cout << "write instruction type" << type_s3 <<endl;
  return 0;                          // Test succesful
}
