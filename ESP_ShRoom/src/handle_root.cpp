#include "web_handlers.h"

// Diese Funktion generiert die Weboberfläche
void handleRoot(Settings* settings, WebServer& server) {
    String html = "<!DOCTYPE html><html lang='de'><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>ESP32 Steuerung</title>";
    html += "<link href='https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css' rel='stylesheet'>";
    html += "<style>";
    html += "body { padding: 20px; }";
    html += ".sensor-data { margin-top: 20px; }";
    html += ".btn { margin-top: 10px; }";
    html += ".runtime { text-align: center; font-size: small; }";  // Stile für Runtime
    html += "</style>";
    html += "</head><body>";
    html += "<div class='container'>";
    
    // Überschrift: Sensordaten
    html += "<h1 class='text-center'>ShRoom</h1>";

    // Ausgabe der Runtime direkt nach der Überschrift
    html += "<div class='runtime'>";
    html += "Laufzeit: <span id='lastUpdateTime'>" + String(settings->lastUpdateTime) + "</span> ms<br>";  // ID für lastUpdateTime bleibt unverändert
    html += "<div class='crashcounter'>Crashcounter: <span id = 'crashcounter'>" + String(settings->crashcounter) + "</span></div><br>";  // ID für crashcounter bleibt unverändert

    // Ausgabe der aktuellen Sensordaten
    html += "<div class='sensor-data'>";
    html += "<h3>Sensordaten</h3>";
    html += "<strong>Temperatur 1: </strong> <span id='temperature1'>" + String(settings->temperature1, 3) + "</span> &deg;C<br>";  // Temperatur ID
    html += "<strong>Temperatur 2: </strong> <span id='temperature2'>" + String(settings->temperature2, 3) + "</span> &deg;C<br>";  // Temperatur ID
    html += "<strong>Luftfeuchtigkeit 1: </strong> <span id='humidity1'>" + String(settings->humidity1, 3) + "</span> %<br>";  // Luftfeuchtigkeit ID
    html += "<strong>Luftfeuchtigkeit 2: </strong> <span id='humidity2'>" + String(settings->humidity2, 3) + "</span> %<br>";  // Luftfeuchtigkeit ID    
    html += "</div>";

    // Überschrift: Sollzustände
    html += "<div class='soll-data'>";
    html += "<h3>Sollzustände</h3>";
    html += "<strong>Soll-Temperatur:\t</strong> <span id='targetTemperature'>" + String(settings->targetTemperature, 1) + "</span> &deg;C<br>";
    html += "<strong>Hysterese:\t</strong> <span id='hysteresis'>" + String(settings->hysteresis, 2) + "</span> &deg;C<br><br>";  // Hysterese fett
    html += "<a href=\"/set_values\" class='btn btn-secondary'>Einstellungen</a><br><br>";  // Button für Einstellungen unter Temperaturregler

    // Überschrift: Aktoreinstellungen
    html += "<h2>Aktoreinstellungen</h2>";
    html += "<div class='aktor_settings'>";

    // Lüfter 1
    html += "<h3>L&uuml;fter 1:</h3>";  // Lüfter 1 als Unterüberschrift
    html += "Aktive Laufzeit: <span id='onTime1'>" + String(settings->onTime1) + "</span> Sekunden<br>";  // Normaler Text
    html += "Zyklusanteil: <span id='onPercentage1'>" + String(settings->onPercentage1) + "</span> %<br><br>";  // Normaler Text

    // Lüfter 2
    html += "<h3>L&uuml;fter 2:</h3>";  // Lüfter 2 als Unterüberschrift
    html += "Aktive Laufzeit: <span id='onTime2'>" + String(settings->onTime2) + "</span> Sekunden<br>";  // Normaler Text
    html += "Zyklusanteil: <span id='onPercentage2'>" + String(settings->onPercentage2) + "</span> %<br><br>";  // Normaler Text

    // Steckdose 1
    html += "<h3>Steckdose 1:</h3>";  // Steckdose 1 als Unterüberschrift
    html += "Aktive Laufzeit: <span id='onTime3'>" + String(settings->onTime3) + "</span> Sekunden<br>";  // Normaler Text
    html += "Zyklusanteil: <span id='onPercentage3'>" + String(settings->onPercentage3) + "</span> %<br><br>";  // Normaler Text

    // Steckdose 2
    html += "<h3>Steckdose 2:</h3>";  // Steckdose 2 als Unterüberschrift
    html += "Aktive Laufzeit: <span id='onTime4'>" + String(settings->onTime4) + "</span> Sekunden<br>";  // Normaler Text
    html += "Zyklusanteil: <span id='onPercentage4'>" + String(settings->onPercentage4) + "</span> %<br><br>";  // Normaler Text
    html += "</div>";

    // Uhranzeige am Ende der Seite in einer Zeile
    html += "<h2>Aktuelle Uhrzeit</h2>";
    html += "<div class='alert alert-info text-center'>";
    html += "<span id='hours'>" + String(settings->hours) + "</span> : ";
    html += "<span id='minutes'>" + String(settings->minutes) + "</span> : ";
    html += "<span id='seconds'>" + String(settings->seconds) + "</span>";
    html += "</div>";

    html += "<a href=\"/setTime\" class='btn btn-secondary'>Einstellungen Uhr</a><br><br>";  // Button für Aktoreinstellungen

    // JavaScript für AJAX-Updates und Reconnect-Logik
    html += "<script>";
    html += "var lastSuccessfulUpdate = Date.now();";  // Zeitpunkt des letzten erfolgreichen Updates

    html += "function updateData() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.onreadystatechange = function() {";
    html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "      var data = JSON.parse(xhr.responseText);";
    html += "      document.getElementById('onTime1').innerHTML = data.onTime1;";
    html += "      document.getElementById('onPercentage1').innerHTML = data.onPercentage1;";
    html += "      document.getElementById('onTime2').innerHTML = data.onTime2;";
    html += "      document.getElementById('onPercentage2').innerHTML = data.onPercentage2;";
    html += "      document.getElementById('onTime3').innerHTML = data.onTime3;";
    html += "      document.getElementById('onPercentage3').innerHTML = data.onPercentage3;";
    html += "      document.getElementById('onTime4').innerHTML = data.onTime4;";
    html += "      document.getElementById('onPercentage4').innerHTML = data.onPercentage4;";
    html += "      document.getElementById('targetTemperature').innerHTML = data.targetTemperature;";
    html += "      document.getElementById('hysteresis').innerHTML = data.hysteresis;";
    html += "      document.getElementById('temperature1').innerHTML = data.temperature1;";
    html += "      document.getElementById('humidity1').innerHTML = data.humidity1;";
    html += "      document.getElementById('temperature2').innerHTML = data.temperature2;";
    html += "      document.getElementById('humidity2').innerHTML = data.humidity2;";
    html += "      document.getElementById('lastUpdateTime').innerHTML = data.lastUpdateTime;";
    html += "      document.getElementById('hours').innerHTML = data.hours;";
    html += "      document.getElementById('minutes').innerHTML = (data.minutes < 10 ? '0' : '') + data.minutes;";
    html += "      document.getElementById('seconds').innerHTML = (data.seconds < 10 ? '0' : '') + data.seconds;";
    html += "      document.getElementById('crashcounter').innerHTML = data.crashcounter;";
    html += "      lastSuccessfulUpdate = Date.now();";  // Erfolgreiches Update
    html += "    }";
    html += "  };";
    
    // Timeout einstellen
    html += "  xhr.timeout = 2000;";  // 2 Sekunden Timeout
    html += "  xhr.ontimeout = function () {";
    html += "    console.warn('Request timed out, attempting reconnect...');";
    html += "    attemptReconnect();";  // Reconnect-Versuch bei Timeout
    html += "  };";
    
    html += "  xhr.open('GET', '/updateData', true);";
    html += "  xhr.send();";
    html += "}";

    html += "function attemptReconnect() {";
    html += "  if (Date.now() - lastSuccessfulUpdate > 2000) {";
    html += "    console.log('No response, attempting to reload the page...');";
    html += "    location.reload();";  // Seite neu laden, um die Verbindung wiederherzustellen
    html += "  }";
    html += "}";

    html += "setInterval(updateData, 1000);";  // Regelmäßige Aktualisierung alle 1 Sekunde
    html += "</script>";

    html += "</div></body></html>";

    server.send(200, "text/html", html);
}

