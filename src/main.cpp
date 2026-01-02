#include <Arduino.h>
#include "ConfigLoader.h"
#include "NetworkHandler.h"
#include "PlantController.h"
#include "main.h"

ConfigLoader configLoader;
NetworkHandler networkHandler;
PlantController plantController(SENSORPIN, RELAYPIN, MINVOLTAGEOFSENSOR, MAXVOLTAGEOFSENSOR, LOWERTHRESHOLD, HIGHERTHRESHOLD); 

void setup() {
    Serial.begin(115200);
    
    if (configLoader.begin()) {
        if (!configLoader.loadCredentials("/wifi_creds.csv")) {
            Serial.println("Failed to load WiFi creds. Halting.");
            while(1); //STOP
        }
    }

    networkHandler.connect(configLoader.ssid, configLoader.password);

    plantController.begin();

    Serial.println("System Initialized");
}

void loop() {

    networkHandler.checkConnection();

    plantController.process();

    delay(1000 * 1 * 2); 
}