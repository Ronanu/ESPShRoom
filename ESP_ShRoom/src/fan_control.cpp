#include "fan_control.h"

// Konstruktor
FanControl::FanControl(bool* enabledAddr, int* onTimeAddr, int* onPercentageAddr, int pin) {
    this->enabledAddr = enabledAddr;
    this->onTimeAddr = onTimeAddr;
    this->onPercentageAddr = onPercentageAddr;
    this->pin = pin;
    this->isFanOn = false; // Lüfter startet aus
    this->lastSwitchTime = 0; // Initialer Zeitpunkt
    this->onTimeMillis = 0;
    this->onDurationMillis = 0;
    this->offDurationMillis = 0;
    pinMode(pin, OUTPUT);
}

// Initialisierungsmethode
void FanControl::initialize() {
    // Sicherstellen, dass der Lüfter im richtigen Zustand startet
    calculateDurations();  // Berechne initiale Ein- und Ausschaltdauer
    if (*enabledAddr) {
        switchFan(true);
        lastSwitchTime = millis();  // Startzeitpunkt setzen
    } else {
        switchFan(false);
    }
}

// Diese Methode wird regelmäßig aufgerufen, um den Zustand basierend auf der Zeit zu überprüfen
void FanControl::update() {
    if (!*enabledAddr) {
        // Wenn der Lüfter ausgeschaltet sein soll, dann immer ausschalten
        switchFan(false);
        return;
    }

    unsigned long currentTime = millis();
    
    if (isFanOn && (currentTime - lastSwitchTime >= onDurationMillis)) {
        // Lüfter ausschalten, wenn die Einschaltdauer erreicht ist
        switchFan(false);
        lastSwitchTime = currentTime;  // Zeit für den nächsten Wechsel setzen
    } else if (!isFanOn && (currentTime - lastSwitchTime >= offDurationMillis)) {
        // Lüfter einschalten, wenn die Pausenzeit erreicht ist
        switchFan(true);
        lastSwitchTime = currentTime;  // Zeit für den nächsten Wechsel setzen
    }
}

// Methode zum Umschalten des Lüfterstatus
void FanControl::switchFan(bool state) {
    isFanOn = state;
    digitalWrite(pin, state ? HIGH : LOW);  // Lüfter an oder aus
}

// Berechnung der Ein- und Ausschaltdauer basierend auf der Gesamtdauer und der Einschaltdauer in Prozent
void FanControl::calculateDurations() {
    onTimeMillis = (*onTimeAddr) * 1000;  // Gesamtdauer des Zyklus in Millisekunden
    onDurationMillis = (onTimeMillis * (*onPercentageAddr)) / 100;  // Einschaltdauer basierend auf dem Prozentsatz
    offDurationMillis = onTimeMillis - onDurationMillis;  // Pausenzeit ist der Rest der Zyklusdauer
}