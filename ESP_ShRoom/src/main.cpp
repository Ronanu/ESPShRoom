#include <Wire.h>
#include "uFire_SHT20.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>  // Für das Speichern von Werten in NVS
#include "web_handlers.h"


// Sensor-Instanzen
uFire_SHT20 sht20_1;  // Sensor 1 an Pin 21, 22
uFire_SHT20 sht20_2;  // Sensor 2 an Pin 18, 19

// Sensordaten
float temperature1;
float temperature2;
float humidity1;
float humidity2;

// Lüftereinschaltdauer (in Prozent)
int fan1DutyCycle = 50;  // Standardwert 50%
int fan2DutyCycle = 50;  // Standardwert 50%

// Soll-Temperatur (in °C)
float targetTemperature = 22.0;  // Standardwert 22°C

// Uhrzeit-Variablen
int hours = 0;
int minutes = 0;
int seconds = 0;
int lastSavedMinute = -1;  // Um die Minute zu verfolgen, die zuletzt gespeichert wurde
unsigned long lastUpdateTime = 0; // Variable zum Speichern der letzten Aktualisierungszeit

Preferences preferences;

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80);



void handleRoot() {
    String html = "<html><body>";
    html += "<h1>ESP32 Steuerung</h1>";

    // Uhranzeige
    html += "<h2>Aktuelle Uhrzeit</h2>";
    html += "<div id='time'>Laden...</div>";
    html += "<a href=\"/setTime\"><button>Uhrzeit einstellen</button></a>";

    // Anzeige der aktuellen Lüftereinschaltdauer und Soll-Temperatur
    html += "<h2>Aktuelle Sollwerte</h2>";
    html += "Einschaltdauer L&uuml;fter 1: " + String(fan1DutyCycle) + " %<br>";
    html += "Einschaltdauer L&uuml;fter 2: " + String(fan2DutyCycle) + " %<br>";
    html += "Soll-Temperatur: " + String(targetTemperature, 1) + " &deg;C<br>";
    html += "<a href=\"/set_values\"><button>Sollwerte umstellen</button></a>";

    // Ausgabe der aktuellen Sensordaten
    html += "<h2>Sensordaten</h2>";
    html += "Temperatur 1: " + String(temperature1, 1) + " &deg;C<br>";
    html += "Temperatur 2: " + String(temperature2, 1) + " &deg;C<br>";
    html += "Luftfeuchtigkeit 1: " + String(humidity1, 1) + " %<br>";
    html += "Luftfeuchtigkeit 2: " + String(humidity2, 1) + " %<br>";

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

void setup() {
    Serial.begin(115200);

    // Initialisierung der Sensoren
    Wire.begin(21, 22);  // SDA, SCL für Sensor 1
    if (!sht20_1.begin()) {
        Serial.println("Sensor 1 nicht gefunden, überprüfe die Verkabelung!");
        while (1);
    }

    Wire.begin(18, 19);  // SDA, SCL für Sensor 2
    if (!sht20_2.begin()) {
        Serial.println("Sensor 2 nicht gefunden, überprüfe die Verkabelung!");
        while (1);
    }

    // Konfiguration des ESP32 als Access Point
    WiFi.softAP(ssid, password);
    Serial.println("Access Point gestartet");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.softAPIP());

    // Webserver Routen
    server.on("/", handleRoot);
    server.on("/time", handleTime); // Route für die aktuelle Uhrzeit
    server.on("/setTime", handleSetTimePage); // Route für die Uhrzeit Einstellungsseite
    server.on("/updateTime", handleSetTime); // Route um die Uhrzeit zu aktualisieren
    server.on("/set_values", handleSetValuesPage); // Route für die Sollwerte Einstellungsseite
    server.on("/update_values", handleSetValues); // Route um die Sollwerte zu aktualisieren
    server.begin();
    Serial.println("Webserver gestartet");

    // NVS Initialisierung und Werte laden
    preferences.begin("my-app", false);
    loadSettings();
}

void loop() {
    // Sensordaten aktualisieren
    Wire.begin(21, 22); 
    temperature1 = sht20_1.temperature();
    humidity1 = sht20_1.humidity();
    Wire.begin(18, 19);  // Wechsel zu Sensor 2
    temperature2 = sht20_2.temperature();
    humidity2 = sht20_2.humidity();

    // Uhrzeit aktualisieren
    updateTime();

    // Webserver anfragen bearbeiten
    server.handleClient();
}
