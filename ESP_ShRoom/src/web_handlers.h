#ifndef HANDLERS_H
#define HANDLERS_H

#include <WebServer.h>
#include <Preferences.h>
#include "settings.h"

void handleTime(Settings* settings, WebServer& server);
void handleSetValuesPage(Settings* settings, WebServer& server);
void handleSetValues(Settings* settings, WebServer& server, Preferences* preferences);
void handleSetTimePage(Settings* settings, WebServer& server);
void handleSetTime(Settings* settings, WebServer& server, Preferences* preferences);
void saveCurrentSettings(Settings* settings, Preferences* preferences);
void loadSettings(Settings* settings, Preferences* preferences);
void updateTime(Settings* settings, Preferences* preferences);

#endif
