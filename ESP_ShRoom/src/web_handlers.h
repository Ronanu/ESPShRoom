#ifndef HANDLERS_H
#define HANDLERS_H

#include <WebServer.h>
#include "settings.h"

void handleRoot(Settings* settings, WebServer& server);
void handleUpdateData(Settings* settings, WebServer& server);
void showSetValuesPage(Settings* settings, WebServer& server);
void handleSetValues(Settings* settings, WebServer& server, Preferences* preferences);
void handleSetTimePage(Settings* settings, WebServer& server);
void handleSetTime(Settings* settings, WebServer& server, Preferences* preferences);

void showHelpPage(Settings* settings, WebServer& server);

#endif

#ifndef OFFLINE_CLOCK_H
#define OFFLINE_CLOCK_H

class OfflineClock {
private:
    unsigned long StartMillis;   // Speichert die Startzeit in Millisekunden
    unsigned long LastSecond;    // Speichert den letzten verstrichenen Sekundenwert
    Settings* settings;          // Zeiger auf das Settings-Objekt
    Preferences* preferences;    // Zeiger auf das Preferences-Objekt

public:
    // Konstruktor zur Initialisierung von StartMillis und LastSecond
    OfflineClock(Settings* settings, Preferences* preferences);

    // Methode zur Aktualisierung der Zeit
    void startClock();
    void updateTime();
};



#endif // OFFLINE_CLOCK_H