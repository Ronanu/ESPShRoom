#include "handle_root.h"

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
    html += "</style>";
    html += "</head><body>";
    html += "<div class='container'>";
    html += "<h1 class='text-center'>ESP32 Steuerung</h1>";

    // Uhranzeige
    html += "<h2>Aktuelle Uhrzeit</h2>";
    html += "<div id='time' class='alert alert-info text-center'>Laden...</div>";
    html += "<a href=\"/setTime\" class='btn btn-primary'>Uhrzeit einstellen</a>";

    // Anzeige der aktuellen Lüfterlaufzeit und Zyklusanteil
    html += "<h2>Aktuelle Sollwerte</h2>";
    html += "<div class='sensor-data'>";
    html += "<strong>L&uuml;fter 1:</strong><br>";
    html += "Aktive Laufzeit: <span id='onTime1'>" + String(settings->onTime1) + "</span> Sekunden<br>";
    html += "Zyklusanteil: <span id='onPercentage1'>" + String(settings->onPercentage1) + "</span> %<br><br>";

    html += "<strong>L&uuml;fter 2:</strong><br>";
    html += "Aktive Laufzeit: <span id='onTime2'>" + String(settings->onTime2) + "</span> Sekunden<br>";
    html += "Zyklusanteil: <span id='onPercentage2'>" + String(settings->onPercentage2) + "</span> %<br><br>";

    html += "<strong>L&uuml;fter 3:</strong><br>";
    html += "Aktive Laufzeit: <span id='onTime3'>" + String(settings->onTime3) + "</span> Sekunden<br>";
    html += "Zyklusanteil: <span id='onPercentage3'>" + String(settings->onPercentage3) + "</span> %<br><br>";

    html += "<strong>L&uuml;fter 4:</strong><br>";
    html += "Aktive Laufzeit: <span id='onTime4'>" + String(settings->onTime4) + "</span> Sekunden<br>";
    html += "Zyklusanteil: <span id='onPercentage4'>" + String(settings->onPercentage4) + "</span> %<br><br>";

    html += "<strong>Soll-Temperatur:</strong> <span id='targetTemperature'>" + String(settings->targetTemperature, 1) + "</span> &deg;C<br>";
    html += "<a href=\"/set_values\" class='btn btn-secondary'>Sollwerte umstellen</a>";
    html += "</div>";

    // Ausgabe der aktuellen Sensordaten
    html += "<h2>Sensordaten</h2>";
    html += "<div class='sensor-data'>";
    html += "<strong>Temperatur 1:</strong> <span id='temperature1'>" + String(settings->temperature1, 1) + "</span> &deg;C<br>";
    html += "<strong>Temperatur 2:</strong> <span id='temperature2'>" + String(settings->temperature2, 1) + "</span> &deg;C<br>";
    html += "<strong>Luftfeuchtigkeit 1:</strong> <span id='humidity1'>" + String(settings->humidity1, 1) + "</span> %<br>";
    html += "<strong>Luftfeuchtigkeit 2:</strong> <span id='humidity2'>" + String(settings->humidity2, 1) + "</span> %<br>";
    html += "</div>";

    // AJAX zur dynamischen Aktualisierung
    html += "<script>";
    html += "function updateData() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.onreadystatechange = function() {";
    html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "      var data = JSON.parse(xhr.responseText);";
    html += "      document.getElementById('time').innerHTML = data.time;";
    html += "      document.getElementById('onTime1').innerHTML = data.onTime1;";
    html += "      document.getElementById('onPercentage1').innerHTML = data.onPercentage1;";
    html += "      document.getElementById('onTime2').innerHTML = data.onTime2;";
    html += "      document.getElementById('onPercentage2').innerHTML = data.onPercentage2;";
    html += "      document.getElementById('onTime3').innerHTML = data.onTime3;";
    html += "      document.getElementById('onPercentage3').innerHTML = data.onPercentage3;";
    html += "      document.getElementById('onTime4').innerHTML = data.onTime4;";
    html += "      document.getElementById('onPercentage4').innerHTML = data.onPercentage4;";
    html += "      document.getElementById('targetTemperature').innerHTML = data.targetTemperature;";
    html += "      document.getElementById('temperature1').innerHTML = data.temperature1;";
    html += "      document.getElementById('temperature2').innerHTML = data.temperature2;";
    html += "      document.getElementById('humidity1').innerHTML = data.humidity1;";
    html += "      document.getElementById('humidity2').innerHTML = data.humidity2;";
    html += "    }";
    html += "  };";
    html += "  xhr.open('GET', '/updateData', true);";
    html += "  xhr.send();";
    html += "}";
    html += "setInterval(updateData, 5000);";  // Aktualisierung alle 5 Sekunden
    html += "</script>";

    html += "</div></body></html>";

    server.send(200, "text/html", html);
}