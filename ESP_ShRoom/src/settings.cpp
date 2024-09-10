#include "settings.h"
#include <Arduino.h>

void printSettings(const Settings& settings) {
    Serial.println("=== FanControlSettings ===");
    Serial.print("isEnabled1: "); Serial.println(settings.isEnabled1);
    Serial.print("isEnabled2: "); Serial.println(settings.isEnabled2);
    Serial.print("isEnabled3: "); Serial.println(settings.isEnabled3);
    Serial.print("isEnabled4: "); Serial.println(settings.isEnabled4);

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

    Serial.println("=== SensorSettings ===");
    Serial.print("temperature1: "); Serial.println(settings.temperature1);
    Serial.print("temperature2: "); Serial.println(settings.temperature2);
    Serial.print("humidity1: "); Serial.println(settings.humidity1);
    Serial.print("humidity2: "); Serial.println(settings.humidity2);

    Serial.print("lastUpdateTime: "); Serial.println(settings.lastUpdateTime);
    Serial.print("crashcounter: "); Serial.println(settings.crashcounter);
}

// ### saving Settings-Handler ###
void saveCurrentSettings(Settings settings, Preferences* preferences) {
    preferences->putInt("onTime1", settings.onTime1);
    preferences->putInt("onTime2", settings.onTime2);
    preferences->putInt("onTime3", settings.onTime3);
    preferences->putInt("onTime4", settings.onTime4);

    preferences->putInt("isEnabled1", settings.isEnabled1);
    preferences->putInt("isEnabled2", settings.isEnabled2);
    preferences->putInt("isEnabled3", settings.isEnabled3);
    preferences->putInt("isEnabled4", settings.isEnabled4);

    preferences->putInt("onPercentage1", settings.onPercentage1);
    preferences->putInt("onPercentage2", settings.onPercentage2);
    preferences->putInt("onPercentage3", settings.onPercentage3);
    preferences->putInt("onPercentage4", settings.onPercentage4);

    preferences->putFloat("targetTemp", settings.targetTemperature);
    preferences->putFloat("hysteresis", settings.hysteresis);
    preferences->putInt("hours", settings.hours);
    preferences->putInt("minutes", settings.minutes);
    preferences->putInt("seconds", settings.seconds);
    preferences->putInt("crashcounter", settings.crashcounter);
    Serial.println("Werte gespeichert!");
}

void loadSettings(Settings* settings, Preferences* preferences) {
    settings->onTime1 = preferences->getInt("onTime1");
    settings->onTime2 = preferences->getInt("onTime2", 10);
    settings->onTime3 = preferences->getInt("onTime3", 10);
    settings->onTime4 = preferences->getInt("onTime4", 10);

    settings->isEnabled1 = preferences->getInt("isEnabled1", 0);
    settings->isEnabled2 = preferences->getInt("isEnabled2", 0);
    settings->isEnabled3 = preferences->getInt("isEnabled3", 0);
    settings->isEnabled4 = preferences->getInt("isEnabled4", 0);

    settings->onPercentage1 = preferences->getInt("onPercentage1", 0);
    settings->onPercentage2 = preferences->getInt("onPercentage2", 0);
    settings->onPercentage3 = preferences->getInt("onPercentage3", 0);
    settings->onPercentage4 = preferences->getInt("onPercentage4", 0);

    settings->targetTemperature = preferences->getFloat("targetTemp", 22.0);
    settings->hysteresis = preferences->getFloat("hysteresis", 0.5);

    settings->hours = preferences->getInt("hours", 0);
    settings->minutes = preferences->getInt("minutes", 0);
    settings->seconds = preferences->getInt("seconds", 0);

    settings->crashcounter = preferences->getInt("crashcounter", -1);

    Serial.println("Werte geladen!");
}