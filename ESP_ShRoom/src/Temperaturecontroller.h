#ifndef TEMPERATURECONTROLLER_H
#define TEMPERATURECONTROLLER_H

#include <Arduino.h>
// Temperaturcontroller-Klasse
class HysteresisController {
public:
    // Konstruktor, der die Adressen für Zieltemperatur, Hysterese und Heizungsstatus übergibt
    HysteresisController(float* targetTemperature, float* hysteresis);

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

class FaultFilter {
private:
    float last_valid_value;  // Speichert den letzten gültigen Wert
    int counter;    // Zählt, wie oft der Wert -1 empfangen wurde
    int maxCount;   // Definiert, nach wie vielen -1 der Wert weitergegeben wird

public:
    // Konstruktor, der MaxCount initialisiert
    FaultFilter(int maxCountValue);

    // Methode, die überprüft, ob der Wert -1 ist
    float checkValue(float value);
};

#endif // VALUECHECKER_H