#include "web_handlers.h"
#include <Arduino.h>

// ### Clock-Handler ###
void handleSetTimePage(Settings* settings, WebServer& server) {
    String html = "<html><body>";
    html += "<h1>Uhrzeit einstellen</h1>";
    html += "<form action=\"/updateTime\" method=\"POST\">";

    // Eingabefelder für die Uhrzeit
    html += "Stunde: <input type=\"number\" name=\"hour\" value=\"" + String(settings->hours) + "\" min=\"0\" max=\"23\"><br>";
    html += "Minute: <input type=\"number\" name=\"minute\" value=\"" + String(settings->minutes) + "\" min=\"0\" max=\"59\"><br>";

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
        
        server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
        server.send(303); // 303 Redirect
    } else {
        server.send(405, "text/html", "Methode nicht erlaubt!");
    }
}


// ### Set-Values-Handler ###

void handleSetValuesPage(Settings* settings, WebServer& server) {
    String html = "<html><body>";
    html += "<h1>Sollwerte umstellen</h1>";
    html += "<form action=\"/update_values\" method=\"POST\">";

    // Soll-Temperatur
    html += "<h2>Temperaturregler</h2>";
    html += "Soll-Temperatur (in &deg;C): ";
    html += "<input type=\"number\" name=\"targetTemp\" value=\"" + String(settings->targetTemperature, 1) + "\" step=\"0.1\"><br><br>";
    html += "Hysterese (in &deg;C): ";
    html += "<input type=\"number\" name=\"hysteresis\" value=\"" + String(settings->hysteresis, 2) + "\" step=\"0.1\"><br><br>";

    // Aktoren

    html += "<h2>Aktoren umstellen</h2>";

    // Lüfter 1
    html += "<h3>L&uuml;fter 1</h3>";
    html += "Aktive Laufzeit (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime1\" value=\"" + String(settings->onTime1) + "\" min=\"0\"><br>";
    html += "Zyklusanteil (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage1\" value=\"" + String(settings->onPercentage1) + "\" min=\"0\" max=\"100\"><br><br>";

    // Lüfter 2
    html += "<h3>L&uuml;fter 2</h3>";
    html += "Aktive Laufzeit (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime2\" value=\"" + String(settings->onTime2) + "\" min=\"0\"><br>";
    html += "Zyklusanteil (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage2\" value=\"" + String(settings->onPercentage2) + "\" min=\"0\" max=\"100\"><br><br>";

    // Steckdose 1 (ehemals Lüfter 3)
    html += "<h3>Steckdose 1</h3>";
    html += "Aktive Laufzeit (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime3\" value=\"" + String(settings->onTime3) + "\" min=\"0\"><br>";
    html += "Zyklusanteil (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage3\" value=\"" + String(settings->onPercentage3) + "\" min=\"0\" max=\"100\"><br><br>";

    // Steckdose 2 (ehemals Lüfter 4)
    html += "<h3>Steckdose 2</h3>";
    html += "Aktive Laufzeit (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime4\" value=\"" + String(settings->onTime4) + "\" min=\"0\"><br>";
    html += "Zyklusanteil (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage4\" value=\"" + String(settings->onPercentage4) + "\" min=\"0\" max=\"100\"><br><br>";

    html += "<input type=\"submit\" value=\"speichern\"><br><br>";  // Submit-Button für Aktoreinstellungen
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

    if (server.hasArg("hysteresis")) {
        settings->hysteresis = server.arg("hysteresis").toFloat();
    }
      
    server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
    server.send(303); // 303 Redirect
    
    saveCurrentSettings(*settings, preferences);  
}

