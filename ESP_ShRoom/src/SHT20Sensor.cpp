#include "SHT20Sensor.h"

// Konstruktor
SHT20Sensor::SHT20Sensor(uint8_t sdaPin, uint8_t sclPin, TwoWire &wireBus)
    : _sdaPin(sdaPin), _sclPin(sclPin), _wireBus(wireBus) {
}

// Methode zum Initialisieren der Sensoren
bool SHT20Sensor::begin() {
    _wireBus.begin(_sdaPin, _sclPin, 9600);
    
    // Initialisiere den uFire SHT20 Sensor auf dem entsprechenden I²C-Bus
    for (int i = 0; i < 2 && !sht20.begin(SHT20_RESOLUTION_12BITS, SHT20_I2C, _wireBus); i++) {
        Serial.println("Sensor nicht gefunden, überprüfe die Verkabelung!");
        delay(50);  // Warte eine halbe Sekunde
    }
    
    return sht20.begin(SHT20_RESOLUTION_12BITS, SHT20_I2C, _wireBus);
}

// Methode zum Abrufen der Temperatur
float SHT20Sensor::getTemperature() {
    if (sht20.connected()) {
        return sht20.temperature();
    }
    else {
        return -1;
    }

}

// Methode zum Abrufen der Luftfeuchtigkeit
float SHT20Sensor::getHumidity() {
    if (sht20.connected()) {
        return sht20.humidity();
    }
    else {
        return -1;
    }
}