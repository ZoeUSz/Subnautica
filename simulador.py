import requests
import json
from datetime import datetime


url = "http://localhost:5000/api/dados"
headers = {"Content-Type": "application/json"}

dados = {
    "temperatura": 27.8,
    "rele_aquecedor": True,
    "ventoinha": False,
    "motor": True,
    "comida_dispensada": True,
    "bomba_oxigenacao": True,
    "ultima_refeicao": datetime.now().strftime("%Y-%m-%d %H:%M:%S")  # adiciona horário atual
}

resposta = requests.post(url, headers=headers, data=json.dumps(dados))

print("Status:", resposta.status_code)
print("Resposta:", resposta.json())
