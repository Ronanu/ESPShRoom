#include <Arduino.h>
#include "fan_control.h"
#include <Wire.h>
#include "uFire_SHT20.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "web_handlers.h"
#include "SHT20Sensor.h"
#include "Temperaturecontroller.h"


const int relayPin1 = 25;
const int relayPin2 = 17;
const int relayPin3 = 32;
const int relayPin4 = 33;

const int pinSens1_sda = 18;
const int pinSens1_scl = 19;
const int pinSens2_sda = 21;
const int pinSens2_scl = 22;

const char* ssid = "ESP32_AP";
const char* password = "12345678";

const unsigned long sensorInterval = 200;  // 500 ms Intervall für Sensorabfragen

// Sensor-Instanzen
SHT20Sensor sht20_1(pinSens1_sda, pinSens1_scl, Wire);  // Sensor 1 an Pin 18, 19
SHT20Sensor sht20_2(pinSens2_sda, pinSens2_scl, Wire1);  // Sensor 2 an Pin 21, 22

// Globales Settings-Objekt
Settings settings;
// Preferences-Objekt, das abgespeichert wird
Preferences preferences;

WebServer server(80);

// SpecialPwmControll-Instanzen für vier Lüfter
SpecialPwmControll* fanControl1;
SpecialPwmControll* fanControl2;
SpecialPwmControll* fanControl3;
SpecialPwmControll* fanControl4;

HysteresisController tempcontr1(&settings.targetTemperature, &settings.hysteresis);
FaultFilter faultfilter1(5);
FaultFilter faultfilter2(5);
OfflineClock offline_clock(&settings, &preferences);


// Sensor Task Zeiten
unsigned long lastSensor1Update = 0;
unsigned long lastSensor2Update = 0;


void setup() {
    Serial.begin(115200);
    offline_clock.startClock();
    Wire.setTimeout(20);  // Timeout auf 100 ms
    Wire1.setTimeout(20);  // Timeout auf 100 ms

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

    fanControl1 = new SpecialPwmControll(&settings.isEnabled1, &settings.onTime1, &settings.onPercentage1, relayPin1);
    fanControl2 = new SpecialPwmControll(&settings.isEnabled2, &settings.onTime2, &settings.onPercentage2, relayPin2);
    fanControl3 = new SpecialPwmControll(&settings.isEnabled3, &settings.onTime3, &settings.onPercentage3, relayPin3);
    fanControl4 = new SpecialPwmControll(&settings.isEnabled4, &settings.onTime4, &settings.onPercentage4, relayPin4);

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

    // Aktualisiere den Lüfterstatus für alle Ausgänge. Regler usw.

    float lazy_temperature1 = faultfilter1.checkValue(settings.temperature1);
    settings.isEnabled3 = tempcontr1.update(settings.isEnabled3, lazy_temperature1);

    float lazy_temperature2 = faultfilter2.checkValue(settings.temperature2);
    settings.isEnabled1 = true;
    if (lazy_temperature2 <= settings.targetTemperature + 1) {
        settings.onPercentage1 = 0;
    } else if (lazy_temperature2 <= settings.targetTemperature + 2) {
        settings.onPercentage1 = 50;
    } else if (lazy_temperature2 <= settings.targetTemperature + 3) {
        settings.onPercentage1 = 100;
    } else if (lazy_temperature2 > settings.targetTemperature + 4)  {
        settings.onPercentage1 = 100;
        settings.isEnabled3 = false;
    }


    fanControl1->update();
    fanControl2->update();
    fanControl3->update();
    fanControl4->update();

    Serial.println("\n\n\nAktualisierte Sensor- und Lüfterdaten");
    Serial.println("loop time = " + String(millis()-loopstart));
    printSettings(settings);
    
    delay(100);  // Kleine Pause, um die CPU zu entlasten
}