#include "TemperatureController.h"

// Konstruktor der TemperatureController-Klasse
TemperatureController::TemperatureController(float* targetTemperature, float* hysteresis, bool* heatingEnabled)
    : targetTemperature_(targetTemperature), hysteresis_(hysteresis), heatingEnabled_(heatingEnabled) {}

// Methode zur Aktualisierung des Heizungsstatus
void TemperatureController::update(float currentTemperature) {
    if (currentTemperature < *targetTemperature_ - *hysteresis_) {
        // Temperatur ist unterhalb des unteren Schwellenwerts, Heizung einschalten
        *heatingEnabled_ = true;
    }
    else if (currentTemperature > *targetTemperature_ + *hysteresis_) {
        // Temperatur ist oberhalb des oberen Schwellenwerts, Heizung ausschalten
        *heatingEnabled_ = false;
    }
}