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

// ### Root-Handler ###

void handleRoot(Settings* settings, WebServer& server) {
    String html = "<html><body>";
    html += "<h1>ESP32 Steuerung</h1>";

    // Uhranzeige
    html += "<h2>Aktuelle Uhrzeit</h2>";
    html += "<div id='time'>Laden...</div>";
    html += "<a href=\"/setTime\"><button>Uhrzeit einstellen</button></a>";

    // Anzeige der aktuellen Lüfterlaufzeit und Zyklusanteil
    html += "<h2>Aktuelle Sollwerte</h2>";
    html += "L&uuml;fter 1<br>";
    html += "Aktive Laufzeit: " + String(settings->onTime1) + " Sekunden<br>";
    html += "Zyklusanteil: " + String(settings->onPercentage1) + " %<br><br>";

    html += "L&uuml;fter 2<br>";
    html += "Aktive Laufzeit: " + String(settings->onTime2) + " Sekunden<br>";
    html += "Zyklusanteil: " + String(settings->onPercentage2) + " %<br><br>";

    html += "L&uuml;fter 3<br>";
    html += "Aktive Laufzeit: " + String(settings->onTime3) + " Sekunden<br>";
    html += "Zyklusanteil: " + String(settings->onPercentage3) + " %<br><br>";

    html += "L&uuml;fter 4<br>";
    html += "Aktive Laufzeit: " + String(settings->onTime4) + " Sekunden<br>";
    html += "Zyklusanteil: " + String(settings->onPercentage4) + " %<br><br>";

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

