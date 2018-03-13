test_lf_forward_backwards:  test_turn_exactly.o robot_initialise.o Robot.o line_following.o Driving_Motor.o components.o
	 g++ -L/export/teach/1BRobot -o test_turn_exactly  test_turn_exactly.o  robot_initialise.o  Robot.o  line_following.o  Motor.o  components.o  -lrobot

test_turn_exactly.o: test_beacon_reader.cpp robot_initialise.h components.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c test_turn_exactly.cpp
robot_initialise.o: robot_initialise.cc robot_initialise.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c robot_initialise.cc
Robot.o: Robot.cpp Robot.h robot_initialise.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c Robot.cpp
line_following.o: line_following.cpp line_following.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c line_following.cpp
Motor.o: Motor.cpp Motor.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c Driving_Motor.cpp
components.o: components.cpp components.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c components.cpp
