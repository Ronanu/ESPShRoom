#include "web_handlers.h"

// Konstruktor-Implementierung
OfflineClock::OfflineClock(Settings* settings, Preferences* preferences) {
    this->settings = settings;
    this->preferences = preferences;
    this->StartMillis = StartMillis;
    this->LastSecond = 0;
}

void OfflineClock::startClock() {
    // NVS Initialisierung und Werte laden
    preferences->begin("my-app", false);
    loadSettings(settings, preferences);
    // Speichere die Start-Millis
    StartMillis = millis();
    settings->crashcounter++;
}

// updateTime Methode zur Aktualisierung der Zeit
void OfflineClock::updateTime() {
    unsigned long currentMillis = millis();
    unsigned long elapsedMillis = currentMillis - StartMillis;
    unsigned long currentSeconds = elapsedMillis / 1000;

    // Berechne die Anzahl der neuen Sekunden, die seit dem letzten Update vergangen sind
    unsigned long newSeconds = currentSeconds - LastSecond;

    // Wenn eine Sekunde oder mehr vergangen ist, Zeit hochzählen
    if (newSeconds > 0) {
        LastSecond = currentSeconds; // Letzten Sekundenzähler aktualisieren

        // Sekunden hochzählen
        settings->seconds += newSeconds;

        // Minuten hochzählen, wenn 60 Sekunden erreicht wurden
        if (settings->seconds >= 60) {
            settings->minutes += settings->seconds / 60;
            settings->seconds = settings->seconds % 60;
        }

        // Stunden hochzählen, wenn 60 Minuten erreicht wurden
        if (settings->minutes >= 60) {
            settings->hours += settings->minutes / 60;
            settings->minutes = settings->minutes % 60;
        }

        // Tage zurücksetzen, wenn 24 Stunden erreicht wurden
        if (settings->hours >= 24) {
            settings->hours = 0;
        }

        // Speichere die Einstellungen, nachdem eine Zeitveränderung aufgetreten ist
        saveCurrentSettings(*settings, preferences);
    }
}