import requests
import json

url = "http://localhost:5000/api/dados"
headers = {"Content-Type": "application/json"}

dados = {
    "temperatura": 12,
    "ultima_refeicao": "2025-05-27 10:00:00",
    "proxima_refeicao": "2025-05-27 18:00:00"
}

resposta = requests.post(url, headers=headers, data=json.dumps(dados))

print("Status:", resposta.status_code)
print("Resposta:", resposta.json())
