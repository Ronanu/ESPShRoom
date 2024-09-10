#ifndef HANDLERS_H
#define HANDLERS_H

#include <WebServer.h>
#include "settings.h"


void handleSetValuesPage(Settings* settings, WebServer& server);
void handleSetValues(Settings* settings, WebServer& server, Preferences* preferences);
void handleSetTimePage(Settings* settings, WebServer& server);
void handleSetTime(Settings* settings, WebServer& server, Preferences* preferences);

void updateTime(Settings* settings, Preferences* preferences);

#endif
