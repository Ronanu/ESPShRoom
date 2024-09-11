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

    // Header
    html += "<h1>Hilfe: ShRoom Systemübersicht</h1>";
    html += "<p>Diese Seite erklärt die verschiedenen Funktionen und Einstellungen des Systems.</p>";

    // Laufzeit und Crashcounter
    html += "<h3>Laufzeit und Crashcounter</h3>";
    html += "<p><strong>Laufzeit:</strong> Zeigt an, wie lange das System seit dem letzten Neustart läuft, gemessen in Millisekunden.</p>";
    html += "<p><strong>Crashcounter:</strong> Zählt, wie oft das System unerwartet neugestartet wurde, was auf einen Fehler hinweisen könnte.</p>";

    // Sensordaten
    html += "<h3>Sensordaten</h3>";
    html += "<p><strong>Temperatur 1 und 2:</strong> Zeigt die aktuellen Temperaturen, die von den angeschlossenen Sensoren erfasst werden. Diese Daten werden regelmäßig aktualisiert.</p>";
    html += "<p><strong>Luftfeuchtigkeit 1 und 2:</strong> Zeigt die gemessene Luftfeuchtigkeit von den jeweiligen Sensoren an. Dies hilft, das Raumklima zu überwachen.</p>";

    // Sollzustände
    html += "<h3>Sollzustände</h3>";
    html += "<p><strong>Soll-Temperatur:</strong> Der gewünschte Temperaturwert, den das System anstrebt. Wenn die Temperatur über oder unter diesen Wert fällt, wird das System reagieren.</p>";
    html += "<p><strong>Hysterese:</strong> Dieser Wert definiert den Bereich, innerhalb dessen die Temperatur als akzeptabel angesehen wird, bevor das System eine Aktion auslöst (z.B. Lüfter einschalten).</p>";

    // Aktoreinstellungen
    html += "<h3>Aktoreinstellungen</h3>";
    html += "<p><strong>Lüfter 1 und 2:</strong> Zeigt die eingestellte aktive Laufzeit und den Zyklusanteil der Lüfter an. Diese Werte steuern, wie lange die Lüfter aktiv sind, um die Temperatur oder Luftfeuchtigkeit zu regulieren.</p>";
    html += "<p><strong>Steckdose 1 und 2:</strong> Zeigt die Laufzeit und den Zyklusanteil der Steckdosen an, die für verschiedene Geräte verwendet werden können. Diese Einstellungen können manuell angepasst werden.</p>";

    // Uhrzeit
    html += "<h3>Aktuelle Uhrzeit</h3>";
    html += "<p>Die Uhrzeit dient als Orientierung und kann manuell über die entsprechenden Einstellungen geändert werden. Sie wird für zeitbasierte Steuerungen im System verwendet.</p>";

    // Zurück-Button
    html += "<a href=\"/\" class='btn btn-primary'>Zurück zur Hauptseite</a>";
    
    html += "</div></body></html>";

    server.send(200, "text/html", html);
}