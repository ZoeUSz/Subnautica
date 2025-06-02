import requests
import json

url = "http://localhost:5000/api/dados"
headers = {"Content-Type": "application/json"}

dados = {
    "temperatura": 30,
    "rele_aquecedor": False
}

resposta = requests.post(url, headers=headers, data=json.dumps(dados))

print("Status:", resposta.status_code)
print("Resposta:", resposta.json())
