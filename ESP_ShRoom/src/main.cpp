#include <Arduino.h>
#include "fan_control.h"
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
Settings settings;

Preferences preferences;

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80);


// FanControl-Instanzen für vier Lüfter
FanControl* fanControl1;
FanControl* fanControl2;
FanControl* fanControl3;
FanControl* fanControl4;

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
    loadSettings(&settings, &preferences);

    // Webserver Routen
    server.on("/", [&]() { handleRoot(&settings, server); });
    server.on("/time", [&]() { handleTime(&settings, server); });
    server.on("/setTime", [&]() { handleSetTimePage(&settings, server); });
    server.on("/updateTime", [&]() { handleSetTime(&settings, server, &preferences); });
    server.on("/set_values", [&]() { handleSetValuesPage(&settings, server); });
    server.on("/update_values", [&]() { handleSetValues(&settings, server, &preferences); });
    server.begin();
    Serial.println("Webserver gestartet");

    // FanControl-Instanzen erstellen und initialisieren
    fanControl1 = new FanControl(& settings.isEnabled1, & settings.onTime1, & settings.onPercentage1, 15);  // GPIO 25
    fanControl2 = new FanControl(& settings.isEnabled2, & settings.onTime2, & settings.onPercentage2, 2);  // GPIO 26
    fanControl3 = new FanControl(& settings.isEnabled3, & settings.onTime3, & settings.onPercentage3, 0);  // GPIO 27
    fanControl4 = new FanControl(& settings.isEnabled4, & settings.onTime4, & settings.onPercentage4, 4);  // GPIO 14

    // Initialisierung der FanControl-Instanzen
    fanControl1->initialize();
    fanControl2->initialize();
    fanControl3->initialize();
    fanControl4->initialize();
}

void loop() {
    // Sensordaten aktualisieren
    Wire.begin(21, 22); 
    temperature1 = sht20_1.temperature();
    humidity1 = sht20_1.humidity();
    Wire.begin(18, 19);  // Wechsel zu Sensor 2
    temperature2 = sht20_2.temperature();
    humidity2 = sht20_2.humidity();

    settings.temperature1 = temperature1;
    settings.temperature2 = temperature2;
    settings.humidity1 = humidity1;
    settings.humidity2 = humidity2;

    // Uhrzeit aktualisieren
    updateTime(&settings, &preferences);
    Serial.println("Temperatur 1: " + String(temperature1, 1) + " °C, Luftfeuchtigkeit 1: " + String(humidity1, 1) + " %");
    Serial.println("Temperatur 2: " + String(settings.temperature2, 1) + " °C, Luftfeuchtigkeit 2: " + String(humidity2, 1) + " %");

    // Webserver anfragen bearbeiten
    server.handleClient();
    // Aktualisiere den Lüfterstatus für alle vier Lüfter
    fanControl1->update();
    fanControl2->update();
    fanControl3->update();
    fanControl4->update();
}