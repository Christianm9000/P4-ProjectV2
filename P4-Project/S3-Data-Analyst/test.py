import pymysql, json
from datetime import datetime, timedelta

with open('config.json', 'r') as file:
    config = json.load(file)

offset= -12

db = config['database']
conn = pymysql.connect(host=db['host'], user=db['username'], passwd=db['password'], db=db['dbname'])
cursor = conn.cursor()
cursor.execute("SELECT ts, data FROM measure_data")
data = cursor.fetchall()
conn.close()
times = [datetime.fromtimestamp(row[0] / 1000).replace(microsecond=0) for row in data if row[0]]
values = [row[1] for row in data if row[1]]
#print(values)
#numbers = [int(value, 16) for value in values]
#print(numbers)

# Convert hex to binary
bits = [bin(int(value, 16))[2:].zfill(len(value)*4) for value in values]
#print(bits[0])
#print(len(bits[0]))
timestamps = []
for time, bit_string in zip(times, bits):
    number_of_measurements = len(bit_string) // 14
    timestamps.extend([time - timedelta(minutes=30*i) for i in range(number_of_measurements)])

moisture = [bit_string[i:i+7] for bit_string in bits for i in range(0, len(bit_string), 7) if len(bit_string[i:i+7]) == 7 and (i // 7) % 2 == 0]
temperature = [bit_string[i:i+7] for bit_string in bits for i in range(0, len(bit_string), 7) if len(bit_string[i:i+7]) == 7 and (i // 7) % 2 != 0]

moisture_percentages = [str(round((int(bit_string, 2) / 127) * 100, 0)) + '%' for bit_string in moisture]
temperature_values = [int(bit_string[:6], 2) - 32 - (offset) + (0.5 if bit_string[-1] == '1' else 0) for bit_string in temperature]
#print(moisture_percentages)
#print(temperature_values)
# Define your desired date and time format
date_format = "%Y-%m-%d %H:%M:%S"

# Convert the timestamps to strings and print them
timestamp_strings = [timestamp.strftime(date_format) for timestamp in timestamps]
print(timestamp_strings)