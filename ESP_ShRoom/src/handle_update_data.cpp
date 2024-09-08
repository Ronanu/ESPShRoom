#include "handle_root.h"

// Diese Funktion liefert die aktuellen Werte im JSON-Format zurück
void handleUpdateData(Settings* settings, WebServer& server) {
    String json = "{";
    json += "\"onTime1\":" + String(settings->onTime1) + ",";
    json += "\"onPercentage1\":" + String(settings->onPercentage1) + ",";
    json += "\"onTime2\":" + String(settings->onTime2) + ",";
    json += "\"onPercentage2\":" + String(settings->onPercentage2) + ",";
    json += "\"onTime3\":" + String(settings->onTime3) + ",";
    json += "\"onPercentage3\":" + String(settings->onPercentage3) + ",";
    json += "\"onTime4\":" + String(settings->onTime4) + ",";
    json += "\"onPercentage4\":" + String(settings->onPercentage4) + ",";
    json += "\"targetTemperature\":" + String(settings->targetTemperature, 1) + ",";
    json += "\"temperature1\":" + String(settings->temperature1, 1) + ",";
    json += "\"temperature2\":" + String(settings->temperature2, 1) + ",";
    json += "\"humidity1\":" + String(settings->humidity1, 1) + ",";
    json += "\"humidity2\":" + String(settings->humidity2, 1) + ",";
    json += "\"hours\":" + String(settings->hours) + ",";
    json += "\"minutes\":" + String(settings->minutes) + ",";
    json += "\"seconds\":" + String(settings->seconds);
    json += "}";

    server.send(200, "application/json", json);
}