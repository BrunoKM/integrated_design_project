# Automatically generated Makefile
sensorboard_output_test :  sensorboard_output_test.o robot_initialise.o
	 g++ -L/export/teach/1BRobot -o sensorboard_output_test  sensorboard_output_test.o  robot_initialise.o  -lrobot

sensorboard_output_test.o: sensorboard_output_test.cc robot_initialise.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c sensorboard_output_test.cc
robot_initialise.o: robot_initialise.cc robot_initialise.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c robot_initialise.cc
