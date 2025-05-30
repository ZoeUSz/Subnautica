from flask import Flask, request, render_template, jsonify
import json
import os
from datetime import datetime

app = Flask(__name__)
ARQUIVO = "dados.json"

# Garante que o arquivo exista e seja uma lista vazia se estiver vazio
def inicializar_arquivo_json():
    if not os.path.exists(ARQUIVO) or os.stat(ARQUIVO).st_size == 0:
        with open(ARQUIVO, "w") as f:
            json.dump([], f) # Inicia como uma lista vazia

@app.route("/", methods=["GET"])
def index():
    try:
        with open(ARQUIVO, "r") as f:
            lista_dados = json.load(f)
            if lista_dados:
                dados = lista_dados[-1]  # Última leitura
            else:
                dados = {
                    "temperatura": "N/A",
                    "ultima_refeicao": "N/A",
                    "proxima_refeicao": "N/A"
                }
    except Exception as e:
        dados = {
            "temperatura": "Erro",
            "ultima_refeicao": "Erro",
            "proxima_refeicao": "Erro"
        }
        print("Erro ao carregar JSON:", e)

    return render_template("index.html", dados=dados)

@app.route("/api/dados", methods=["POST"])
def receber_dados():
    inicializar_arquivo_json() # Garante que o arquivo exista
    nova_leitura = request.get_json()

    if not nova_leitura or "temperatura" not in nova_leitura:
        return jsonify({"status": "erro", "mensagem": "Dados inválidos. 'temperatura' é obrigatório."}), 400

    # Adiciona um timestamp à nova leitura
    nova_leitura["timestamp"] = datetime.now().isoformat()

    try:
        with open(ARQUIVO, "r+") as f: # Abre para leitura e escrita
            conteudo_atual = json.load(f)
            conteudo_atual.append(nova_leitura) # Adiciona a nova leitura à lista
            f.seek(0) # Volta para o início do arquivo
            json.dump(conteudo_atual, f, indent=4) # Salva a lista atualizada
            f.truncate() # Remove qualquer conteúdo extra se o novo for menor
        return jsonify({"status": "ok", "mensagem": "Leitura adicionada!"}), 201 # 201 Created
    except Exception as e:
        return jsonify({"status": "erro", "mensagem": f"Erro ao salvar dados: {str(e)}"}), 500

@app.route("/api/dados", methods=["GET"])
def ver_dados():
    inicializar_arquivo_json() # Garante que o arquivo exista
    with open(ARQUIVO, "r") as f:
        dados = json.load(f)
    return jsonify(dados)

if __name__ == "__main__":
    inicializar_arquivo_json() # Garante que o arquivo exista ao iniciar a aplicação
    app.run(host="0.0.0.0", port=5000)