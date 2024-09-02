#include "web_handlers.h"
#include <Arduino.h>


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

void saveCurrentSettings(Settings* settings, Preferences* preferences) {
    preferences->putInt("fan1DutyCycle", settings->fan1DutyCycle);
    preferences->putInt("fan2DutyCycle", settings->fan2DutyCycle);
    preferences->putFloat("targetTemperature", settings->targetTemperature);
    preferences->putInt("hours", settings->hours);
    preferences->putInt("minutes", settings->minutes);
    Serial.println("Werte gespeichert!");
}

void loadSettings(Settings* settings, Preferences* preferences) {
    settings->fan1DutyCycle = preferences->getInt("fan1DutyCycle", 50);
    settings->fan2DutyCycle = preferences->getInt("fan2DutyCycle", 50);
    settings->targetTemperature = preferences->getFloat("targetTemperature", 22.0);
    settings->hours = preferences->getInt("hours", 0);
    settings->minutes = preferences->getInt("minutes", 0);
    settings->seconds = 0;  // Sekunden zurücksetzen
    settings->lastSavedMinute = settings->minutes;  // Initialisiere die letzte gespeicherte Minute
    Serial.println("Werte geladen!");
}

void handleRoot(Settings* settings, WebServer& server) {
    String html = "<html><body>";
    html += "<h1>ESP32 Steuerung</h1>";

    // Uhranzeige
    html += "<h2>Aktuelle Uhrzeit</h2>";
    html += "<div id='time'>Laden...</div>";
    html += "<a href=\"/setTime\"><button>Uhrzeit einstellen</button></a>";

    // Anzeige der aktuellen Lüftereinschaltdauer und Soll-Temperatur
    html += "<h2>Aktuelle Sollwerte</h2>";
    html += "Einschaltdauer L&uuml;fter 1: " + String(settings->fan1DutyCycle) + " %<br>";
    html += "Einschaltdauer L&uuml;fter 2: " + String(settings->fan2DutyCycle) + " %<br>";
    html += "Soll-Temperatur: " + String(settings->targetTemperature, 1) + " &deg;C<br>";
    html += "<a href=\"/set_values\"><button>Sollwerte umstellen</button></a>";

    // Ausgabe der aktuellen Sensordaten
    html += "<h2>Sensordaten</h2>";
    html += "Temperatur 1: " + String(settings->temperature1, 1) + " &deg;C<br>";
    html += "Temperatur 2: " + String(settings->temperature2, 1) + " &deg;C<br>";
    html += "Luftfeuchtigkeit 1: " + String(settings->humidity1, 1) + " %<br>";
    html += "Luftfeuchtigkeit 2: " + String(settings->humidity2, 1) + " %<br>";

    html += "<script>";
    html += "function updateClock() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.onreadystatechange = function() {";
    html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "      document.getElementById('time').innerHTML = xhr.responseText;";
    html += "    }";
    html += "  };";
    html += "  xhr.open('GET', '/time', true);";
    html += "  xhr.send();";
    html += "}";
    html += "setInterval(updateClock, 1000);";
    html += "</script>";

    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleTime(Settings* settings, WebServer& server) {
    String time = String(settings->hours) + ":" + String(settings->minutes) + ":" + String(settings->seconds);
    server.send(200, "text/plain", time);
}

void handleSetValuesPage(Settings* settings, WebServer& server) {
    String html = "<html><body>";
    html += "<h1>Sollwerte umstellen</h1>";
    html += "<form action=\"/update_values\" method=\"POST\">";

    // Eingabefelder für Lüftereinschaltdauer in Prozent
    html += "Einschaltdauer L&uuml;fter 1 (in Prozent): ";
    html += "<input type=\"number\" name=\"fan1\" value=\"" + String(settings->fan1DutyCycle) + "\" min=\"0\" max=\"100\"><br>";
    html += "Einschaltdauer L&uuml;fter 2 (in Prozent): ";
    html += "<input type=\"number\" name=\"fan2\" value=\"" + String(settings->fan2DutyCycle) + "\" min=\"0\" max=\"100\"><br>";

    // Eingabefeld für Soll-Temperatur
    html += "Soll-Temperatur (in &deg;C): ";
    html += "<input type=\"number\" name=\"targetTemp\" value=\"" + String(settings->targetTemperature) + "\" step=\"0.1\"><br>";

    html += "<input type=\"submit\" value=\"Werte setzen\">";
    html += "</form>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

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

void handleSetValues(Settings* settings, WebServer& server, Preferences* preferences) {
    if (server.hasArg("fan1")) {
        settings->fan1DutyCycle = server.arg("fan1").toInt();
    }

    if (server.hasArg("fan2")) {
        settings->fan2DutyCycle = server.arg("fan2").toInt();
    }

    if (server.hasArg("targetTemp")) {
        settings->targetTemperature = server.arg("targetTemp").toFloat();
    }

    // Speichere die neuen Werte sofort
    saveCurrentSettings(settings, preferences);

    server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
    server.send(303); // 303 Redirect
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
