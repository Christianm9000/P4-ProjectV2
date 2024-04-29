import json

# Read the JSON file
with open('downlink.json', 'r') as file:
    data = json.load(file)

# Convert the JSON object to a string
json_string = json.dumps(data)

# Convert the string to bytes
bytes_object = json_string.encode()

# Convert the bytes to hexadecimal
hex_string = bytes_object.hex()

print(hex_string)