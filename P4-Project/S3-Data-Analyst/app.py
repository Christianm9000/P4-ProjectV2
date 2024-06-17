from flask import Flask, render_template
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import pymysql, json
import pandas as pd
from datetime import datetime, timedelta

# Initialize the Flask application
app = Flask(__name__)

# Offset for temperature calculation
offset = -12

# Load database configuration from a JSON file
with open('config.json', 'r') as file:
    config = json.load(file)

# MySQL database connection details
db = config['database']
conn = pymysql.connect(host=db['host'], user=db['username'], passwd=db['password'], db=db['dbname'])

@app.route('/', methods=['GET', 'POST'])
def home():
    # Create a cursor object to interact with the database
    cursor = conn.cursor()
    
    # Execute SQL query to fetch timestamp and data where command is 'rx'
    cursor.execute("SELECT ts, data FROM iot_data WHERE cmd = 'rx'")
    data = cursor.fetchall()
    
    # Close the database connection
    conn.close()

    # Process the fetched data
    # Convert timestamps from milliseconds to datetime objects, removing microseconds
    times = [datetime.fromtimestamp(row[0] / 1000).replace(microsecond=0) for row in data if row[0]]
    
    # Extract data values
    values = [row[1] for row in data if row[1]]
    
    # Convert hexadecimal data to binary strings
    bits = [bin(int(value, 16))[2:].zfill(len(value) * 4) for value in values]

    # Initialize list to store timestamps for each measurement
    timestamps = []
    
    # Calculate timestamps for each 30-minute interval measurement
    for time, bit_string in zip(times, bits):
        number_of_measurements = len(bit_string) // 14
        timestamps.extend([time - timedelta(minutes=30 * i) for i in range(number_of_measurements)])
    
    # Extract moisture and temperature bit strings
    moisture = [bit_string[i:i + 7] for bit_string in bits for i in range(0, len(bit_string), 7) if len(bit_string[i:i + 7]) == 7 and (i // 7) % 2 == 0]
    temperature = [bit_string[i:i + 7] for bit_string in bits for i in range(0, len(bit_string), 7) if len(bit_string[i:i + 7]) == 7 and (i // 7) % 2 != 0]

    # Convert moisture data to percentage
    moisture_percentages = [str(round((int(bit_string, 2) / 127) * 100, 0)) + '%' for bit_string in moisture]
    
    # Convert temperature data to actual values
    temperature_values = [int(bit_string[:6], 2) - 32 - (offset) + (0.5 if bit_string[-1] == '1' else 0) for bit_string in temperature]

    # Create a Plotly subplot for visualization
    fig = make_subplots(rows=1, cols=1, specs=[[{'secondary_y': True}]])
    
    # Add moisture percentage data trace
    fig.add_trace(go.Scatter(x=timestamps, y=moisture_percentages, mode='lines', marker=dict(color='green'), name='Moisture Percentage'), row=1, col=1, secondary_y=False)
    
    # Add temperature data trace
    fig.add_trace(go.Scatter(x=timestamps, y=temperature_values, mode='lines', marker=dict(color='orange'), name='Temperature'), row=1, col=1, secondary_y=True)
    
    # Set y-axis titles
    fig.update_yaxes(title_text='Moisture Percentage', secondary_y=False)
    fig.update_yaxes(title_text='Temperature', secondary_y=True)
    
    # Set chart layout title and x-axis title
    fig.update_layout(title='IOT DEVICE', xaxis_title='Time')

    # Limit x-axis range to the last 7 days
    last_7_days = pd.Timestamp.now() - pd.DateOffset(days=1)
    fig.update_xaxes(range=[last_7_days, pd.Timestamp.now()])
    
    # Convert Plotly figure to HTML
    chart = fig.to_html(full_html=False)

    # Render the HTML template with the chart
    return render_template('index.html', chart=chart)

if __name__ == '__main__':
    from waitress import serve
    # Run the app with waitress server for production use
    #serve(app, host="0.0.0.0", port=80)
    
    # Run the app with Flask's built-in server for development use
    app.run(host='0.0.0.0', port=5001, debug=True)
