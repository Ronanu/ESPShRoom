#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

#include <Arduino.h>

// FanControl-Klasse
class FanControl {
public:
    // Konstruktor erhält die Adresse des Status, der Gesamtdauer, der Einschaltdauer in Prozent und den Pin
    FanControl(bool* enabledAddr, int* onTimeAddr, int* onPercentageAddr, int pin);
    
    // Initialisierungsmethode
    void initialize();

    // Diese Methode wird im `loop()` regelmäßig aufgerufen, um den Zustand zu überprüfen
    void update();

private:
    bool* enabledAddr;
    int* onTimeAddr;
    int* onPercentageAddr;
    int pin;

    bool isFanOn;              // Zustand des Lüfters
    unsigned long lastSwitchTime;  // Zeitpunkt des letzten Zustandswechsels
    int onTimeMillis;// Einschaltdauer in Millisekunden
    int offTimeMillis;// Berechnete Ausschaltdauer in Millisekunden

    // Methode zur Steuerung des Lüfterstatus
    void switchFan(bool state);

    // Berechnung der Ein- und Ausschaltdauer basierend auf der Gesamtdauer und der Einschaltdauer in Prozent
    void calculateDurations();
};

#endif // FAN_CONTROL_H