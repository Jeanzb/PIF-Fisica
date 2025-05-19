// File: src/main.cpp

#include <Arduino.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
//Utilizando el microcontrolador, los datos se adquieren con el sensor en el pin 34 en donde se recive el voltaje para despues hacer las respectivas formulas
//modificar el codigo, utilizando librerias para el manejo de la informacion para facilitar el uso de datos para la visualizacion en tiempo real
//localhoost con red local, bluethoot, conexion a la nube
const int sensorPin = 34;
const float factorConversion = 200.0;
WebServer server(80);
BluetoothSerial SerialBT;

String csvData = "ADC,Voltaje,V,Radiación,W/m²\n";

// ✅ ThingSpeak API Key - tu clave real
const char* thingspeakServer = "http://api.thingspeak.com/update";
const char* apiKey = "P79WX5MIDMYCY9JC";

void sendToThingSpeak(float voltaje, float radiacion) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = String(thingspeakServer) + "?api_key=" + apiKey +
               "&field1=" + String(voltaje, 3) +
               "&field2=" + String(radiacion, 1);
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.println("ThingSpeak -> " + String(httpCode));
  } else {
    Serial.println("Error al enviar a ThingSpeak: " + http.errorToString(httpCode));
  }
  http.end();
}

void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain", "Error cargando /index.html");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

void handleData() {
  int valor = analogRead(sensorPin);
  float voltaje = valor * (3.3 / 4095.0);
  float radiacion = voltaje * factorConversion;
  String json = "{\"adc\":" + String(valor) + ",\"v\":" + String(voltaje, 3) + ",\"r\":" + String(radiacion, 1) + "}";
  server.send(200, "application/json", json);

  csvData += String(valor) + "," + String(voltaje, 3) + "," + String(radiacion, 1) + "\n";
  sendToThingSpeak(voltaje, radiacion);
}

void handleCSVDownload() {
  server.send(200, "text/csv", csvData);
}

void startAccessPoint() {
  WiFi.softAP("Piranometro_AP", "12345678");
  IPAddress IP = WiFi.softAPIP();
  Serial.println("\nModo AP activado. Conéctate a: Piranometro_AP");
  Serial.println("Accede vía: http://" + IP.toString());
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("PiranometroESP32");
  delay(1000);
  pinMode(sensorPin, INPUT);

  if (!SPIFFS.begin(true)) {
    Serial.println("Error iniciando SPIFFS");
    return;
  }

  WiFi.begin("prueba", "cachofeliz");
  Serial.print("Conectando a Wi-Fi");
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nError al conectar. Iniciando modo Access Point.");
    startAccessPoint();
  }

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/csv", handleCSVDownload);
  server.begin();
}

void loop() {
  server.handleClient();
  int valor = analogRead(sensorPin);
  float voltaje = valor * (3.3 / 4095.0);
  float radiacion = voltaje * factorConversion;
  String btMsg = "ADC: " + String(valor) + ", Voltaje: " + String(voltaje, 3) + " V, Radiación: " + String(radiacion, 1) + " W/m²\n";
  SerialBT.print(btMsg);
  delay(1000);
}
