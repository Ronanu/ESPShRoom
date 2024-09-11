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

    // Es ist nicht bedenklich, wenn das System manchmal abstürzt. Der ESP hat einen reset-knopf, der das System neu startet. Beide drücken geht auch.
    // Einmal aus und Ein secken hat auch den leichen effekt. Das System startet neu und läuft weiter.

        // Sollzustände
    html += "<h3>Sollzustände</h3>";
    html += "<p><strong>Soll-Temperatur:</strong> Der gewünschte Temperaturwert, den das System anstrebt. Wenn die Temperatur über oder unter diesen Wert fällt, wird das System reagieren.</p>";
    html += "<p><strong>Hysterese:</strong> Dieser Wert definiert die maximale Temperaturschwankung. Dies hat einfluss auf das Einschaltverhalten der Heizung</p>";


    // Sensordaten
    html += "<h3>Sensordaten und Regelung</h3>";

    // Sensor 1 wird als referenz für die solltemperatur verwendet und sollte deshalb an einem optimalen platz sein, 
    // der aussaekräftig ist fpür die gesamte umgebung. Über diesen Sensor wird die Heizung an und ausgeschaltet
    // Sensor 2 wird für die Temperaturbegrenzung im Bauraum verwendet und sollte deshalb in der Nähe der Heizung platziert werden
    // Bei Temperaturdifferenzen zwischen Sensor 1 und 2 wird Lüfter 1 eingeschaltet.
    // Ist die Temperaturdifferenz zu groß, wird die Heizung abgeschaltet

    
    // Allgemeinerklärung zu den Sensoren
    html += "<p>Die Sensoren, die in diesem System verwendet werden, sind bidirektional einsetzbar. Das bedeutet, dass sie sowohl in unterschiedlichen Positionen als auch in verschiedenen Steckplätzen angeschlossen werden können. ";
    html += "Auf der Platine ist jedoch gekennzeichnet, welcher Sensor idealerweise für bestimmte Steckplätze verwendet werden sollte. ";
    html += "Diese Flexibilität ermöglicht eine einfache Anpassung des Systems an Ihre Bedürfnisse, ohne dass die Sensoren neu kalibriert werden müssen. ";
    html += "Die Sensoren messen sowohl Temperatur als auch Luftfeuchtigkeit und sind für eine präzise Überwachung der Umgebungsbedingungen ausgelegt.</p>";

    // Regelung
    html += "<h3>Regelkreis Programmablauf</h3>";
    // das System Reelt aktuell Lüfter 1 und die Heizung. Lüfter 2 und 3 sind noch nicht implementiert
    html += "<pre>";
    html += "float lazy_temperature1 = faultfilter1.checkValue(settings.temperature1);";
    html += "settings.isEnabled3 = hysteresiscontroller.update(settings.isEnabled3, lazy_temperature1);";
    html += "// Lüfter 1 hängt an relais 1";
    html += "// Heizung hängt an Steckdose 1 bzw. relais 3";
    html += "float lazy_temperature2 = faultfilter2.checkValue(settings.temperature2);";
    html += "settings.isEnabled1 = true;";
    html += "if (lazy_temperature2 <= settings.targetTemperature + 1) {";
    html += "    settings.onPercentage1 = 0;";
    html += "} else if (lazy_temperature2 <= settings.targetTemperature + 2) {";
    html += "    settings.onPercentage1 = 50;";
    html += "} else if (lazy_temperature2 <= settings.targetTemperature + 3) {";
    html += "    settings.onPercentage1 = 100;";
    html += "} else if (lazy_temperature2 > settings.targetTemperature + 4) {";
    html += "    settings.onPercentage1 = 100;";
    html += "    settings.isEnabled3 = false; // Temperaturbegrenzung im Bauraum";
    html += "}";
    html += "</pre>";
    html += "<p>Die Regelung basiert auf einer Hysterese, um das ständige Ein- und Ausschalten der Geräte zu verhindern. Die Lüfter werden schrittweise hochgefahren, sobald die Temperatur über den Zielwert steigt. Außerdem wird die Heizung abgeschaltet, falls die Temperatur zu hoch wird, um eine Überhitzung zu verhindern.</p>";

    // Aktoreinstellungen
    html += "<h3>Aktoreinstellungen</h3>";
    // Zeigt die eingestellte aktive Laufzeit und den Zyklusanteil der Lüfter an. Diese Werte steuern, wie lange die Lüfter aktiv sind, um die Temperatur oder Luftfeuchtigkeit zu regulieren.</p>";
    // ein Zyklusanteil von 0% heißt aus, 100% heißt dauerbetrieb und alles dazwischen ist ein prozentualer Anteil der Zeit, die der Aktor aktiv ist
    // wenn der Zyklusanzeil irgendwo zwischen 0 und 100 liegt, beträgt die Zeit der Ein-Phase immer die der Aktiven Laufzeit

    // Beispiel: 
    // Zyklusanteil 10 %, aktive Laufzeit 10 Sekunden:
    // 10s aktiv, 90s inaktiv  -> 10% von 100s = 10s
    // Periodendauer 100s

    
    // Uhrzeit
    html += "<h3>Aktuelle Uhrzeit</h3>";
    html += "<p>Die Uhrzeit dient als Orientierung und kann manuell über die entsprechenden Einstellungen geändert werden. Sie wird für zeitbasierte Steuerungen im System verwendet.</p>";

    // Zurück-Button
    html += "<a href=\"/\" class='btn btn-primary'>Zurück zur Hauptseite</a>";
    
    html += "</div></body></html>";

    server.send(200, "text/html", html);
}