import requests
headers = {
'Content-Type': 'application/json',
'Authorization': 'Bearer vnoV5AAAABFpb3RuZXQudGVyYWNvbS5kawE_rxX1mxXoLskIDZ27kNg=',
}
data = '{ "cmd": "tx", "EUI": "A8610A34322F6A0F", "port": 1, "confirmed": false, "data": "0ffff0", "appid": "01F6452E" }'
response = requests.post('https://eu1.loriot.io/1/rest', headers=headers, data=data)
print(response)