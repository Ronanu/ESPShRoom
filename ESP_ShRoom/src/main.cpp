#include <Arduino.h>
#include "fan_control.h"
#include <Wire.h>
#include "uFire_SHT20.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "web_handlers.h"
#include "handle_root.h"
#include "SHT20Sensor.h"
#include "Temperaturecontroller.h"

// Sensor-Instanzen
SHT20Sensor sht20_1(18, 19, Wire);  // Sensor 1 an Pin 18, 19
SHT20Sensor sht20_2(21, 22, Wire1);  // Sensor 2 an Pin 21, 22

// Globales Settings-Objekt
Settings settings;
// Preferences-Objekt, das abgespeichert wird
Preferences preferences;

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80);

// FanControl-Instanzen für vier Lüfter
FanControl* fanControl1;
FanControl* fanControl2;
FanControl* fanControl3;
FanControl* fanControl4;

TemperatureController tempcontr1(&settings.targetTemperature, &settings.hysteresis);
ValueChecker valuechecker(5);
OfflineClock offline_clock(&settings, &preferences);


// Sensor Task Zeiten
unsigned long lastSensor1Update = 0;
unsigned long lastSensor2Update = 0;
const unsigned long sensorInterval = 500;  // 500 ms Intervall für Sensorabfragen

void setup() {
    Serial.begin(115200);
    offline_clock.startClock();
    Wire.setTimeout(100);  // Timeout auf 100 ms
    Wire1.setTimeout(100);  // Timeout auf 100 ms

    // Initialisierung der Sensoren
    if (!sht20_1.begin()) {
        Serial.println("Maximale Anzahl von Versuchen erreicht, Sensor 1 konnte nicht verbunden werden.");
    } else {
        Serial.println("Sensor 1 erfolgreich initialisiert.");
    }

    if (!sht20_2.begin()) {
        Serial.println("Maximale Anzahl von Versuchen erreicht, Sensor 2 konnte nicht verbunden werden.");
    } else {
        Serial.println("Sensor 2 erfolgreich initialisiert.");
    }

    // Konfiguration des ESP32 als Access Point
    WiFi.softAP(ssid, password);
    Serial.println("Access Point gestartet");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", [&]() { handleRoot(&settings, server); });
    server.on("/setTime", [&]() { handleSetTimePage(&settings, server); });
    server.on("/set_values", [&]() { handleSetValuesPage(&settings, server); });
    server.on("/updateData", [&]() { handleUpdateData(&settings, server); });
    server.on("/updateTime", [&]() { handleSetTime(&settings, server, &preferences); });
    server.on("/update_values", [&]() { handleSetValues(&settings, server, &preferences); });
    server.begin();
    Serial.println("Webserver gestartet");

    fanControl1 = new FanControl(&settings.isEnabled1, &settings.onTime1, &settings.onPercentage1, 25);
    fanControl2 = new FanControl(&settings.isEnabled2, &settings.onTime2, &settings.onPercentage2, 17);
    fanControl3 = new FanControl(&settings.isEnabled3, &settings.onTime3, &settings.onPercentage3, 32);
    fanControl4 = new FanControl(&settings.isEnabled4, &settings.onTime4, &settings.onPercentage4, 33);

    fanControl1->initialize();
    fanControl2->initialize();
    fanControl3->initialize();
    fanControl4->initialize();
}

void loop() {
    // Webserver anfragen bearbeiten
    server.handleClient();

    unsigned long loopstart = millis();
    settings.lastUpdateTime = loopstart;

    // Überprüfe, ob genug Zeit seit der letzten Sensor 1-Abfrage vergangen ist
    if (loopstart - lastSensor1Update >= sensorInterval) {
        // Sensor 1-Daten aktualisieren
        settings.temperature1 = sht20_1.getTemperature();
        settings.humidity1 = sht20_1.getHumidity();
        lastSensor1Update = loopstart;  // Zeit der letzten Aktualisierung speichern
        Serial.println("Sensor 1 Daten aktualisiert");
    }

    // Überprüfe, ob genug Zeit seit der letzten Sensor 2-Abfrage vergangen ist
    if (loopstart - lastSensor2Update >= sensorInterval) {
        // Sensor 2-Daten aktualisieren
        settings.temperature2 = sht20_2.getTemperature();
        settings.humidity2 = sht20_2.getHumidity();
        lastSensor2Update = loopstart;  // Zeit der letzten Aktualisierung speichern
        Serial.println("Sensor 2 Daten aktualisiert");
    }

    // Uhrzeit aktualisieren
    offline_clock.updateTime();

    // Aktualisiere den Lüfterstatus für alle vier Lüfter

    float lazy_temperature = valuechecker.checkValue(settings.temperature1);
    settings.isEnabled3 = tempcontr1.update(settings.isEnabled3, lazy_temperature);

    fanControl1->update();
    fanControl2->update();
    fanControl3->update();
    fanControl4->update();

    Serial.println("\n\n\nAktualisierte Sensor- und Lüfterdaten");
    Serial.println("loop time = " + String(millis()-loopstart));
    printSettings(settings);
    
    delay(100);  // Kleine Pause, um die CPU zu entlasten
}