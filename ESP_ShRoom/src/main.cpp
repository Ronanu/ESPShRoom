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
SensorData sensordata;
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
OfflineClock offline_clock(&settings, &preferences);

// Mutex to protect shared data
portMUX_TYPE sensorDataMutex = portMUX_INITIALIZER_UNLOCKED;

void sensorTask(void* parameter) {
    while (true) {
        // Protect the sensor data updates with a mutex
        portENTER_CRITICAL(&sensorDataMutex);
        sensordata.temperature1 = sht20_1.getTemperature();
        sensordata.temperature2 = sht20_2.getTemperature();
        sensordata.humidity1 = sht20_1.getHumidity();
        sensordata.humidity2 = sht20_2.getHumidity();
        portEXIT_CRITICAL(&sensorDataMutex);

        vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay for 1 second
    }
}

void setup() {
    Serial.begin(115200);
    offline_clock.startClock();
    Wire.setTimeout(50);  // Setze das Timeout auf 50ms, um lange Wartezeiten zu vermeiden
    Wire1.setTimeout(50);  // Setze das Timeout auf 50ms, um lange Wartezeiten zu vermeiden
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

    // Create sensor task on core 0
    xTaskCreatePinnedToCore(
        sensorTask,   // Task function
        "SensorTask", // Name of the task
        10000,        // Stack size (in bytes)
        NULL,         // Task input parameter
        1,            // Priority of the task
        NULL,         // Task handle
        0             // Pin to core 0
    );
}

void loop() {
    // Webserver anfragen bearbeiten
    server.handleClient();

    unsigned long start = millis();
    settings.lastUpdateTime = start;

    // Protect reading shared data with a mutex
    portENTER_CRITICAL(&sensorDataMutex);
    settings.temperature1 = sensordata.temperature1;
    settings.temperature2 = sensordata.temperature2;
    settings.humidity1 = sensordata.humidity1;
    settings.humidity2 = sensordata.humidity2;
    portEXIT_CRITICAL(&sensorDataMutex);

    // Uhrzeit aktualisieren
    offline_clock.updateTime();

    settings.isEnabled3 = tempcontr1.update(settings.isEnabled3, settings.temperature1);

    // Aktualisiere den Lüfterstatus für alle vier Lüfter
    fanControl1->update();
    fanControl2->update();
    fanControl3->update();
    fanControl4->update();

    Serial.println("\n\n\nLoop time: " + String(millis() - start) + " ms");
    printSettings(settings);
}