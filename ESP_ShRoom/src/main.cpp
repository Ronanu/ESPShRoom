#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

Preferences preferences;  // Für den Zugriff auf den NVS

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

// Funktion zum Laden der gespeicherten Uhrzeit aus dem NVS
void loadTimeFromNVS() {
  hours = preferences.getInt("hours", 0); // Standardwert ist 0
  minutes = preferences.getInt("minutes", 0); // Standardwert ist 0
}

// Funktion zum Speichern der Uhrzeit im NVS
void saveTimeToNVS() {
  preferences.putInt("hours", hours);
  preferences.putInt("minutes", minutes);
}

// Funktion für die Startseite
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Aktuelle Uhrzeit</h1>";
  html += "<div id='time'>Laden...</div>";
  html += "<a href=\"/setTime\"><button>Uhrzeit einstellen</button></a>";
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

// Funktion, um die Uhrzeit im JSON-Format zu senden
void handleTime() {
  String time = String(hours) + ":" + String(minutes) + ":" + String(seconds);
  server.send(200, "text/plain", time);
}

// Funktion für die Uhrzeit Einstellungsseite
void handleSetTimePage() {
  String html = "<html><body>";
  html += "<h1>Uhrzeit einstellen</h1>";
  html += "<form action=\"/updateTime\" method=\"POST\">";
  html += "Stunde: <input type=\"text\" name=\"hour\" value=\"" + String(hours) + "\"><br>";
  html += "Minute: <input type=\"text\" name=\"minute\" value=\"" + String(minutes) + "\"><br>";
  html += "<input type=\"submit\" value=\"Setze Uhrzeit\">";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Funktion, um die Uhrzeit zu setzen und im NVS zu speichern
void handleSetTime() {
  if (server.method() == HTTP_POST) {
    hours = server.arg("hour").toInt();
    minutes = server.arg("minute").toInt();
    seconds = 0; // Sekunden auf 0 setzen
    lastUpdateTime = millis(); // Startzeitpunkt speichern
    saveTimeToNVS(); // Speichert die geänderte Zeit im NVS
    server.sendHeader("Location", "/"); // Umleitung zur Root-Seite
    server.send(303); // 303 Redirect
  } else {
    server.send(405, "text/html", "Methode nicht erlaubt!");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialisierung des NVS
  preferences.begin("my-app", false);
  
  // Laden der gespeicherten Uhrzeit
  loadTimeFromNVS();

  WiFi.softAP("ESP32-AP", "12345678"); // Zugangspunkt (AP) erstellen
  server.on("/", handleRoot); // Route für die Startseite
  server.on("/time", handleTime); // Route für die aktuelle Uhrzeit
  server.on("/setTime", handleSetTimePage); // Route für die Zeit setzen Seite
  server.on("/updateTime", handleSetTime); // Route um die Zeit zu aktualisieren
  server.begin(); // Webserver starten
}

void loop() {
  server.handleClient(); // Eingehende Anfragen verarbeiten
  updateTime(); // Aktualisiert die Zeit
}
