#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WebServer server(80);

const int pwmPin = 18; // Pin für PWM
int pwmFrequency = 1000; // Initiale PWM-Frequenz in Hz

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP32 PWM Steuerung</h1>";
  html += "<form action=\"/set_pwm\" method=\"GET\">";
  html += "Frequenz in Hz: ";
  
  // Hier fügen wir den aktuellen PWM-Wert als Standardwert ins Textfeld ein
  html += "<input type=\"number\" name=\"freq\" value=\"" + String(pwmFrequency) + "\" min=\"1\"><br>";
  
  html += "<input type=\"submit\" value=\"OK\">";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleSetPwm() {
  if (server.hasArg("freq")) {
    pwmFrequency = server.arg("freq").toInt();
    ledcWriteTone(0, pwmFrequency);
  }

  String html = "<html><body>";
  html += "<h1>Frequenz wurde auf ";
  html += pwmFrequency;
  html += " Hz gesetzt.</h1>";
  html += "<a href=\"/\">Zurück</a>";
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

  // PWM-Setup
  ledcSetup(0, pwmFrequency, 8); // Kanal 0, Frequenz, Auflösung 8-bit
  ledcAttachPin(pwmPin, 0);      // PWM Pin an Kanal 0

  // Webserver Routen
  server.on("/", handleRoot);
  server.on("/set_pwm", handleSetPwm);
  server.begin();
  Serial.println("Webserver gestartet");
}

void loop() {
  server.handleClient();
}
