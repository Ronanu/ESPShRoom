#ifndef TEMPERATURECONTROLLER_H
#define TEMPERATURECONTROLLER_H

#include <Arduino.h>
// Temperaturcontroller-Klasse
class TemperatureController {
public:
    // Konstruktor, der die Adressen für Zieltemperatur, Hysterese und Heizungsstatus übergibt
    TemperatureController(float* targetTemperature, float* hysteresis);

    // Methode zum Aktualisieren des Heizungsstatus basierend auf der aktuellen Temperatur
    bool update(bool currentState, float currentTemperature);

private:
    float* targetTemperature_;  // Zeiger auf die Zieltemperatur
    float* hysteresis_;         // Zeiger auf die Hysterese
    bool* heatingEnabled_;      // Zeiger auf den Heizungsstatus
};

#endif // TEMPERATURECONTROLLER_H

#ifndef VALUECHECKER_H
#define VALUECHECKER_H

class ValueChecker {
private:
    int counter;    // Zählt, wie oft der Wert -1 empfangen wurde
    int maxCount;   // Definiert, nach wie vielen -1 der Wert weitergegeben wird

public:
    // Konstruktor, der MaxCount initialisiert
    ValueChecker(int maxCountValue);

    // Methode, die überprüft, ob der Wert -1 ist
    float checkValue(float value);
};

#endif // VALUECHECKER_H