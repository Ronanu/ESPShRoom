#include "web_handlers.h"


void showHelpPage(Settings* settings, WebServer& server) {
    String html = "<!DOCTYPE html><html lang='de'><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Hilfe</title>";
    html += "<link href='https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css' rel='stylesheet'>";
    html += "<style>";
    html += "body { padding: 20px; }";
    html += ".container { margin-top: 50px; }";
    html += "</style>";
    html += "</head><body>";
    html += "<div class='container'>";
    html += "<h1>Hilfe</h1>";
    html += "<p>Willkommen auf der Hilfe-Seite. Hier finden Sie Erklärungen zu den Funktionen dieses Systems.</p>";
    html += "<p>Weitere Inhalte folgen in Kürze.</p>";
    html += "<a href=\"/\" class='btn btn-primary'>Zurück zur Hauptseite</a>";
    html += "</div></body></html>";

    server.send(200, "text/html", html);
}