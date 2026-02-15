#include "main.h"

ConfigLoader configLoader;
NetworkHandler networkHandler;
PlantController plantController(SENSORPIN, WATERSENSORPIN, RELAYPIN, MINVOLTAGEOFSENSOR, MAXVOLTAGEOFSENSOR, LOWERTHRESHOLD, HIGHERTHRESHOLD);MqttHandler mqttHandler;

// const char* commandTopic = "uni/project/smartplant/command";

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.print("Command received: ");
    Serial.println(message);
    plantController.handleCommand(message);
}

void setup() {
    Serial.begin(115200);
    
    if (configLoader.begin()) {
        configLoader.loadCredentials("/wifi_creds.csv");
    }

    networkHandler.connect(configLoader.ssid, configLoader.password);

    plantController.begin();
    mqttHandler.setCallback(mqttCallback); 
    mqttHandler.setup();

    Serial.println("System Initialized (Offline Mode Ready)");
}

void loop() {
    networkHandler.checkConnection();
    mqttHandler.loop();
    // plantController.process();

    if (WiFi.status() == WL_CONNECTED) {
        mqttHandler.loop();
    }

    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        plantController.process();
                
        String jsonMsg = "{\"moisture\": " + String(plantController.lastMoisture) + 
                                ", \"empty\": " + (plantController.isTankEmpty ? "true" : "false") + "}";
        if (WiFi.status() == WL_CONNECTED) {
            Serial.print("Sending to Server: ");
            Serial.println(jsonMsg);
            mqttHandler.publish(jsonMsg);
        } else {
            Serial.println("Offline Mode: Data processed but not sent.");
        }
    }
    // delay(1000 * 1 * 2); 
}