import mysql.connector
import json

# Load the database configuration from the JSON file
with open('config.json') as f:
    config = json.load(f)['database']

# Connect to the MySQL database
cnx = mysql.connector.connect(user=config['username'], password=config['password'],
                              host=config['host'],
                              database=config['dbname'])

# Create a cursor object
cursor = cnx.cursor()

# Execute the SQL query
# Execute the SQL query
query = ("SELECT data FROM iot_data WHERE data IS NOT NULL")
cursor.execute(query)
# Fetch the results
for (data,) in cursor:
    print(data)
    try:
        # Interpret the data as an integer
        number = int(data, 16)
        # Convert the number to a binary string
        bits = bin(number)[2:]  # [2:] to remove the '0b' prefix
        print(len(bits))
    except ValueError:
        print("Cannot decode data: ", data)

# Close the cursor and connection
cursor.close()
cnx.close()