#ifndef HANDLERS_H
#define HANDLERS_H

#include <WebServer.h>
#include <Preferences.h>

struct Settings {
    // FanControlSettings
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

    // TemperatureControlSettings
    float targetTemperature;

    // to be removed
    int fan1DutyCycle;
    int fan2DutyCycle;
    

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

void handleRoot(Settings* settings, WebServer& server);
void handleTime(Settings* settings, WebServer& server);
void handleSetValuesPage(Settings* settings, WebServer& server);
void handleSetValues(Settings* settings, WebServer& server, Preferences* preferences);
void handleSetTimePage(Settings* settings, WebServer& server);
void handleSetTime(Settings* settings, WebServer& server, Preferences* preferences);
void saveCurrentSettings(Settings* settings, Preferences* preferences);
void loadSettings(Settings* settings, Preferences* preferences);
void updateTime(Settings* settings, Preferences* preferences);

#endif
