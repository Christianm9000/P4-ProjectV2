from flask import Flask, render_template
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import pymysql, json
import pandas as pd
from datetime import datetime, timedelta

app = Flask(__name__)
offset = -12
with open('config.json', 'r') as file:
    config = json.load(file)
# MySQL database connection details
db = config['database']
conn = pymysql.connect(host=db['host'], user=db['username'], passwd=db['password'], db=db['dbname'])

@app.route('/', methods=['GET', 'POST'])
def home():
    cursor = conn.cursor()
    cursor.execute("SELECT ts, data FROM iot_data WHERE cmd = 'rx'")
    data = cursor.fetchall()
    conn.close()
    times = [datetime.fromtimestamp(row[0] / 1000).replace(microsecond=0) for row in data if row[0]]
    values = [row[1] for row in data if row[1]]
    bits = [bin(int(value, 16))[2:].zfill(len(value)*4) for value in values]
    timestamps = []
    for time, bit_string in zip(times, bits):
        number_of_measurements = len(bit_string) // 14
        timestamps.extend([time - timedelta(minutes=30*i) for i in range(number_of_measurements)])
    moisture = [bit_string[i:i+7] for bit_string in bits for i in range(0, len(bit_string), 7) if len(bit_string[i:i+7]) == 7 and (i // 7) % 2 == 0]
    temperature = [bit_string[i:i+7] for bit_string in bits for i in range(0, len(bit_string), 7) if len(bit_string[i:i+7]) == 7 and (i // 7) % 2 != 0]

    moisture_percentages = [str(round((int(bit_string, 2) / 127) * 100, 0)) + '%' for bit_string in moisture]
    temperature_values = [int(bit_string[:6], 2) - 32 - (offset) + (0.5 if bit_string[-1] == '1' else 0) for bit_string in temperature]
    fig = make_subplots(rows=1, cols=1, specs=[[{'secondary_y': True}]])
    fig.add_trace(go.Scatter(x=timestamps, y=moisture_percentages, mode='lines', marker=dict(color='green'), name='Moisture Percentage'), row=1, col=1, secondary_y=False)
    fig.add_trace(go.Scatter(x=timestamps, y=temperature_values, mode='lines', marker=dict(color='orange'), name='Temperature'), row=1, col=1, secondary_y=True)
    fig.update_yaxes(title_text='Moisture Percentage', secondary_y=False)
    fig.update_yaxes(title_text='Temperature', secondary_y=True)
    fig.update_layout(title='IOT DEVICE', xaxis_title='Time')


    last_7_days = pd.Timestamp.now() - pd.DateOffset(days=1)
    fig.update_xaxes(range=[last_7_days, pd.Timestamp.now()])
    chart = fig.to_html(full_html=False)

    return render_template('index.html', chart=chart)


if __name__ == '__main__':
    from waitress import serve
    #serve(app, host="0.0.0.0", port=80)
    app.run(host='0.0.0.0', port=5001, debug=True)