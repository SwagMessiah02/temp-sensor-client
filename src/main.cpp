#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_AHTX0.h>

// Configurações da rede Wi-Fi
const char* ssid     = "NOME DA REDE";     // Substitua pelo SSID do seu Wi-Fi
const char* password = "SENHA DO WIFI";    // Substitua pela senha

// Configurações do servidor TCP
const char* host = "IP DO SERVIDOR";  // IP do servidor 
const uint16_t port = 5050;          // Porta do servidor
bool connected = false;

WiFiClient client;
Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando sensor AHT10...");

  delay(2000);

  if (!aht.begin()) {
    Serial.println("Falha ao inicializar AHT10/AHT20, verifique conexões!");
    while (1) delay(10);
  }

  Serial.println("AHT10 detectado com sucesso!");

  Serial.println();
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Conectando ao servidor %s:%d ...\n", host, port);

  if (client.connect(host, port)) {
    Serial.println("Conectado ao servidor!");
    connected = true;
  } else {
    Serial.println("Erro ao tentar se conectar ao servidor.");
  }
}

void loop() {
  if(connected) {
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);

    char buffer[43];

    snprintf(buffer, sizeof(buffer), 
            "Temperatura: %.2lf °C Umidade: %.2lf %%", temp.temperature, humidity.relative_humidity);

    String mensagem = String(buffer);

    client.print(mensagem);
  }

  delay(1000);
}