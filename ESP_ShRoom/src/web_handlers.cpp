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
    html += "Sekunde: <input type=\"number\" name=\"second\" value=\"" + String(settings->seconds) + "\" min=\"0\" max=\"59\"><br>";
    html += "<input type=\"submit\" value=\"Setze Uhrzeit\">";
    html += "</form>";
    html += "</body></html>";
    
    server.send(200, "text/html", html);
}

void handleSetTime(Settings* settings, WebServer& server, Preferences* preferences) {
    if (server.method() == HTTP_POST) {
        settings->hours = server.arg("hour").toInt();
        settings->minutes = server.arg("minute").toInt();
        settings->seconds = server.arg("second").toInt();
        
        server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
        server.send(303); // 303 Redirect
    } else {
        server.send(405, "text/html", "Methode nicht erlaubt!");
    }
}


// ### Settings-Handler ###

void showSetValuesPage(Settings* settings, WebServer& server) {
    String html = "<html><body>";
    html += "<h1>Sollwerte umstellen</h1>";
    html += "<form action=\"/update_values\" method=\"POST\">";

    // Soll-Temperatur
    html += "<h2>Regler-Werte</h2>";
    html += "Soll-Temperatur (in &deg;C): ";
    html += "<input type=\"number\" name=\"targetTemp\" value=\"" + String(settings->targetTemperature, 1) + "\" step=\"0.1\"><br><br>";
    html += "Hysterese (in &deg;C): ";
    html += "<input type=\"number\" name=\"hysteresis\" value=\"" + String(settings->hysteresis, 2) + "\" step=\"0.1\"><br><br>";
    html += "Maximale Luftfeuchtigkeit (in %): ";
    html += "<input type=\"number\" name=\"maxHumidity\" value=\"" + String(settings->maxHumidity, 1) + "\" step=\"0.1\"><br><br>";
    html += "<h2>Zeitschaltuhr</h2>";
    html += "Einschaltzeit: ";
    html += "<input type=\"number\" name=\"ct_lowerHour\" value=\"" + String(settings->ct_lowerHour) + "\" min=\"0\" max=\"23\">:";
    html += "<input type=\"number\" name=\"ct_lowerMinute\" value=\"" + String(settings->ct_lowerMinute) + "\" min=\"0\" max=\"59\"><br>";
    html += "Ausschaltzeit: ";
    html += "<input type=\"number\" name=\"ct_upperHour\" value=\"" + String(settings->ct_upperHour) + "\" min=\"0\" max=\"23\">:";
    html += "<input type=\"number\" name=\"ct_upperMinute\" value=\"" + String(settings->ct_upperMinute) + "\" min=\"0\" max=\"59\"><br><br>";

    // Aktoren

    html += "<h2>Aktoren umstellen</h2>";

    // Lüfter 1
    html += "<h3>L&uuml;fter 1</h3>";
    html += "Aktive Laufzeit (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime1\" value=\"" + String(settings->onTime1) + "\" min=\"0\"><br>";
    html += "Zyklusanteil (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage1\" value=\"" + String(settings->onPercentage1) + "\" min=\"5\" max=\"100\"><br>";
    html += "Aktiviert: <input type=\"checkbox\" name=\"isEnabled1\" " + String(settings->isEnabled1 ? "checked" : "") + "><br><br>";

    // Lüfter 2
    html += "<h3>L&uuml;fter 2</h3>";
    html += "Aktive Laufzeit (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime2\" value=\"" + String(settings->onTime2) + "\" min=\"0\"><br>";
    html += "Zyklusanteil (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage2\" value=\"" + String(settings->onPercentage2) + "\" min=\"5\" max=\"100\"><br>";
    html += "Aktiviert: <input type=\"checkbox\" name=\"isEnabled2\" " + String(settings->isEnabled2 ? "checked" : "") + "><br><br>";

    // Steckdose 1
    html += "<h3>Steckdose 1: Temperaturregler</h3>";
    html += "Aktive Laufzeit (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime3\" value=\"" + String(settings->onTime3) + "\" min=\"0\"><br>";
    html += "Zyklusanteil (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage3\" value=\"" + String(settings->onPercentage3) + "\" min=\"5\" max=\"100\"><br>";
    html += "Aktiviert wird anhand des Temperaturreglers.<br>";
    // html += "Aktiviert: <input type=\"checkbox\" name=\"isEnabled3\" " + String(settings->isEnabled3 ? "checked" : "") + "><br><br>";

    // Steckdose 2
    html += "<h3>Steckdose 2: Lichtsteuerung</h3>";
    html += "Aktive Laufzeit (in Sekunden): ";
    html += "<input type=\"number\" name=\"onTime4\" value=\"" + String(settings->onTime4) + "\" min=\"0\"><br>";
    html += "Zyklusanteil (in Prozent): ";
    html += "<input type=\"number\" name=\"onPercentage4\" value=\"" + String(settings->onPercentage4) + "\" min=\"5\" max=\"100\"><br>";
    html += "Aktiviert wird anhand der Zeitschaltuhr.<br>";

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

    if (server.hasArg("maxHumidity")) {
        settings->maxHumidity = server.arg("maxHumidity").toFloat();
    }

    // Clock time settings
    if (server.hasArg("ct_lowerHour")) {
        settings->ct_lowerHour = server.arg("ct_lowerHour").toInt();
    }

    if (server.hasArg("ct_lowerMinute")) {
        settings->ct_lowerMinute = server.arg("ct_lowerMinute").toInt();
    }

    if (server.hasArg("ct_upperHour")) {
        settings->ct_upperHour = server.arg("ct_upperHour").toInt();
    }

    if (server.hasArg("ct_upperMinute")) {
        settings->ct_upperMinute = server.arg("ct_upperMinute").toInt();
    }

    // Verarbeiten der neuen isEnabled-Flags
    settings->isEnabled1 = server.hasArg("isEnabled1");
    settings->isEnabled2 = server.hasArg("isEnabled2");
    // settings->isEnabled3 is controlled by temperature controller
    settings->isEnabled4 = server.hasArg("isEnabled4");

    // Save updated settings and redirect to root page
    saveCurrentSettings(*settings, preferences);  
    server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
    server.send(303); // 303 Redirect
}