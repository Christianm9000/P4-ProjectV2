import pandas as pd
import matplotlib.pyplot as plt

file1 = "tx_check_downlink%adr.csv"
file2 = "tx_check_downlink+adr.csv"
df1 = pd.read_csv(file1)
df2 = pd.read_csv(file2)
x = df1["Timestamp(ms)"]
y1 = df1["Current(uA)"] / 1000
y2 = df2["Current(uA)"] / 1000
fig, ax1 = plt.subplots(figsize=(16, 9))
color = 'tab:red'
ax1.set_xlabel('Timestamp (ms)')
ax1.set_ylabel('Current (mA) - Without ADR', color=color)
ax1.plot(x, y1, color=color)
ax1.tick_params(axis='y', labelcolor=color)
ax2 = ax1.twinx()  
color = 'tab:green'
ax2.set_ylabel('Current (mA) - With ADR', color=color)  
ax2.plot(x, y2, color=color)
ax2.tick_params(axis='y', labelcolor=color)
ax1.grid(True)
ax2.grid(True)
fig.tight_layout()  
plt.savefig("milliamps_draw.png", dpi=500)
plt.show()