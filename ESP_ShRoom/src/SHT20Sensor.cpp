#include "SHT20Sensor.h"

// Konstruktor
SHT20Sensor::SHT20Sensor(uint8_t sdaPin, uint8_t sclPin, TwoWire &wireBus)
    : _sdaPin(sdaPin), _sclPin(sclPin), _wireBus(wireBus) {
}

// Methode zum Initialisieren der Sensoren
bool SHT20Sensor::begin() {
    _wireBus.begin(_sdaPin, _sclPin, 9600);
    
    // Initialisiere den uFire SHT20 Sensor auf dem entsprechenden I²C-Bus
    for (int i = 0; i < 5 && !sht20.begin(&_wireBus); i++) {
        Serial.println("Sensor nicht gefunden, überprüfe die Verkabelung!");
        delay(500);  // Warte eine halbe Sekunde
    }
    
    return sht20.begin(&_wireBus);
}

// Methode zum Abrufen der Temperatur
float SHT20Sensor::getTemperature() {
    return sht20.temperature();  // Temperatur aus der uFire_SHT20-Bibliothek abfragen
}

// Methode zum Abrufen der Luftfeuchtigkeit
float SHT20Sensor::getHumidity() {
    return sht20.humidity();  // Luftfeuchtigkeit aus der uFire_SHT20-Bibliothek abfragen
}