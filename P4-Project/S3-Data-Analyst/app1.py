from flask import Flask, render_template
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import random
from datetime import datetime, timedelta

app = Flask(__name__)
offset = -12

# Function to generate data with constraints
def generate_data(previous_moisture, previous_temperature):
    # Generate moisture within 5% of previous measurement
    max_moisture_change = 5
    moisture_change = random.uniform(-max_moisture_change, max_moisture_change)
    new_moisture = max(0, min(100, previous_moisture + moisture_change))

    # Generate temperature within 2 degrees of previous measurement
    max_temp_change = 2
    temp_change = random.uniform(-max_temp_change, max_temp_change)
    new_temp = max(-20, min(43.5, previous_temperature + temp_change))

    return new_moisture, new_temp

@app.route('/', methods=['GET', 'POST'])
def home():
    # Generate initial values
    previous_moisture = 50  # Initial moisture
    previous_temperature = 20  # Initial temperature

    timestamps = [datetime.now() - timedelta(minutes=i * 30) for i in range(20)]  # Generate timestamps

    moisture_values = []
    temperature_values = []

    for timestamp in timestamps:
        new_moisture, new_temp = generate_data(previous_moisture, previous_temperature)
        moisture_values.append(new_moisture)
        temperature_values.append(new_temp)
        previous_moisture = new_moisture
        previous_temperature = new_temp

    # Plot data
    fig = make_subplots(rows=1, cols=1, specs=[[{'secondary_y': True}]])
    fig.add_trace(go.Scatter(x=timestamps, y=moisture_values, mode='lines', marker=dict(color='green'), name='Moisture Percentage'), row=1, col=1, secondary_y=False)
    fig.add_trace(go.Scatter(x=timestamps, y=temperature_values, mode='lines', marker=dict(color='orange'), name='Temperature'), row=1, col=1, secondary_y=True)
    fig.update_yaxes(title_text='Moisture Percentage', secondary_y=False)
    fig.update_yaxes(title_text='Temperature', secondary_y=True)
    fig.update_layout(title='IOT DEVICE', xaxis_title='Time')

    # Convert plot to HTML
    chart = fig.to_html(full_html=False)

    return render_template('index.html', chart=chart)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=True)