// Diese Funktion liefert die aktuellen Werte im JSON-Format zurück
void handleUpdateData(Settings* settings, WebServer& server) {
    String json = "{";
    json += "\"onTime1\":" + String(settings->onTime1) + ",";
    json += "\"onPercentage1\":" + String(settings->onPercentage1) + ",";
    json += "\"onTime2\":" + String(settings->onTime2) + ",";
    json += "\"onPercentage2\":" + String(settings->onPercentage2) + ",";
    json += "\"onTime3\":" + String(settings->onTime3) + ",";
    json += "\"onPercentage3\":" + String(settings->onPercentage3) + ",";
    json += "\"onTime4\":" + String(settings->onTime4) + ",";
    json += "\"onPercentage4\":" + String(settings->onPercentage4) + ",";
    json += "\"targetTemperature\":" + String(settings->targetTemperature) + ",";
    json += "\"temperature1\":" + String(settings->temperature1) + ",";
    json += "\"temperature2\":" + String(settings->temperature2) + ",";
    json += "\"humidity1\":" + String(settings->humidity1) + ",";
    json += "\"humidity2\":" + String(settings->humidity2) + ",";
    json += "\"hours\":" + String(settings->hours) + ",";
    json += "\"minutes\":" + String(settings->minutes) + ",";
    json += "\"seconds\":" + String(settings->seconds) + ",";
    json += "\"hysteresis\":" + String(settings->hysteresis) + ",";  // Hysterese hinzugefügt
    json += "\"crashcounter\":" + String(settings->crashcounter) + ",";  // Crashcounter hinzugefügt
    json += "\"lastUpdateTime\":" + String(settings->lastUpdateTime);  // Füge die Laufzeit hinzu
    json += "}";

    server.send(200, "application/json", json);
}
