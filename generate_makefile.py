import os

if __name__ == "__main__":
    # Open the makefile
    makefile  = open("tests_makefile.mk", "w")

    executable_lines_array = []
    dependencies_lines_array = []

    # For every file beginning with test and ending with cpp
    for file in os.listdir("."):
        if (file.endswith(".cpp") and "test" in file):
            filename, _ = os.path.splitext(file)
            # Store lines for that file to arrays
            executable_lines_array.append("{} :  {}.o robot_initialise.o Robot.o line_following.o Driving_Motor.o components.o\n".format(filename, filename))
            executable_lines_array.append("\t g++ -L/export/teach/1BRobot -o {}  {}.o  robot_initialise.o  Robot.o  line_following.o  Motor.o  components.o  -lrobot\n".format(filename, filename))

            dependencies_lines_array.append("{}.o: {}.cpp robot_initialise.h components.h Motor.h line_following.h Robot.h\n".format(filename, filename))
            dependencies_lines_array.append("\t g++ -ansi -Wall -g -I/export/teach/1BRobot -c {}.cpp\n".format(filename))

    for line in executable_lines_array:
        makefile.write(line)

    makefile.write("\n")

    for line in dependencies_lines_array:
        makefile.write(line)

    # Finally add the compilation for the core files:
    makefile.write("robot_initialise.o: robot_initialise.cc robot_initialise.h\n\t g++ -ansi -Wall -g -I/export/teach/1BRobot -c robot_initialise.cc\nRobot.o: Robot.cpp Robot.h robot_initialise.h\n\t g++ -ansi -Wall -g -I/export/teach/1BRobot -c Robot.cpp\nline_following.o: line_following.cpp line_following.h\n\t g++ -ansi -Wall -g -I/export/teach/1BRobot -c line_following.cpp\nMotor.o: Motor.cpp Motor.h\n\t g++ -ansi -Wall -g -I/export/teach/1BRobot -c Driving_Motor.cpp\ncomponents.o: components.cpp components.h\n\t g++ -ansi -Wall -g -I/export/teach/1BRobot -c components.cpp")

    makefile.close()
