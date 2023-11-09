import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('simulation_data.txt')

x = data[:, 0]
y1 = data[:, 1]
y2 = data[:, 2]
y3 = data[:, 3]
y4 = data[:, 4]

plt.figure()
plt.plot(x, y1, label='First-Fit')
plt.plot(x, y2, label='Next-Fit')
plt.plot(x, y3, label='Best-Fit')
plt.plot(x, y4, label='Worst-Fit')
plt.xlabel('Run Time')
plt.ylabel('Average Holes Examined')
plt.legend()
plt.show()

y5 = data[:, 5]
y6 = data[:, 6]
y7 = data[:, 7]
y8 = data[:, 8]

plt.figure()
plt.plot(x, y5, label='First-Fit')
plt.plot(x, y6, label='Next-Fit')
plt.plot(x, y7, label='Best-Fit')
plt.plot(x, y8, label='Worst-Fit')
plt.xlabel('Run Time')
plt.ylabel('Memory Utilization')
plt.legend()
plt.show()