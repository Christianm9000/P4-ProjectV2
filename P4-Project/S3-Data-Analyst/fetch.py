import pymysql, json
from datetime import datetime
#Bits 0= 1111111 1111111 0111111 1000000 0000000 0000000 1111111 1111111
#Bits 1= 1111111 1111111 1111111 1111111 1111111 1111111 1111111 1111111 1111111 1111111 00
#Moisture = 1111111 0111111 0000000 1111111 1111111 1111111 1111111 1111111 1111111 00
#['1111111', '0111111', '0000000', '1111111', '1111111', '1111111', '1111111', '1111111', '1111111', '00']

with open('config.json', 'r') as file:
    config = json.load(file)

db_host = config['database']['host']
db_name = config['database']['dbname']
db_username = config['database']['username']
db_password = config['database']['password']

conn = pymysql.connect(host=db_host, user=db_username, passwd=db_password, db=db_name)
cursor = conn.cursor()
cursor.execute("SELECT ts, data FROM iot_data WHERE cmd = 'rx'")
data = cursor.fetchall()
cursor.close()
conn.close()
times = []
moisture_percentages = []
moisture = []
temperature = []

for row in data:
    try:
        # Convert Unix timestamp to datetime using datetime module
        time = datetime.fromtimestamp(row[0] / 1000)
        time = time.replace(microsecond=0)  # Remove milliseconds
        times.append(time)
    except ValueError:
        print("ValueError: Timestamp out of range")
        continue

values = [row[1] for row in data if row[1]]
numbers = [int(value, 16) for value in values]
bits = [bin(number)[2:] for number in numbers]

# Split bits into moisture and temperature
for bit_string in bits:
    for i in range(0, len(bit_string), 7):
        chunk = bit_string[i:i+7]
        if len(chunk) != 7:
            continue
        if (i // 7) % 2 == 0:
            moisture.append(chunk)
        else:
            temperature.append(chunk)

# Convert moisture bits to percentage
for bit_string in moisture:
    value = int(bit_string, 2)  # Convert bits to integer
    percentage = (value / 127) * 100  # Convert to percentage
    moisture_percentages.append(round(percentage,0))

temperature_values = []
offset = -12  # Replace with the actual offset value

for bit_string in temperature:
    integer_part = int(bit_string[:6], 2)  # Convert first 6 bits to integer
    decimal_part = 0.5 if bit_string[-1] == '1' else 0  # Convert last bit to decimal
    value = integer_part - 32 - offset + decimal_part  # Calculate real value
    temperature_values.append(value)

print(temperature_values)