#ifndef SETTINGS_H  // Überprüft, ob SETTINGS_H noch nicht definiert ist
#define SETTINGS_H  // Definiert SETTINGS_H, damit die Datei nur einmal eingebunden wird

#include <Preferences.h>

struct Settings {
    // RelaySettings
    bool isEnabled1 = false;
    bool isEnabled2 = false;
    bool isEnabled3 = false;
    bool isEnabled4 = false;
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
    float hysteresis;
    float deltaTmax;
    float maxHumidity;

    // TimeSettings
    int hours;
    int minutes;
    int seconds;

    // stability indicators
    int lastUpdateTime;
    int crashcounter;

    // SensorSettings
    float temperature1;
    float temperature2;
    float humidity1;
    float humidity2;
};

struct sensorData {
    float temperature1;
    float temperature2;
    float humidity1;
    float humidity2;
};

void printSettings(const Settings& settings);
void saveCurrentSettings(Settings settings, Preferences* preferences);
void loadSettings(Settings* settings, Preferences* preferences);
#endif  // Beendet den Include-Guard