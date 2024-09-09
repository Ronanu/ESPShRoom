#ifndef SHT20SENSOR_H
#define SHT20SENSOR_H

#include <Wire.h>
#include <uFire_SHT20.h>
#include <Arduino.h>

class SHT20Sensor {
public:
    // Konstruktor mit Übergabe des Wire-Busses (z.B. Wire oder Wire1)
    SHT20Sensor(uint8_t sdaPin, uint8_t sclPin, TwoWire &wireBus);

    // Methode, um den Sensor zu initialisieren
    bool begin();

    // Methoden zum Abrufen der Temperatur und der Luftfeuchtigkeit
    float getTemperature();
    float getHumidity();

private:
    uint8_t _sdaPin;
    uint8_t _sclPin;
    TwoWire &_wireBus;  // Referenz auf den I²C-Bus
    uFire_SHT20 sht20;  // uFire_SHT20 Instanz als Member
};

#endif // SHT20SENSOR_H