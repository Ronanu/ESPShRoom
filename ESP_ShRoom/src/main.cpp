#include <WiFi.h>
#include <WebServer.h>

// Webserver auf Port 80
WebServer server(80);

// Initiale Zeit (nur Stunden, Minuten und Sekunden)
int hours = 0;
int minutes = 0;
int seconds = 0;
unsigned long lastUpdateTime = 0; // Variable zum Speichern der letzten Aktualisierungszeit

// Funktion zur Aktualisierung der Zeit
void updateTime() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateTime >= 1000) { // Eine Sekunde vergangen?
    lastUpdateTime = currentMillis;
    seconds++;
    if (seconds >= 60) {
      seconds = 0;
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
}

// Funktion für die Startseite
void handleRoot() {
  String html = "<html><head><meta http-equiv='refresh' content='1'></head><body style='background-color:black; color:white;'>";
  html += "<h1>Aktuelle Uhrzeit</h1>";
  html += "Stunde: " + String(hours) + "<br>";
  html += "Minute: " + String(minutes) + "<br>";
  html += "Sekunde: " + String(seconds) + "<br>";
  html += "<a href=\"/setTime\"><button>Uhrzeit einstellen</button></a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Funktion für die Uhrzeit Einstellungsseite
void handleSetTimePage() {
  String html = "<html><body style='background-color:black; color:white;'>";
  html += "<h1>Uhrzeit einstellen</h1>";
  html += "<form action=\"/updateTime\" method=\"POST\">";
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
    seconds = 0; // Sekunden auf 0 setzen
    lastUpdateTime = millis(); // Startzeitpunkt speichern
    server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
    server.send(303); // 303 Redirect
  } else {
    server.send(405, "text/html", "Methode nicht erlaubt!");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP("ESP32-AP", "12345678"); // Zugangspunkt (AP) erstellen
  server.on("/", handleRoot); // Route für die Startseite
  server.on("/setTime", handleSetTimePage); // Route für die Zeit setzen Seite
  server.on("/updateTime", handleSetTime); // Route um die Zeit zu aktualisieren
  server.begin(); // Webserver starten
}

void loop() {
  server.handleClient(); // Eingehende Anfragen verarbeiten
  updateTime(); // Aktualisiert die Zeit
}