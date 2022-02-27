import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


# Load temperature data
temps = pd.read_excel('temperatures.xlsx').temp

# Plot temp over time
plt.scatter(np.array(range(len(temps)))/60, temps, zorder=2, color='r')
plt.title('Temperature as a Function of Time', fontsize=22)
plt.xlabel('Time (hours)', fontsize=16)
plt.ylabel('Temperature (degrees Celsius)', fontsize=16)
plt.grid()
plt.savefig('temperature_plot.png')
