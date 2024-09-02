#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SHT2x.h>

// Instanzen für zwei SHT20-Sensoren
SHT2x sht20_1;
SHT2x sht20_2;

// Sensordaten
float temperature1 = 0.0;
float humidity1 = 0.0;
float temperature2 = 0.0;
float humidity2 = 0.0;

// Lüftereinschaltdauer (in Prozent)
int fan1DutyCycle = 50;
int fan2DutyCycle = 50;

// Soll-Temperatur (in °C)
float targetTemperature = 22.0;

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
    html += "Luftfeuchtigkeit 1: " + String(humidity1) + " %<br>";
    html += "Temperatur 2: " + String(temperature2) + " &deg;C<br>";
    html += "Luftfeuchtigkeit 2: " + String(humidity2) + " %<br>";
    
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

    // WiFi Setup
    WiFi.softAP(ssid, password);
    Serial.println("Access Point gestartet");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.softAPIP());

    // I2C-Initialisierung für den ersten Sensor
    Wire.begin(21, 22); // SDA = GPIO 21, SCL = GPIO 22
    sht20_1.begin();

    // I2C-Initialisierung für den zweiten Sensor
    Wire.begin(18, 19); // SDA = GPIO 18, SCL = GPIO 19
    sht20_2.begin();

    // Webserver Routen
    server.on("/", handleRoot);
    server.on("/set_values", handleSetValues);
    server.begin();
    Serial.println("Webserver gestartet");
}

void loop() {
    // Sensordaten vom ersten SHT20-Sensor aktualisieren
    Wire.begin(21, 22); // Erste I2C-Verbindung
    temperature1 = sht20_1.readTemperature();
    humidity1 = sht20_1.readHumidity();

    // Sensordaten vom zweiten SHT20-Sensor aktualisieren
    Wire.begin(18, 19); // Zweite I2C-Verbindung
    temperature2 = sht20_2.readTemperature();
    humidity2 = sht20_2.readHumidity();

    server.handleClient();
}
