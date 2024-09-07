#include <Wire.h>
#include "uFire_SHT20.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "web_handlers.h"

// Sensor-Instanzen
uFire_SHT20 sht20_1;  // Sensor 1 an Pin 21, 22
uFire_SHT20 sht20_2;  // Sensor 2 an Pin 18, 19

// Sensordaten
float temperature1;
float temperature2;
float humidity1;
float humidity2;

// Globales Settings-Objekt
Settings currentSettings;

Preferences preferences;

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80);

void setup() {
    Serial.begin(115200);

    // Initialisierung der Sensoren
    Wire.begin(21, 22);  // SDA, SCL für Sensor 1
    if (!sht20_1.begin()) {
        Serial.println("Sensor 1 nicht gefunden, überprüfe die Verkabelung!");
        while (1);
    }

    Wire.begin(18, 19);  // SDA, SCL für Sensor 2
    if (!sht20_2.begin()) {
        Serial.println("Sensor 2 nicht gefunden, überprüfe die Verkabelung!");
        while (1);
    }

    // Konfiguration des ESP32 als Access Point
    WiFi.softAP(ssid, password);
    Serial.println("Access Point gestartet");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.softAPIP());

    // NVS Initialisierung und Werte laden
    preferences.begin("my-app", false);
    loadSettings(&currentSettings, &preferences);

    // Webserver Routen
    server.on("/", [&]() { handleRoot(&currentSettings, server); });
    server.on("/time", [&]() { handleTime(&currentSettings, server); });
    server.on("/setTime", [&]() { handleSetTimePage(&currentSettings, server); });
    server.on("/updateTime", [&]() { handleSetTime(&currentSettings, server, &preferences); });
    server.on("/set_values", [&]() { handleSetValuesPage(&currentSettings, server); });
    server.on("/update_values", [&]() { handleSetValues(&currentSettings, server, &preferences); });
    server.begin();
    Serial.println("Webserver gestartet");
}

void loop() {
    // Sensordaten aktualisieren
    Wire.begin(21, 22); 
    temperature1 = sht20_1.temperature();
    humidity1 = sht20_1.humidity();
    Wire.begin(18, 19);  // Wechsel zu Sensor 2
    temperature2 = sht20_2.temperature();
    humidity2 = sht20_2.humidity();

    currentSettings.temperature1 = temperature1;
    currentSettings.temperature2 = temperature2;
    currentSettings.humidity1 = humidity1;
    currentSettings.humidity2 = humidity2;

    // Uhrzeit aktualisieren
    updateTime(&currentSettings, &preferences);
    Serial.println("Temperatur 1: " + String(temperature1, 1) + " °C, Luftfeuchtigkeit 1: " + String(humidity1, 1) + " %");
    Serial.println("Temperatur 2: " + String(currentSettings.temperature2, 1) + " °C, Luftfeuchtigkeit 2: " + String(humidity2, 1) + " %");

    // Webserver anfragen bearbeiten
    server.handleClient();
}
