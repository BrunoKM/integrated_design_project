"""
Make and save a plot of the motor speeds vs motor commands.
"""

import numpy as np
import matplotlib.pyplot as plt


if __name__ == '__main__':
    motor_num = 1 # Enter which motor to produce the graph for!
    filename = "motor_" + motor_num + "_speed_test_results.txt"
    results = np.loadtxt(filename)
    plt.xlabel("'Motor speed' command")
    plt.ylabel("Actual motor speed (rpm)")
    plt.grid()
    plt.scatter(results[:, 0], results[:, 1], label='Measurements')
    plt.show()
    out_name = "motor_" + motor_num + "_speed_graph.png"
    plt.savefig(out_name)
