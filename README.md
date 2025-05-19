# 🌞 Sistema de Medición de Radiación Solar con ESP32

Este proyecto corresponde a una investigación formativa en el curso de Física II: Electricidad y Magnetismo. Consiste en el diseño e implementación de un sistema basado en el microcontrolador ESP32 que permite la **visualización en tiempo real** de niveles de radiación solar (irradiancia) mediante un servidor web local, comunicación Bluetooth y conectividad con la nube vía ThingSpeak.

> ⚠️ **Nota:** Este sistema aún no ha sido conectado a un piranómetro real. Los datos actuales provienen de entradas analógicas simuladas para validar el funcionamiento del sistema.

---

## 📋 Características del Proyecto

- 📡 Lectura analógica en pin GPIO34.
- 🌐 Servidor web embebido para monitoreo local.
- 📤 Envío de datos a ThingSpeak usando HTTP.
- 🔊 Transmisión de datos por Bluetooth.
- 📁 Exportación de datos en formato CSV.
- 🔋 Funciona alimentado por batería (modo portátil).
- 🔌 Modo Access Point automático si no hay WiFi.

---

## 🧰 Tecnologías utilizadas

- Plataforma: [PlatformIO](https://platformio.org/)
- Lenguaje: C++
- Microcontrolador: ESP32
- Librerías:
  - `WiFi.h`
  - `WebServer.h`
  - `SPIFFS.h`
  - `BluetoothSerial.h`
  - `HTTPClient.h`

---

## 📦 Estructura del proyecto

