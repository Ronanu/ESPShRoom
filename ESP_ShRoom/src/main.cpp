#include <Arduino.h>
#include <Wire.h>
#include "SHT20Sensor.h"

// Sensor-Instanz 21, 22 ; 18, 19
SHT20Sensor sensor1(21, 22, Wire);  // Sensor 1 an Pin 21 (SDA) und 22 (SCL)

void setup() {
    Serial.begin(115200);

    // Initialisierung des Sensors
    if (!sensor1.begin()) {
        Serial.println("Maximale Anzahl von Versuchen erreicht, Sensor 1 konnte nicht verbunden werden.");
    } else {
        Serial.println("Sensor 1 erfolgreich initialisiert.");
    }
}

void loop() {
    // Sensordaten abrufen
    float temperature = sensor1.getTemperature();
    float humidity = sensor1.getHumidity();

    // Sensordaten auf der Konsole ausgeben
    Serial.print("Temperatur: ");
    Serial.print(temperature, 1);
    Serial.println(" °C");

    Serial.print("Luftfeuchtigkeit: ");
    Serial.print(humidity, 1);
    Serial.println(" %");

    // Eine Sekunde warten
    delay(1000);
}