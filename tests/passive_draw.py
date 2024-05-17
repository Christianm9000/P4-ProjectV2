import matplotlib.pyplot as plt

initial_voltage = 4.40
final_voltage = 1.0
voltage_drop_per_minute = 0.022
time_intervals = []
voltage_levels = []
current_voltage = initial_voltage
while current_voltage > final_voltage:
    time_intervals.append(len(time_intervals))
    voltage_levels.append(current_voltage)
    current_voltage -= voltage_drop_per_minute
total_minutes = len(time_intervals)
fig, ax = plt.subplots(figsize=(16, 9))
ax.plot(time_intervals, voltage_levels)
ax.set_xlabel('Time (minutes)')
ax.set_ylabel('Voltage (V)')
ax.set_title('Super Capacitor Discharge')
ax.grid(True)
plt.savefig("super_capacitor_discharge_plot.png", dpi=300)
plt.show()
print("Total minutes to completely discharge:", total_minutes)