import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Given data
data = [(0, 4.5), (5, 4.38), (10, 4.33), (15, 4.30), (20, 4.28), (40, 4.25), (50, 4.23), (80, 4.20)]

# Extracting the min and V values
min_values = np.array([point[0] for point in data])
V_values = np.array([point[1] for point in data])

# Define the exponential decay model function
def exponential_decay(x, V0, k):
    return V0 * np.exp(-k * x)

# Initial guess for the parameters V0 and k
initial_guess = (V_values[0], 0.01)

# Fit the model to the data
params, covariance = curve_fit(exponential_decay, min_values, V_values, p0=initial_guess)

# Extract the parameters
V0, k = params

# Print the fitted parameters
print(f"Fitted parameters: V0 = {V0}, k = {k}")

# Define a very small voltage value to find when it reaches "zero"
very_small_voltage = 1e-6

# Calculate the time at which the voltage is approximately zero
time_to_zero = np.log(very_small_voltage / V0) / -k
print(f"Time to reach approximately zero voltage: {time_to_zero} minutes")

# Generate points for plotting the fitted curve
min_fit = np.linspace(0, time_to_zero, 500)
V_fit = exponential_decay(min_fit, V0, k)

# Create the plot with a 16:9 aspect ratio
plt.figure(figsize=(16, 9))

# Plot the data points
plt.scatter(min_values, V_values, label='Data', color='red')

# Plot the fitted exponential decay curve
plt.plot(min_fit, V_fit, label='Fitted Curve', color='blue')

# Plot the time to reach approximately zero voltage
plt.axvline(x=time_to_zero, color='green', linestyle='--', label='Time to 0 Voltage')

# Add labels and legend
plt.xlabel('Time (min)')
plt.ylabel('Voltage (V)')
plt.title('Exponential Decay Fit')
plt.legend()
plt.grid(True)

# Show the plot
plt.show()
