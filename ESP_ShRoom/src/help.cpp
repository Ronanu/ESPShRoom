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
    html += "<p>Es ist nicht bedenklich, wenn das System gelegentlich abstürzt. Der ESP32 verfügt über einen Reset-Knopf, mit dem das System neu gestartet werden kann. Alternativ kann das Gerät einfach für einige Sekunden aus- und wieder eingeschaltet werden, um den gleichen Effekt zu erzielen.</p>";

    // Sollzustände
    html += "<h3>Sollzustände</h3>";
    html += "<p><strong>Soll-Temperatur:</strong> Der gewünschte Temperaturwert, den das System anstrebt. Wenn die Temperatur über oder unter diesen Wert fällt, wird das System reagieren.</p>";
    html += "<p><strong>Hysterese:</strong> Dieser Wert definiert die maximale Temperaturschwankung. Er beeinflusst das Ein- und Ausschaltverhalten der Heizung, um zu vermeiden, dass sie ständig ein- und ausgeschaltet wird.</p>";

    // Sensordaten und Regelung
    html += "<h3>Sensordaten und Regelung</h3>";
    html += "<p><strong>Sensor 1:</strong> wird als Referenz für die Solltemperatur verwendet und sollte an einem Platz positioniert sein, der repräsentativ für die Umgebung ist. Über diesen Sensor wird die Heizung an- und ausgeschaltet.</p>";
    html += "<p><strong>Sensor 2:</strong> wird zur Temperaturüberwachung in der Nähe der Heizung verwendet, um sicherzustellen, dass es zu keiner Überhitzung kommt. Lüfter 1 wird eingeschaltet, wenn die Temperaturdifferenz zwischen Sensor 1 und 2 zu groß ist. Ist die Differenz zu groß, wird die Heizung abgeschaltet.</p>";
    
    // Allgemeinerklärung zu den Sensoren
    html += "<p>Die Sensoren in diesem System sind bidirektional einsetzbar, was bedeutet, dass sie in unterschiedlichen Positionen und Steckplätzen verwendet werden können. Auf der Platine sind jedoch bestimmte Steckplätze für spezifische Sensoren gekennzeichnet. Diese Flexibilität ermöglicht eine einfache Anpassung des Systems, ohne dass eine Neukalibrierung der Sensoren notwendig ist. Die Sensoren messen sowohl Temperatur als auch Luftfeuchtigkeit, um die Umgebungsbedingungen genau zu überwachen.</p>";

    // Regelung
    html += "<h3>Regelkreis Programmablauf</h3>";
    html += "<p>Das System regelt aktuell Lüfter 1 und die Heizung. Lüfter 2 und 3 sind noch nicht implementiert. Der Regelkreis steuert die Geräte basierend auf den gemessenen Temperaturen und der festgelegten Zieltemperatur.</p>";
    html += "<pre>";
    html += "float lazy_temperature1 = faultfilter1.checkValue(settings.temperature1);\n";
    html += "settings.isEnabled3 = hysteresiscontroller.update(settings.isEnabled3, lazy_temperature1);\n";
    // Lüfter 1 hängt an relais 1

    // Heizung hängt an Steckdose 1 bzw. relais 3
    html += "float lazy_temperature2 = faultfilter2.checkValue(settings.temperature2);\n";
    html += "settings.isEnabled1 = true;\n";
    html += "if (lazy_temperature2 <= settings.targetTemperature + 1) {\n";
    html += "    settings.onPercentage1 = 0;\n";
    html += "} else if (lazy_temperature2 <= settings.targetTemperature + 2) {\n";
    html += "    settings.onPercentage1 = 50;\n\n";
    html += "} else if (lazy_temperature2 <= settings.targetTemperature + 3) {\n";
    html += "    settings.onPercentage1 = 100;\n";
    html += "} else if (lazy_temperature2 > settings.targetTemperature + 4) {\n";
    html += "    settings.onPercentage1 = 100;\n";
    html += "    settings.isEnabled3 = false; // Temperaturbegrenzung im Bauraum\n";
    html += "}\n";
    html += "</pre>";
    html += "<p>Die Regelung basiert auf einer Hysterese, um das ständige Ein- und Ausschalten der Geräte zu verhindern. Die Lüfter werden schrittweise hochgefahren, sobald die Temperatur über den Zielwert steigt. Außerdem wird die Heizung abgeschaltet, falls die Temperatur zu hoch wird, um eine Überhitzung zu verhindern.</p>";
    // Aktoreinstellungen
    html += "<h3>Aktoreinstellungen</h3>";
    html += "<p>Die Aktoreinstellungen bestimmen die aktive Laufzeit und den Zyklusanteil der Lüfter. Ein Zyklusanteil von 0 % bedeutet, dass das Gerät ausgeschaltet ist, während 100 % Dauerbetrieb bedeutet. Werte dazwischen geben an, wie lange das Gerät während eines Zyklus eingeschaltet ist. Wenn der Zyklusanteil beispielsweise 10 % beträgt und die aktive Laufzeit 10 Sekunden beträgt, ist das Gerät 10 Sekunden lang aktiv und 90 Sekunden lang inaktiv, was zu einer Periodendauer von 100 Sekunden führt.</p>";

    // Uhrzeit
    html += "<h3>Aktuelle Uhrzeit</h3>";
    html += "<p>Die Uhrzeit dient als Orientierung und kann manuell über die entsprechenden Einstellungen geändert werden. Sie wird für zeitbasierte Steuerungen im System verwendet.</p>";

    // Zurück-Button
    html += "<a href=\"/\" class='btn btn-primary'>Zurück zur Hauptseite</a>";
    
    html += "</div></body></html>";

    server.send(200, "text/html", html);
}