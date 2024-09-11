#include "TemperatureController.h"

int realistic_upper_limit = 30;  // Realistische obere Grenze für die Temperatur
int realistic_lower_limit = 10;  // Realistische untere Grenze für die Temperatur

// Konstruktor der TemperatureController-Klasse
TemperatureController::TemperatureController(float* targetTemperature, float* hysteresis)
    : targetTemperature_(targetTemperature), hysteresis_(hysteresis) {}

// Methode zur Aktualisierung des Heizungsstatus
bool TemperatureController::update(bool currentState, float currentTemperature) {
    if (currentTemperature < realistic_lower_limit || currentTemperature > realistic_upper_limit){
        // Temperatur ist außerhalb des realistischen Bereichs, Heizung ausschalten
        Serial.println("Temperatur außerhalb des realistischen Bereichs\n\n");
        return false;
    }
    else


    if (currentTemperature < *targetTemperature_ - *hysteresis_) {
        // Temperatur ist unterhalb des unteren Schwellenwerts, Heizung einschalten
        return true;
    }
    else if (currentTemperature > *targetTemperature_ + *hysteresis_) {
        // Temperatur ist oberhalb des oberen Schwellenwerts, Heizung ausschalten
        return false;
    }
    else {
        // Temperatur ist im Bereich der Hysterese, Heizungszustand beibehalten
        return currentState;
    }


}



// Konstruktor setzt den Counter auf 0 und nimmt maxCount als Parameter
ValueChecker::ValueChecker(int maxCountValue) : counter(0), maxCount(maxCountValue) {}

// Methode, die die Logik für den Wert-Check enthält
float ValueChecker::checkValue(float value) {
    if (value == -1) {
        counter++;
        if (counter == maxCount) {
            counter = 0; // Counter zurücksetzen
            return -1;   // Wert -1 weitergeben
        } else {
            return 0;    // Wert ignorieren
        }
    } else {
        counter = 0;      // Counter zurücksetzen, wenn nicht -1
        return value;     // Den gegebenen Wert weitergeben
    }
}