# Read the text file
with open('downlink.txt', 'r') as file:
    data = file.read()

# Convert the string to bytes
bytes_object = data.encode()

# Convert the bytes to hexadecimal
hex_string = bytes_object.hex()

print(hex_string)