#include <WiFi.h>
#include <WebServer.h>

// Dummy-Werte für Sensoren
float temperature1 = 25.3;
float temperature2 = 26.7;
float humidity1 = 45.8;

// Lüftereinschaltdauer (in Prozent)
int fan1DutyCycle = 50;  // Standardwert 50%
int fan2DutyCycle = 50;  // Standardwert 50%

// Soll-Temperatur (in °C)
float targetTemperature = 22.0;  // Standardwert 22°C

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80);

void handleRoot() {
    String html = "<html><body>";
    html += "<h1>ESP32 Steuerung</h1>";
    html += "<form action=\"/set_values\" method=\"GET\">";
    
    // Eingabefelder für Lüftereinschaltdauer in Prozent
    html += "Einschaltdauer L&uuml;fter 1 (in Prozent): ";
    html += "<input type=\"number\" name=\"fan1\" value=\"" + String(fan1DutyCycle) + "\" min=\"0\" max=\"100\"><br>";
    html += "Einschaltdauer L&uuml;fter 2 (in Prozent): ";
    html += "<input type=\"number\" name=\"fan2\" value=\"" + String(fan2DutyCycle) + "\" min=\"0\" max=\"100\"><br>";
    
    // Eingabefeld für Soll-Temperatur
    html += "Soll-Temperatur (in &deg;C): ";
    html += "<input type=\"number\" name=\"targetTemp\" value=\"" + String(targetTemperature) + "\" step=\"0.1\"><br>";
    
    html += "<input type=\"submit\" value=\"OK\">";
    html += "</form>";

    // Ausgabe der aktuellen Sensordaten
    html += "<h2>Sensordaten</h2>";
    html += "Temperatur 1: " + String(temperature1) + " &deg;C<br>";
    html += "Temperatur 2: " + String(temperature2) + " &deg;C<br>";
    html += "Luftfeuchtigkeit 1: " + String(humidity1) + " %<br>";
    
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

    String html = "<html><body>";
    html += "<h1>Werte aktualisiert</h1>";
    html += "<p>Einschaltdauer L&uuml;fter 1: " + String(fan1DutyCycle) + " %</p>";
    html += "<p>Einschaltdauer L&uuml;fter 2: " + String(fan2DutyCycle) + " %</p>";
    html += "<p>Soll-Temperatur: " + String(targetTemperature) + " &deg;C</p>";
    html += "<a href=\"/\">Zur&uuml;ck</a>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void setup() {
    Serial.begin(115200);

    // Konfiguration des ESP32 als Access Point
    WiFi.softAP(ssid, password);
    Serial.println("Access Point gestartet");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.softAPIP());

    // Webserver Routen
    server.on("/", handleRoot);
    server.on("/set_values", handleSetValues);
    server.begin();
    Serial.println("Webserver gestartet");
}

void loop() {
    // Hier würde man normalerweise die Sensordaten aktualisieren
    // Für dieses Beispiel nehmen wir feste Dummy-Werte an

    server.handleClient();
}