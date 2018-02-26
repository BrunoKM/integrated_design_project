"""
Generate a whole list of actual motor speeds corresponding to each of the
demanded 'motor speed commands' at the input by interpolation.
"""
import numpy as np

if __name__ == '__main__':
    motor_num = 1 # Enter which motor to produce the graph for!
    filename = "motor_" + motor_num + "_speed_test_results.txt"
    results = np.loadtxt(filename)

    cmd_speeds_measured = results[:, 0]
    angular_speeds_measured = results[:, 1]

    # We want results for every single cmd_speed
    cmd_speeds = np.arange(128, dtype=np.int32)
    angular_speeds = np.zeros(128, dtype=np.float32)

    # Interpolate between the measured data points
    for cmd_speed in range(0, 128):
        if cmd_speed in cmd_speeds_measured: # For first iteration: true
            # Index of the lower value in the measured array
            idx_lower = np.searchsorted(cmd_speeds_measured, cmd_speed)
            # Check if there are more points to interpolate between. If not
            # just continue by extrapolating
            if (idx_lower + 1) < len(cmd_speeds_measured):
                x_lower = cmd_speed
                x_higher = cmd_speeds_measured[idx_lower + 1]
                y_lower = angular_speeds_measured[idx_lower]
                y_higher = angular_speeds_measured[idx_lower + 1]

                x_delta = x_higher - x_lower
                y_delta = y_higher - y_lower
            angular_speeds[cmd_speed] = angular_speeds_measured[idx_lower]
        else:
            angular_speed = y_lower + (cmd_speed - x_lower) * (y_delta) / (x_delta)
            angular_speeds[cmd_speed] = angular_speed

    # Save the results to a file
    out_name = "motor_" + motor_num + "_calibration_tab.txt"
    with open(out_name, "w") as out_file:
        for cmd_speed in cmd_speeds:
            out_file.write(cmd_speed + " " + angular_speeds[cmd_speed])
