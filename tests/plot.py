import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Given data in minutes and voltage
data = [(0, 4.5), (5, 4.38), (10, 4.33), (15, 4.30), (20, 4.28), (40, 4.25), (50, 4.23), (80, 4.20)]

# Convert the time to seconds and set the initial voltage to 5.4 V
initial_voltage = 5.4
time_min = np.array([point[0] for point in data])
time_sec = time_min * 60
voltage = np.array([point[1] for point in data])

# Define the exponential decay model function
def exponential_decay(t, V0, k):
    return V0 * np.exp(-k * t)

# Initial guess for the parameters V0 and k
initial_guess = (initial_voltage, 0.01)

# Fit the model to the data
params, covariance = curve_fit(exponential_decay, time_sec, voltage, p0=initial_guess)

# Extract the parameters
V0, k = params

# Print the fitted parameters
print(f"Fitted parameters: V0 = {V0}, k = {k}")

# Calculate the time at which the voltage is 1 V
target_voltage = 1.0
time_to_1V = np.log(target_voltage / V0) / -k
print(f"Time to reach 1 V: {time_to_1V} seconds")

# Define a very small voltage value to find when it reaches "zero"
very_small_voltage = 1e-6

# Calculate the time at which the voltage is approximately zero
time_to_zero = np.log(very_small_voltage / V0) / -k
print(f"Time to reach approximately zero voltage: {time_to_zero} seconds")

# Generate points for plotting the fitted curve
time_fit = np.linspace(0, time_to_zero, 500)
voltage_fit = exponential_decay(time_fit, V0, k)

# Create the plot with a 16:9 aspect ratio
plt.figure(figsize=(16, 9))

# Plot the data points
plt.scatter(time_sec, voltage, label='Data', color='red')

# Plot the fitted exponential decay curve
plt.plot(time_fit, voltage_fit, label='Fitted Curve', color='blue')

# Plot the time to reach approximately zero voltage
plt.axvline(x=time_to_zero, color='green', linestyle='--', label='Time to 0 Voltage')

# Plot the line at 1 V
plt.axhline(y=1, color='green', linestyle='--', label='1 V Line')
plt.axvline(x=time_to_1V, color='green', linestyle='--')

# Add labels and legend
plt.xlabel('Time (seconds)')
plt.ylabel('Voltage (V)')
plt.title('Exponential Decay Fit')
plt.legend()
plt.grid(True)

# Show the plot
plt.show()
