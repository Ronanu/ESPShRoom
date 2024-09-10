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
    this->offTimeMillis = 0;
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

    // Berechne die neuen Ein- und Ausschaltzeiten
    calculateDurations();

    unsigned long currentTime = millis();
    
    if (isFanOn && (currentTime - lastSwitchTime >= onTimeMillis) && *onPercentageAddr < 100) {
        // Lüfter ausschalten, wenn die Einschaltdauer erreicht ist
        switchFan(false);
        lastSwitchTime = currentTime;  // Zeit für den nächsten Wechsel setzen
    } else if (!isFanOn && (currentTime - lastSwitchTime >= offTimeMillis && *onPercentageAddr > 0)) {
        // Lüfter einschalten, wenn die Pausenzeit erreicht ist
        switchFan(true);
        lastSwitchTime = currentTime;  // Zeit für den nächsten Wechsel setzen
    }
}

// Methode zum Umschalten des Lüfterstatus
void FanControl::switchFan(bool state) {
    isFanOn = state;
    delay(10);  // Verzögerung, um den Lüfter nicht zu schnell ein- und auszuschalten
    digitalWrite(pin, state ? LOW : HIGH);  // Lüfter an oder aus
    delay(10);  // Verzögerung, um den Lüfter nicht zu schnell ein- und auszuschalten
}

// Berechnung der Ein- und Ausschaltdauer basierend auf der Gesamtdauer und der Einschaltdauer in Prozent
void FanControl::calculateDurations() {
    onTimeMillis = (*onTimeAddr) * 1000;  // Gesamtdauer des Zyklus in Millisekunden
    if (*onPercentageAddr <= 0) {
        //*enabledAddr = false;  // Lüfter ausschalten, wenn der Prozentsatz 0 oder negativ ist
    }
    else {
        //*enabledAddr = true;  // Lüfter einschalten, wenn der Prozentsatz positiv ist
        int total_period = 100 * onTimeMillis / *onPercentageAddr;  // Gesamtdauer des Zyklus
        offTimeMillis = total_period - onTimeMillis;  // Pausenzeit ist der Rest der Zyklusdauer
    }
    //onTimeMillis = 1000;  // Einschaltdauer ist 1 Sekunde
    //offTimeMillis = 100;  // Pausenzeit ist 1 Sekunde
}