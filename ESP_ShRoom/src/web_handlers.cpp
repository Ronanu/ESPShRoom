#include "web_handlers.h"
#include <Arduino.h>

// ### Clock-Handler ###

void handleSetTimePage(Settings* settings, WebServer& server) {
    String html = "<html><body>";
    html += "<h1>Uhrzeit einstellen</h1>";
    html += "<form action=\"/updateTime\" method=\"POST\">";
    html += "Stunde: <input type=\"text\" name=\"hour\" value=\"" + String(settings->hours) + "\"><br>";
    html += "Minute: <input type=\"text\" name=\"minute\" value=\"" + String(settings->minutes) + "\"><br>";
    html += "<input type=\"submit\" value=\"Setze Uhrzeit\">";
    html += "</form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleSetTime(Settings* settings, WebServer& server, Preferences* preferences) {
    if (server.method() == HTTP_POST) {
        settings->hours = server.arg("hour").toInt();
        settings->minutes = server.arg("minute").toInt();
        settings->seconds = 0; // Sekunden auf 0 setzen
        settings->lastUpdateTime = millis(); // Startzeitpunkt speichern

        // Speichere die neuen Uhrzeit-Werte sofort
        saveCurrentSettings(settings, preferences);

        server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
        server.send(303); // 303 Redirect
    } else {
        server.send(405, "text/html", "Methode nicht erlaubt!");
    }
}

void handleTime(Settings* settings, WebServer& server) {
    String time = String(settings->hours) + ":" + String(settings->minutes) + ":" + String(settings->seconds);
    server.send(200, "text/plain", time);
}

void updateTime(Settings* settings, Preferences* preferences) {
    unsigned long currentMillis = millis();
    if (currentMillis - settings->lastSavedMinute >= 1000) { // Eine Sekunde vergangen?
        settings->seconds++;
        if (settings->seconds >= 60) {
            settings->seconds = 0;
            settings->minutes++;
            if (settings->minutes >= 60) {
                settings->minutes = 0;
                settings->hours++;
                if (settings->hours >= 24) {
                    settings->hours = 0;
                }
            }
        }
        // Speichere die Werte, wenn sich die Minute ändert
        if (settings->minutes != settings->lastSavedMinute) {
            saveCurrentSettings(settings, preferences);
            settings->lastSavedMinute = settings->minutes;
        }
    }
}

// ### saving Settings-Handler ###
void saveCurrentSettings(Settings* settings, Preferences* preferences) {
    preferences->putInt("onTime1", settings->onTime1);
    preferences->putInt("onTime2", settings->onTime2);
    preferences->putInt("onTime3", settings->onTime3);
    preferences->putInt("onTime4", settings->onTime4);

    preferences->putInt("onPercentage1", settings->onPercentage1);
    preferences->putInt("onPercentage2", settings->onPercentage2);
    preferences->putInt("onPercentage3", settings->onPercentage3);
    preferences->putInt("onPercentage4", settings->onPercentage4);

    preferences->putFloat("targetTemperature", settings->targetTemperature);
    preferences->putInt("hours", settings->hours);
    preferences->putInt("minutes", settings->minutes);
    Serial.println("Werte gespeichert!");
}

void loadSettings(Settings* settings, Preferences* preferences) {
    settings->onTime1 = preferences->getInt("onTime1", 10);
    settings->onTime2 = preferences->getInt("onTime2", 10);
    settings->onTime3 = preferences->getInt("onTime3", 10);
    settings->onTime4 = preferences->getInt("onTime4", 10);

    settings->onPercentage1 = preferences->getInt("onPercentage1", 50);
    settings->onPercentage2 = preferences->getInt("onPercentage2", 50);
    settings->onPercentage3 = preferences->getInt("onPercentage3", 50);
    settings->onPercentage4 = preferences->getInt("onPercentage4", 50);

    settings->targetTemperature = preferences->getFloat("targetTemperature", 22.0);
    settings->hours = preferences->getInt("hours", 0);
    settings->minutes = preferences->getInt("minutes", 0);
    settings->seconds = 0;  // Sekunden zurücksetzen
    settings->lastSavedMinute = settings->minutes;  // Initialisiere die letzte gespeicherte Minute
    Serial.println("Werte geladen!");
}

// ### Set-Values-Handler ###

void handleSetValuesPage(Settings* settings, WebServer& server) {
    String html = "<html><body>";
    html += "<h1>Sollwerte umstellen</h1>";
    html += "<form action=\"/update_values\" method=\"POST\">";

    // Eingabefelder für Lüftereinschaltdauer in Sekunden und Prozentsatz
    html += "L&uuml;fter 1 Einschaltdauer (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime1\" value=\"" + String(settings->onTime1) + "\" min=\"0\"><br>";
    html += "L&uuml;fter 1 Einschaltdauer (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage1\" value=\"" + String(settings->onPercentage1) + "\" min=\"0\" max=\"100\"><br>";

    html += "L&uuml;fter 2 Einschaltdauer (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime2\" value=\"" + String(settings->onTime2) + "\" min=\"0\"><br>";
    html += "L&uuml;fter 2 Einschaltdauer (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage2\" value=\"" + String(settings->onPercentage2) + "\" min=\"0\" max=\"100\"><br>";

    html += "L&uuml;fter 3 Einschaltdauer (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime3\" value=\"" + String(settings->onTime3) + "\" min=\"0\"><br>";
    html += "L&uuml;fter 3 Einschaltdauer (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage3\" value=\"" + String(settings->onPercentage3) + "\" min=\"0\" max=\"100\"><br>";

    html += "L&uuml;fter 4 Einschaltdauer (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime4\" value=\"" + String(settings->onTime4) + "\" min=\"0\"><br>";
    html += "L&uuml;fter 4 Einschaltdauer (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage4\" value=\"" + String(settings->onPercentage4) + "\" min=\"0\" max=\"100\"><br>";

    // Eingabefeld für Soll-Temperatur
    html += "Soll-Temperatur (in &deg;C): ";
    html += "<input type=\"number\" name=\"targetTemp\" value=\"" + String(settings->targetTemperature) + "\" step=\"0.1\"><br>";

    html += "<input type=\"submit\" value=\"Werte setzen\">";
    html += "</form>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleSetValues(Settings* settings, WebServer& server, Preferences* preferences) {
    if (server.hasArg("onTime1")) {
        settings->onTime1 = server.arg("onTime1").toInt();
    }

    if (server.hasArg("onTime2")) {
        settings->onTime2 = server.arg("onTime2").toInt();
    }

    if (server.hasArg("onTime3")) {
        settings->onTime3 = server.arg("onTime3").toInt();
    }

    if (server.hasArg("onTime4")) {
        settings->onTime4 = server.arg("onTime4").toInt();
    }

    if (server.hasArg("onPercentage1")) {
        settings->onPercentage1 = server.arg("onPercentage1").toInt();
    }

    if (server.hasArg("onPercentage2")) {
        settings->onPercentage2 = server.arg("onPercentage2").toInt();
    }

    if (server.hasArg("onPercentage3")) {
        settings->onPercentage3 = server.arg("onPercentage3").toInt();
    }

    if (server.hasArg("onPercentage4")) {
        settings->onPercentage4 = server.arg("onPercentage4").toInt();
    }

    if (server.hasArg("targetTemp")) {
        settings->targetTemperature = server.arg("targetTemp").toFloat();
    }

    // Speichere die neuen Werte sofort
    saveCurrentSettings(settings, preferences);

    server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
    server.send(303); // 303 Redirect
}

