#include "TemperatureController.h"

int realistic_upper_limit = 30;  // Realistische obere Grenze für die Temperatur
int realistic_lower_limit = 10;  // Realistische untere Grenze für die Temperatur

// Konstruktor der TemperatureController-Klasse
TemperatureController::TemperatureController(float* targetTemperature, float* hysteresis, bool* heatingEnabled)
    : targetTemperature_(targetTemperature), hysteresis_(hysteresis), heatingEnabled_(heatingEnabled) {}

// Methode zur Aktualisierung des Heizungsstatus
void TemperatureController::update(float currentTemperature) {
    if (currentTemperature < *targetTemperature_ - *hysteresis_) {
        // Temperatur ist unterhalb des unteren Schwellenwerts, Heizung einschalten
        if (currentTemperature > realistic_lower_limit && currentTemperature < realistic_upper_limit){
            *heatingEnabled_ = true;
        }
        
    }
    else if (currentTemperature > *targetTemperature_ + *hysteresis_) {
        // Temperatur ist oberhalb des oberen Schwellenwerts, Heizung ausschalten
        *heatingEnabled_ = false;
    }
}