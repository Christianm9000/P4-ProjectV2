import pymysql, json
from datetime import datetime

with open('config.json', 'r') as file:
    config = json.load(file)

offset= -12

db = config['database']
conn = pymysql.connect(host=db['host'], user=db['username'], passwd=db['password'], db=db['dbname'])
cursor = conn.cursor()
cursor.execute("SELECT ts, data FROM iot_data WHERE cmd = 'rx'")
data = cursor.fetchall()
conn.close()

times = [datetime.fromtimestamp(row[0] / 1000).replace(microsecond=0) for row in data if row[0]]
values = [row[1] for row in data if row[1]]
numbers = [int(value, 16) for value in values]
bits = [bin(number)[2:] for number in numbers]

moisture = [bit_string[i:i+7] for bit_string in bits for i in range(0, len(bit_string), 7) if len(bit_string[i:i+7]) == 7 and (i // 7) % 2 == 0]
temperature = [bit_string[i:i+7] for bit_string in bits for i in range(0, len(bit_string), 7) if len(bit_string[i:i+7]) == 7 and (i // 7) % 2 != 0]

moisture_percentages = [str(round((int(bit_string, 2) / 127) * 100, 0)) + '%' for bit_string in moisture]
temperature_values = [int(bit_string[:6], 2) - 32 - (offset) + (0.5 if bit_string[-1] == '1' else 0) for bit_string in temperature]

print(temperature_values)
print(moisture_percentages)
print(times)