#ifndef SETTINGS_H  // Überprüft, ob SETTINGS_H noch nicht definiert ist
#define SETTINGS_H  // Definiert SETTINGS_H, damit die Datei nur einmal eingebunden wird


struct Settings {
    // FanControlSettings
    bool isEnabled1 = true;
    bool isEnabled2 = true;
    bool isEnabled3 = true;
    bool isEnabled4 = true;
    int onTime1;
    int onTime2;
    int onTime3;
    int onTime4;
    int onPercentage1;
    int onPercentage2;
    int onPercentage3;
    int onPercentage4;

    // TemperatureControlSettings
    float targetTemperature;    

    // TimeSettings
    int hours;
    int minutes;
    int seconds;

    int lastSavedMinute;
    int lastUpdateTime;

    // SensorSettings
    float temperature1;
    float temperature2;
    float humidity1;
    float humidity2;
};

#endif  // Beendet den Include-Guard