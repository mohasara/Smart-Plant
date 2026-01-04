#include <Arduino.h>
#include "ConfigLoader.h"
#include "NetworkHandler.h"
#include "PlantController.h"
#include "main.h"
#include "MqttHandler.h"

ConfigLoader configLoader;
NetworkHandler networkHandler;
PlantController plantController(SENSORPIN, RELAYPIN, MINVOLTAGEOFSENSOR, MAXVOLTAGEOFSENSOR, LOWERTHRESHOLD, HIGHERTHRESHOLD); 
MqttHandler mqttHandler;

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
    mqttHandler.setup();

    Serial.println("System Initialized");
}

void loop() {
    networkHandler.checkConnection();
    mqttHandler.loop();
    // plantController.process();

    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        plantController.process();
        
        String jsonMsg = "{\"moisture\": " + String(random(20, 90)) + "}"; 
        
        Serial.print("Sending to Server: ");
        Serial.println(jsonMsg);
        
        mqttHandler.publish(jsonMsg);
    }
    // delay(1000 * 1 * 2); 
}