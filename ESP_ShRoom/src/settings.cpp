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

    preferences->putBool("isEnabled1", settings.isEnabled1);
    preferences->putBool("isEnabled2", settings.isEnabled2);
    preferences->putBool("isEnabled3", settings.isEnabled3);
    preferences->putBool("isEnabled4", settings.isEnabled4);

    preferences->putInt("onPercentage1", settings.onPercentage1);
    preferences->putInt("onPercentage2", settings.onPercentage2);
    preferences->putInt("onPercentage3", settings.onPercentage3);
    preferences->putInt("onPercentage4", settings.onPercentage4);

    preferences->putFloat("targetTemp", settings.targetTemperature);
    preferences->putFloat("hysteresis", settings.hysteresis);
    preferences->putFloat("deltaTmax", settings.deltaTmax);
    preferences->putFloat("maxHumidity", settings.maxHumidity);

    preferences->putInt("hours", settings.hours);
    preferences->putInt("minutes", settings.minutes);
    preferences->putInt("seconds", settings.seconds);

    preferences->putFloat("temperature1", settings.temperature1);
    preferences->putFloat("temperature2", settings.temperature2);
    preferences->putFloat("humidity1", settings.humidity1);
    preferences->putFloat("humidity2", settings.humidity2);

    preferences->putInt("ct_lowerHour", settings.ct_lowerHour);
    preferences->putInt("ct_lowerMinute", settings.ct_lowerMinute);
    preferences->putInt("ct_upperHour", settings.ct_upperHour);
    preferences->putInt("ct_upperMinute", settings.ct_upperMinute);

    preferences->putInt("crashcounter", settings.crashcounter);
    preferences->putInt("lastUpdateTime", settings.lastUpdateTime);

    Serial.println("Werte gespeichert!");
}

void loadSettings(Settings* settings, Preferences* preferences) {
    settings->onTime1 = preferences->getInt("onTime1", 10);
    settings->onTime2 = preferences->getInt("onTime2", 10);
    settings->onTime3 = preferences->getInt("onTime3", 10);
    settings->onTime4 = preferences->getInt("onTime4", 10);

    settings->isEnabled1 = preferences->getBool("isEnabled1", false);
    settings->isEnabled2 = preferences->getBool("isEnabled2", false);
    settings->isEnabled3 = preferences->getBool("isEnabled3", false);
    settings->isEnabled4 = preferences->getBool("isEnabled4", false);

    settings->onPercentage1 = preferences->getInt("onPercentage1", 100);
    settings->onPercentage2 = preferences->getInt("onPercentage2", 100);
    settings->onPercentage3 = preferences->getInt("onPercentage3", 100);
    settings->onPercentage4 = preferences->getInt("onPercentage4", 100);

    settings->targetTemperature = preferences->getFloat("targetTemp", 22.0);
    settings->hysteresis = preferences->getFloat("hysteresis", 0.3);
    // settings->deltaTmax = preferences->getFloat("deltaTmax", 5.0);
    // settings->maxHumidity = preferences->getFloat("maxHumidity", 60.0);

    settings->hours = preferences->getInt("hours", 0);
    settings->minutes = preferences->getInt("minutes", 0);
    settings->seconds = preferences->getInt("seconds", 0);

    //settings->temperature1 = preferences->getFloat("temperature1", 20.0);
    //settings->temperature2 = preferences->getFloat("temperature2", 20.0);
    //settings->humidity1 = preferences->getFloat("humidity1", 50.0);
    //settings->humidity2 = preferences->getFloat("humidity2", 50.0);

    settings->ct_lowerHour = preferences->getInt("ct_lowerHour", 0);
    settings->ct_lowerMinute = preferences->getInt("ct_lowerMinute", 0);
    settings->ct_upperHour = preferences->getInt("ct_upperHour", 0);
    settings->ct_upperMinute = preferences->getInt("ct_upperMinute", 0);

    settings->crashcounter = preferences->getInt("crashcounter", -1);
    settings->lastUpdateTime = preferences->getInt("lastUpdateTime", 0);

    Serial.println("Werte geladen!");
}