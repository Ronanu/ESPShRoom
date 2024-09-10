#include "settings.h"
#include <Arduino.h>

void printSettings(const Settings& settings) {
    Serial.println("=== FanControlSettings ===");
    Serial.print("isEnabled1: "); Serial.println(settings.isEnabled1 ? "true" : "false");
    Serial.print("isEnabled2: "); Serial.println(settings.isEnabled2 ? "true" : "false");
    Serial.print("isEnabled3: "); Serial.println(settings.isEnabled3 ? "true" : "false");
    Serial.print("isEnabled4: "); Serial.println(settings.isEnabled4 ? "true" : "false");

    Serial.print("onTime1: "); Serial.println(settings.onTime1);
    Serial.print("onTime2: "); Serial.println(settings.onTime2);
    Serial.print("onTime3: "); Serial.println(settings.onTime3);
    Serial.print("onTime4: "); Serial.println(settings.onTime4);

    Serial.print("onPercentage1: "); Serial.println(settings.onPercentage1);
    Serial.print("onPercentage2: "); Serial.println(settings.onPercentage2);
    Serial.print("onPercentage3: "); Serial.println(settings.onPercentage3);
    Serial.print("onPercentage4: "); Serial.println(settings.onPercentage4);

    Serial.println("=== TemperatureControlSettings ===");
    Serial.print("targetTemperature: "); Serial.println(settings.targetTemperature);
    Serial.print("hysteresis: "); Serial.println(settings.hysteresis);

    Serial.println("=== TimeSettings ===");
    Serial.print("hours: "); Serial.println(settings.hours);
    Serial.print("minutes: "); Serial.println(settings.minutes);
    Serial.print("seconds: "); Serial.println(settings.seconds);
    Serial.print("lastUpdateTime: "); Serial.println(settings.lastUpdateTime);

    Serial.println("=== SensorSettings ===");
    Serial.print("temperature1: "); Serial.println(settings.temperature1);
    Serial.print("temperature2: "); Serial.println(settings.temperature2);
    Serial.print("humidity1: "); Serial.println(settings.humidity1);
    Serial.print("humidity2: "); Serial.println(settings.humidity2);
}