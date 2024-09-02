#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>


// Webserver auf Port 80
WebServer server(80);

// Initiale Zeit (nur Stunden und Minuten)
int hours = 0;
int minutes = 0;
unsigned long lastUpdateTime = 0; // Variable zum Speichern der letzten Aktualisierungszeit

// Funktion für die Startseite
void handleRoot() {
  String html = "<html><head><meta http-equiv='refresh' content='1'></head><body>";
  html += "<h1>Aktuelle Uhrzeit</h1>";
  html += "Stunde: " + String(hours) + "<br>";
  html += "Minute: " + String(minutes) + "<br>";
  html += "<form action=\"/setTime\" method=\"POST\">";
  html += "Stunde: <input type=\"text\" name=\"hour\" value=\"" + String(hours) + "\"><br>";
  html += "Minute: <input type=\"text\" name=\"minute\" value=\"" + String(minutes) + "\"><br>";
  html += "<input type=\"submit\" value=\"Setze Uhrzeit\">";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Funktion, um die Uhrzeit zu setzen
void handleSetTime() {
  if (server.method() == HTTP_POST) {
    hours = server.arg("hour").toInt();
    minutes = server.arg("minute").toInt();
    lastUpdateTime = millis(); // Startzeitpunkt speichern
    server.send(200, "text/html", "Uhrzeit gesetzt! <a href=\"/\">Zurück</a>");
  } else {
    server.send(405, "text/html", "Methode nicht erlaubt!");
  }
}

// Funktion zur Aktualisierung der Zeit
void updateTime() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateTime >= 60000) { // Eine Minute vergangen?
    lastUpdateTime = currentMillis;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
      if (hours >= 24) {
        hours = 0;
      }
    }
  }
}


void setup() {
  Serial.begin(115200);
  WiFi.softAP("ESP32-AP", "12345678"); // Zugangspunkt (AP) erstellen
  server.on("/", handleRoot); // Route für die Startseite
  server.on("/setTime", handleSetTime); // Route für die Zeit setzen
  server.begin(); // Webserver starten
}

void loop() {
  server.handleClient(); // Eingehende Anfragen verarbeiten
  updateTime(); // Aktualisiert die Zeit
}

