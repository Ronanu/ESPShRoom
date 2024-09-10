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

TemperatureController tempcontr1(&settings.targetTemperature, &settings.hysteresis, &settings.isEnabled3);

// Task-Handle für den WebServer
TaskHandle_t WebServerTaskHandle;

// Mutex-Handle für die Synchronisierung
SemaphoreHandle_t settingsMutex;

// WebServer-Task
void handleWebServer(void* pvParameters) {
    for (;;) {
        // Mutex sperren, bevor auf `settings` zugegriffen wird
        if (xSemaphoreTake(settingsMutex, portMAX_DELAY)) {
            server.handleClient();  // Anfragen bearbeiten
            xSemaphoreGive(settingsMutex);  // Mutex freigeben
        }
        delay(10);  // Kurze Pause, um die CPU nicht zu überlasten
    }
}

void setup() {
    Serial.begin(115200);

    // Initialisierung der Sensoren
    if (!sht20_1.begin()) {
        Serial.println("Maximale Anzahl von Versuchen erreicht, Sensor 1 konnte nicht verbunden werden.");
    } else {
        Serial.println("Sensor 1 erfolgreich initialisiert.");
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
    server.on("/updateData", [&]() { handleUpdateData(&settings, server); });
    server.on("/update_values", [&]() { handleSetValues(&settings, server, &preferences); });
    server.begin();
    Serial.println("Webserver gestartet");

    // FanControl-Instanzen erstellen und initialisieren
    fanControl1 = new FanControl(&settings.isEnabled1, &settings.onTime1, &settings.onPercentage1, 25);
    fanControl2 = new FanControl(&settings.isEnabled2, &settings.onTime2, &settings.onPercentage2, 17);
    fanControl3 = new FanControl(&settings.isEnabled3, &settings.onTime3, &settings.onPercentage3, 32);
    fanControl4 = new FanControl(&settings.isEnabled4, &settings.onTime4, &settings.onPercentage4, 33);

    fanControl1->initialize();
    fanControl2->initialize();
    fanControl3->initialize();
    fanControl4->initialize();

    // Mutex für Settings erstellen
    settingsMutex = xSemaphoreCreateMutex();

    // WebServer-Task auf Core 0 starten
    xTaskCreatePinnedToCore(
        handleWebServer,
        "WebServerTask",
        10000,
        NULL,
        1,
        &WebServerTaskHandle,
        0  // Core 0
    );
}

void loop() {
    int start = millis();

    // Sensordaten aktualisieren
    if (xSemaphoreTake(settingsMutex, portMAX_DELAY)) {
        settings.temperature1 = sht20_1.getTemperature();
        settings.humidity1 = sht20_1.getHumidity();
        tempcontr1.update(settings.temperature1);
        xSemaphoreGive(settingsMutex);  // Mutex freigeben
    }

    // Lüfterstatus aktualisieren
    if (xSemaphoreTake(settingsMutex, portMAX_DELAY)) {
        fanControl1->update();
        fanControl2->update();
        fanControl3->update();
        fanControl4->update();
        xSemaphoreGive(settingsMutex);  // Mutex freigeben
    }

    // Zeit in der Schleife anzeigen
    Serial.println("\n\n\nLoop time: " + String(millis() - start) + " ms");
    printSettings(settings);
}