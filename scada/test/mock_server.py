from http.server import BaseHTTPRequestHandler, HTTPServer
import json

class TestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        self.send_response(200)
        self.send_header("Content-type", "application/json")
        self.end_headers()

        # Simulando resposta JSON com temperatura
        response = json.dumps({"temperatura": 27.3})
        self.wfile.write(response.encode())

server = HTTPServer(("localhost", 5000), TestHandler)
print("Servidor de teste rodando em http://localhost:5000")
server.serve_forever()
