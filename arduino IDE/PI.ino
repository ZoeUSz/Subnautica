#include <OneWire.h>
#include <DallasTemperature.h>
#include <Stepper.h>
#include <WiFi.h>
#include <HTTPClient.h>

// --- Wi-Fi ---
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";
const char* serverUrl = "http://192.168.57.19:5000/api/dados"; // Altere para IP do Flask

// --- Sensor de Temperatura (DS18B20) ---
const int ONE_WIRE_BUS = 5;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// --- Relés ---
const int releAquecedor = 4;
const int releFan = 2;

// --- Limites ---
const float TEMPERATURA_MINIMA = 25.0;
const float TEMPERATURA_MAXIMA = 28.0;

// --- Controle de tempo ---
unsigned long previousMillisTemp = 0;
const long intervalTemp = 5000;

unsigned long previousMillisFeeder = 0;
const long intervalFeeder = 15000;
bool feederActivated = false;
String ultimaRefeicao = "";

// --- Motor de Passo ---
int passosPorVolta = 4;
Stepper mp(passosPorVolta, 33, 25, 26, 27);

// --- Setup ---
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  
  pinMode(releAquecedor, OUTPUT);
  pinMode(releFan, OUTPUT);
  digitalWrite(releAquecedor, HIGH);
  digitalWrite(releFan, HIGH);

  sensors.begin();
  mp.setSpeed(500);
}

// --- Loop ---
void loop() {
  unsigned long currentMillis = millis();
  static float temperaturaCelsius = 0;
  static bool estadoAquecedor = false;
  static bool estadoFan = false;
  bool motorRodou = false;

  // --- Controle de Temperatura ---
  if (currentMillis - previousMillisTemp >= intervalTemp) {
    previousMillisTemp = currentMillis;

    sensors.requestTemperatures();
    temperaturaCelsius = sensors.getTempCByIndex(0);
    Serial.print("Temperatura atual: ");
    Serial.print(temperaturaCelsius);
    Serial.println(" °C");

    if (temperaturaCelsius < TEMPERATURA_MINIMA) {
      digitalWrite(releAquecedor, LOW);
      digitalWrite(releFan, HIGH);
      estadoAquecedor = true;
      estadoFan = false;
    } else if (temperaturaCelsius > TEMPERATURA_MAXIMA) {
      digitalWrite(releAquecedor, HIGH);
      digitalWrite(releFan, LOW);
      estadoAquecedor = false;
      estadoFan = true;
    } else {
      digitalWrite(releAquecedor, HIGH);
      digitalWrite(releFan, HIGH);
      estadoAquecedor = false;
      estadoFan = false;
    }
  }

  // --- Alimentador ---
  if (currentMillis - previousMillisFeeder >= intervalFeeder) {
    previousMillisFeeder = currentMillis;

    Serial.println("Acionando o alimentador...");
    for (int i = 0; i < 32; i++) {
      mp.step(passosPorVolta);
    }
    Serial.println("Alimentador acionado!");
    motorRodou = true;

    // Grava horário da refeição
    time_t now = time(nullptr);
    struct tm* tm_info = localtime(&now);
    char buffer[9]; // hh:mm:ss
    strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);
    ultimaRefeicao = String(buffer);
  }

  // --- Enviar Dados ---
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"temperatura\":" + String(temperaturaCelsius, 1) + ",";
    json += "\"rele_aquecedor\":" + String(estadoAquecedor ? "true" : "false") + ",";
    json += "\"ventoinha\":" + String(estadoFan ? "true" : "false") + ",";
    json += "\"motor\":" + String(motorRodou ? "true" : "false") + ",";
    json += "\"comida_dispensada\":" + String(motorRodou ? "true" : "false") + ",";
    json += "\"bomba_oxigenacao\":true,";
    json += "\"ultima_refeicao\":\"" + ultimaRefeicao + "\"";
    json += "}";

    int httpResponseCode = http.POST(json);
    Serial.print("POST status: ");
    Serial.println(httpResponseCode);
    http.end();
  }

  delay(1000); // leve atraso para estabilidade
}
