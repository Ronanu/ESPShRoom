#include "SHT20Sensor.h"

// Instanz für Sensor 1 mit Standard-I²C-Bus
SHT20Sensor sensor1(22, 21, Wire);

// Instanz für Sensor 2 mit zweitem I²C-Bus (Wire1)
SHT20Sensor sensor2(18, 19, Wire1);

void setup() {
    Serial.begin(9600);

    if (!sensor1.begin()) {
        Serial.println("Sensor 1 konnte nicht verbunden werden.");
    }

    if (!sensor2.begin()) {
        Serial.println("Sensor 2 konnte nicht verbunden werden.");
    }
}

void loop() {
    float temperature1 = sensor1.getTemperature();
    float humidity1 = sensor1.getHumidity();

    float temperature2 = sensor2.getTemperature();
    float humidity2 = sensor2.getHumidity();

    Serial.print("Sensor 1 - Temperatur: ");
    Serial.println(temperature1);
    Serial.print("Sensor 1 - Luftfeuchtigkeit: ");
    Serial.println(humidity1);

    Serial.print("Sensor 2 - Temperatur: ");
    Serial.println(temperature2);
    Serial.print("Sensor 2 - Luftfeuchtigkeit: ");
    Serial.println(humidity2);

    delay(1000);  // Warte 1 Sekunde
}