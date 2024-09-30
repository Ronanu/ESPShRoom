#include <Arduino.h>
#include "fan_control.h"
#include <Wire.h>
#include "uFire_SHT20.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "web_handlers.h"
#include "SHT20Sensor.h"
#include "Temperaturecontroller.h"
#include "esp_task_wdt.h" // Bibliothek für den Task Watchdog
#include "ClockSwitch.h"


const int relayPin1 = 25;
const int relayPin2 = 17;
const int relayPin3 = 32;
const int relayPin4 = 33;

const int pinSens1_sda = 18;
const int pinSens1_scl = 19;
const int pinSens2_sda = 21;
const int pinSens2_scl = 22;

const char* ssid = "ShRoom";
const char* password = "12345678";
// ip = http://192.168.4.1/

// Sensor-Instanzen
SHT20Sensor sht20_1(pinSens1_sda, pinSens1_scl, Wire);  // Sensor 1 an Pin 18, 19
SHT20Sensor sht20_2(pinSens2_sda, pinSens2_scl, Wire1);  // Sensor 2 an Pin 21, 22

// Globales Settings-Objekt
Settings settings;
// Preferences-Objekt, das abgespeichert wird
Preferences preferences;

WebServer server(80);

// SpecialPwmControll-Instanzen für vier Lüfter
SpecialPwmControll* relayControl1;
SpecialPwmControll* relayControl2;
SpecialPwmControll* relayControl3;
SpecialPwmControll* relayControl4;

HysteresisController hysteresis_temp_controller(&settings.targetTemperature, &settings.hysteresis);
FaultFilter faultfilter1(5);
FaultFilter faultfilter2(5);
OfflineClock offline_clock(&settings, &preferences);
//ClockSwitch light_clock_controller(&settings);


// Shared sensor data variable
sensorData sharedSensorData;

// Mutex handle
SemaphoreHandle_t mutex;
// Task function for Core 1
void taskOnCore1(void *param) {
  while (true) {
    // Read sensor data
    float temperature1 = sht20_1.getTemperature();
    float humidity1 = sht20_1.getHumidity();
    float temperature2 = sht20_2.getTemperature();
    float humidity2 = sht20_2.getHumidity();

    // Ensure sensor readings are valid (check for NaN or invalid data)
    if (isnan(temperature1) || isnan(humidity1) || isnan(temperature2) || isnan(humidity2)) {
      Serial.println("Sensor data invalid, skipping update.");
      delay(100);
      continue;
    }

    // Mutex lock for shared data
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      sharedSensorData.temperature1 = temperature1;
      sharedSensorData.humidity1 = humidity1;
      sharedSensorData.temperature2 = temperature2;
      sharedSensorData.humidity2 = humidity2;
      xSemaphoreGive(mutex);
    }
    
    delay(100);
  }
}




