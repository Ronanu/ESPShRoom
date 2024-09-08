#ifndef TEMPERATURECONTROLLER_H
#define TEMPERATURECONTROLLER_H

// Temperaturcontroller-Klasse
class TemperatureController {
public:
    // Konstruktor, der die Adressen für Zieltemperatur, Hysterese und Heizungsstatus übergibt
    TemperatureController(float* targetTemperature, float* hysteresis, bool* heatingEnabled);

    // Methode zum Aktualisieren des Heizungsstatus basierend auf der aktuellen Temperatur
    void update(float currentTemperature);

private:
    float* targetTemperature_;  // Zeiger auf die Zieltemperatur
    float* hysteresis_;         // Zeiger auf die Hysterese
    bool* heatingEnabled_;      // Zeiger auf den Heizungsstatus
};

#endif // TEMPERATURECONTROLLER_H