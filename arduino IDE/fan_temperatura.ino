#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Pino do ESP32 para o sinal do sensor DS18B20
const int ONE_WIRE_BUS = 5; // Pino GPIO onde o DS18B20 está conectado

// Pinos do ESP32 para os relés
const int releAquecedor = 4; // Pino GPIO para o relé do aquecedor
const int releFan = 2;       // Pino GPIO para o relé da ventoinha

// Limites de temperatura
const float TEMPERATURA_MINIMA = 25.0; // Temperatura mínima desejada para ligar o aquecedor
const float TEMPERATURA_MAXIMA = 28.0; // Temperatura máxima desejada para ligar a ventoinha

// Configuração do sensor DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Wi-Fi e servidor
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";
const char* serverUrl = "http://192.168.57.19:5000/api/dados";

unsigned long ultimaTransmissao = 0;
const long intervalo = 10000; // 10 segundos

void setup() {
  // Inicia a comunicação serial para debug
  Serial.begin(115200);
  Serial.println("Iniciando o sistema de controle de temperatura do aquario...");

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado!");

  // Inicializa os pinos dos relés como saída
  pinMode(releAquecedor, OUTPUT);
  pinMode(releFan, OUTPUT);

  // Garante que ambos estão desligados no início
  digitalWrite(releAquecedor, HIGH); // HIGH geralmente desliga o relé (ativo baixo)
  digitalWrite(releFan, HIGH);       // HIGH geralmente desliga o relé (ativo baixo)

  // Inicia o sensor de temperatura
  sensors.begin();
  Serial.println("Sensor DS18B20 inicializado.");
}

void loop() {
  // Solicita a leitura da temperatura
  sensors.requestTemperatures();
  float temperaturaCelsius = sensors.getTempCByIndex(0); // Lê a temperatura do primeiro sensor

  // Imprime a temperatura no Monitor Serial
  Serial.print("Temperatura atual: ");
  Serial.print(temperaturaCelsius);
  Serial.println(" °C");

  // Lógica de controle:
  bool statusRele = false;

  if (temperaturaCelsius < TEMPERATURA_MINIMA) {
    digitalWrite(releAquecedor, LOW); // LOW geralmente liga o relé (ativo baixo)
    digitalWrite(releFan, HIGH);      // HIGH geralmente desliga o relé
    statusRele = true;
    Serial.println("Temperatura baixa. Aquecedor LIGADO!");
  } else if (temperaturaCelsius > TEMPERATURA_MAXIMA) {
    digitalWrite(releAquecedor, HIGH); // HIGH geralmente desliga o relé
    digitalWrite(releFan, LOW);       // LOW geralmente liga o relé (ativo baixo)
    statusRele = false;
    Serial.println("Temperatura alta. Fan LIGADA!");
  } else {
    digitalWrite(releAquecedor, HIGH); // HIGH geralmente desliga o relé
    digitalWrite(releFan, HIGH);      // HIGH geralmente desliga o relé
    statusRele = false;
    Serial.println("Temperatura ideal. Aquecedor e Fan DESLIGADOS.");
  }

  // Envio de dados para o servidor Flask
  if (millis() - ultimaTransmissao >= intervalo) {
    ultimaTransmissao = millis();
    enviarDados(temperaturaCelsius, statusRele);
  }

  // Pequeno atraso entre as leituras para não sobrecarregar o sistema
  delay(5000); // Espera 5 segundos antes da próxima leitura
}

void enviarDados(float temperatura, bool releLigado) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"temperatura\": " + String(temperatura, 2) + ",";
    json += "\"rele_aquecedor\": " + String(releLigado ? "true" : "false");
    json += "}";

    int httpResponseCode = http.POST(json);
    Serial.print("POST status: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi desconectado, não foi possível enviar os dados.");
  }
}