void setup() {
    // Initialisierung des Task Watchdog Timers
    esp_task_wdt_init(5, true);  // Timeout von 5 Sekunden, Reset beim Timeout
    esp_task_wdt_add(NULL);      // Aktuelle Task zum Watchdog hinzufügen
    // NVS Initialisierung und Werte laden
    preferences.begin("my-app", false);
    loadSettings(&settings, &preferences);

    Serial.begin(115200);
    offline_clock.startClock();
    Wire.begin(pinSens1_sda, pinSens1_scl);  // Initialisiere den Wire Bus für Sensor 1
    Wire1.begin(pinSens2_sda, pinSens2_scl); // Initialisiere den Wire1 Bus für Sensor 2
    Wire.setTimeout(100);  // Timeout auf 100 ms
    Wire1.setTimeout(100);  // Timeout auf 100 ms

    sharedSensorData.temperature1 = 0.0f;
    sharedSensorData.temperature2 = 0.0f;
    sharedSensorData.humidity1 = 0.0f;
    sharedSensorData.humidity2 = 0.0f;

        // Create the mutex
    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL) {
        Serial.println("Failed to create mutex");
        while (1); // Stop execution if mutex creation failed
    }
    
    // Start task on Core 1
    xTaskCreatePinnedToCore(
        taskOnCore1,   // Task function
        "TaskOnCore1", // Name of the task
        10000,         // Stack size
        NULL,          // Task input parameter
        1,             // Priority
        NULL,          // Task handle
        1              // Core to pin the task to (1 = Core 1)
    );

    // Konfiguration des ESP32 als Access Point
    WiFi.softAP(ssid, password);
    Serial.println("Access Point gestartet");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", [&]() { handleRoot(&settings, server); });
    server.on("/setTime", [&]() { handleSetTimePage(&settings, server); });
    server.on("/set_values", [&]() { showSetValuesPage(&settings, server); });
    server.on("/help", [&]() { showHelpPage(&settings, server); });

    server.on("/updateData", [&]() { handleUpdateData(&settings, server); });

    server.on("/updateTime", [&]() { handleSetTime(&settings, server, &preferences); });
    server.on("/update_values", [&]() { handleSetValues(&settings, server, &preferences); });
    server.begin();
    Serial.println("Webserver gestartet");

    relayControl1 = new SpecialPwmControll(&settings.isEnabled1, &settings.onTime1, &settings.onPercentage1, relayPin1);
    relayControl2 = new SpecialPwmControll(&settings.isEnabled2, &settings.onTime2, &settings.onPercentage2, relayPin2);
    relayControl3 = new SpecialPwmControll(&settings.isEnabled3, &settings.onTime3, &settings.onPercentage3, relayPin3);
    relayControl4 = new SpecialPwmControll(&settings.isEnabled4, &settings.onTime4, &settings.onPercentage4, relayPin4);

    relayControl1->initialize();
    relayControl2->initialize();
    relayControl3->initialize();
    relayControl4->initialize();
}

void loop() {
    // Webserver anfragen bearbeiten
    server.handleClient();

    unsigned long loopstart = millis();
    settings.lastUpdateTime = loopstart;

    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      // Modify sensor data safely
        settings.temperature1 = sharedSensorData.temperature1;
        settings.humidity1 = sharedSensorData.humidity1;
        settings.temperature2 = sharedSensorData.temperature2;
        settings.humidity2 = sharedSensorData.humidity2;
    }      
        // Release the mutex
        xSemaphoreGive(mutex);

    // Uhrzeit aktualisieren
    offline_clock.updateTime();

    // Regelung der Lüfter und Steckdosen
    //##############################################################################################
    //##############################################################################################
    // Heizungsregler: Steckdose 1
    // sensor value smoothing
    float lazy_temperature1 = faultfilter1.checkValue(settings.temperature1);
    float lazy_temperature2 = faultfilter2.checkValue(settings.temperature2);
    // update heating status depending on temperature1 
    settings.isEnabled3 = hysteresis_temp_controller.update(settings.isEnabled3, lazy_temperature1);

    // Lüfter 1 hängt an relais 1
    // Heitzung hängt an relais 3 --> Steckdose 1 

    settings.isEnabled1 = true;
    if (lazy_temperature2 <= settings.targetTemperature + 1) {
        settings.onPercentage1 = 0;
    } else if (lazy_temperature2 <= settings.targetTemperature + 2) {
        settings.onPercentage1 = 50;
    } else if (lazy_temperature2 <= settings.targetTemperature + 3) {
        settings.onPercentage1 = 100;
    } else if (lazy_temperature2 > settings.targetTemperature + 4)  {
        settings.onPercentage1 = 100;
        settings.isEnabled3 = false; // Temperaturbegrenzumg im Bauraum
    }

    //##############################################################################################
    //##############################################################################################


    relayControl1->update();
    relayControl2->update();
    relayControl3->update();
    relayControl4->update();

    Serial.println("\n\n\nAktualisierte Sensor- und Lüfterdaten");
    Serial.println("loop time = " + String(millis()-loopstart));
    printSettings(settings);
    
    esp_task_wdt_reset();  // Lebenszeichen senden
    delay(100);  // Kleine Pause, um die CPU zu entlasten
}