#include <Arduino.h>
#include "fan_control.h"

// Eine gemeinsame Settings-Instanz für alle Lüfter
struct Settings {
    bool isEnabled1;
    bool isEnabled2;
    bool isEnabled3;
    bool isEnabled4;
    int onTime1;
    int onTime2;
    int onTime3;
    int onTime4;
    int onPercentage1;
    int onPercentage2;
    int onPercentage3;
    int onPercentage4;
};

// Eine Instanz der Settings
Settings settings;

// FanControl-Instanzen für vier Lüfter
FanControl* fanControl1;
FanControl* fanControl2;
FanControl* fanControl3;
FanControl* fanControl4;

void setup() {
    // Beispiel-Settings für Lüfter 1
    settings.isEnabled1 = true;
    settings.onTime1 = 1;         // 60 Sekunden Zykluszeit
    settings.onPercentage1 = 50;   // 50% Einschaltdauer

    // Beispiel-Settings für Lüfter 2
    settings.isEnabled2 = true;
    settings.onTime2 = 6;         // 60 Sekunden Zykluszeit
    settings.onPercentage2 = 40;   // 40% Einschaltdauer

    // Beispiel-Settings für Lüfter 3
    settings.isEnabled3 = true;
    settings.onTime3 = 5;         // 60 Sekunden Zykluszeit
    settings.onPercentage3 = 60;   // 60% Einschaltdauer

    // Beispiel-Settings für Lüfter 4
    settings.isEnabled4 = true;
    settings.onTime4 = 60;         // 60 Sekunden Zykluszeit
    settings.onPercentage4 = 30;   // 30% Einschaltdauer

    // FanControl-Instanzen erstellen und initialisieren
    fanControl1 = new FanControl(&settings.isEnabled1, &settings.onTime1, &settings.onPercentage1, 15);  // GPIO 25
    fanControl2 = new FanControl(&settings.isEnabled2, &settings.onTime2, &settings.onPercentage2, 2);  // GPIO 26
    fanControl3 = new FanControl(&settings.isEnabled3, &settings.onTime3, &settings.onPercentage3, 0);  // GPIO 27
    fanControl4 = new FanControl(&settings.isEnabled4, &settings.onTime4, &settings.onPercentage4, 4);  // GPIO 14

    // Initialisierung der FanControl-Instanzen
    fanControl1->initialize();
    fanControl2->initialize();
    fanControl3->initialize();
    fanControl4->initialize();
}

void loop() {
    // Aktualisiere den Lüfterstatus für alle vier Lüfter
    fanControl1->update();
    fanControl2->update();
    fanControl3->update();
    fanControl4->update();
}