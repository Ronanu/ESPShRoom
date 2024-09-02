#include "web_handlers.h"
#include <WebServer.h>
#include <Preferences.h>
#include <Arduino.h>

extern WebServer server;
extern Preferences preferences;
extern int fan1DutyCycle, fan2DutyCycle, hours, minutes, seconds, lastSavedMinute;
extern float targetTemperature;
extern unsigned long lastUpdateTime;

void updateTime() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdateTime >= 1000) { // Eine Sekunde vergangen?
        lastUpdateTime = currentMillis;
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
                if (hours >= 24) {
                    hours = 0;
                }
            }
        }
        // Speichere die Werte, wenn sich die Minute ändert
        if (minutes != lastSavedMinute) {
            saveCurrentSettings();
            lastSavedMinute = minutes;
        }
    }
}

void saveCurrentSettings() {
    preferences.putInt("fan1DutyCycle", fan1DutyCycle);
    preferences.putInt("fan2DutyCycle", fan2DutyCycle);
    preferences.putFloat("targetTemperature", targetTemperature);
    preferences.putInt("hours", hours);
    preferences.putInt("minutes", minutes);
    Serial.println("Werte gespeichert!");
}

void loadSettings() {
    fan1DutyCycle = preferences.getInt("fan1DutyCycle", 50);
    fan2DutyCycle = preferences.getInt("fan2DutyCycle", 50);
    targetTemperature = preferences.getFloat("targetTemperature", 22.0);
    hours = preferences.getInt("hours", 0);
    minutes = preferences.getInt("minutes", 0);
    seconds = 0;  // Sekunden zurücksetzen
    lastSavedMinute = minutes;  // Initialisiere die letzte gespeicherte Minute
    Serial.println("Werte geladen!");
}


void handleTime() {
    String time = String(hours) + ":" + String(minutes) + ":" + String(seconds);
    server.send(200, "text/plain", time);
}

void handleSetValuesPage() {
    String html = "<html><body>";
    html += "<h1>Sollwerte umstellen</h1>";
    html += "<form action=\"/update_values\" method=\"POST\">";

    // Eingabefelder für Lüftereinschaltdauer in Prozent
    html += "Einschaltdauer L&uuml;fter 1 (in Prozent): ";
    html += "<input type=\"number\" name=\"fan1\" value=\"" + String(fan1DutyCycle) + "\" min=\"0\" max=\"100\"><br>";
    html += "Einschaltdauer L&uuml;fter 2 (in Prozent): ";
    html += "<input type=\"number\" name=\"fan2\" value=\"" + String(fan2DutyCycle) + "\" min=\"0\" max=\"100\"><br>";

    // Eingabefeld für Soll-Temperatur
    html += "Soll-Temperatur (in &deg;C): ";
    html += "<input type=\"number\" name=\"targetTemp\" value=\"" + String(targetTemperature) + "\" step=\"0.1\"><br>";

    html += "<input type=\"submit\" value=\"Werte setzen\">";
    html += "</form>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleSetValues() {
    if (server.hasArg("fan1")) {
        fan1DutyCycle = server.arg("fan1").toInt();
    }

    if (server.hasArg("fan2")) {
        fan2DutyCycle = server.arg("fan2").toInt();
    }

    if (server.hasArg("targetTemp")) {
        targetTemperature = server.arg("targetTemp").toFloat();
    }

    // Speichere die neuen Werte sofort
    saveCurrentSettings();

    server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
    server.send(303); // 303 Redirect
}

void handleSetTimePage() {
    String html = "<html><body>";
    html += "<h1>Uhrzeit einstellen</h1>";
    html += "<form action=\"/updateTime\" method=\"POST\">";
    html += "Stunde: <input type=\"text\" name=\"hour\" value=\"" + String(hours) + "\"><br>";
    html += "Minute: <input type=\"text\" name=\"minute\" value=\"" + String(minutes) + "\"><br>";
    html += "<input type=\"submit\" value=\"Setze Uhrzeit\">";
    html += "</form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleSetTime() {
    if (server.method() == HTTP_POST) {
        hours = server.arg("hour").toInt();
        minutes = server.arg("minute").toInt();
        seconds = 0; // Sekunden auf 0 setzen
        lastUpdateTime = millis(); // Startzeitpunkt speichern

        // Speichere die neuen Uhrzeit-Werte sofort
        saveCurrentSettings();

        server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
        server.send(303); // 303 Redirect
    } else {
        server.send(405, "text/html", "Methode nicht erlaubt!");
    }
}
