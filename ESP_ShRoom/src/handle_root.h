#ifndef WEBHANDLER_CUSTOM_H
#define WEBHANDLER_CUSTOM_H

#include <WebServer.h>
#include <settings.h>

void handleRoot(Settings* settings, WebServer& server);
void handleUpdateData(Settings* settings, WebServer& server);

#endif