#ifndef HANDLERS_H
#define HANDLERS_H

#include <WebServer.h>
#include <Preferences.h>

struct Settings {
    int fan1DutyCycle;
    int fan2DutyCycle;
    float targetTemperature;

    int hours;
    int minutes;
    int seconds;

    int lastSavedMinute;
    int lastUpdateTime;

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
