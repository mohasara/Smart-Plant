#include "MqttHandler.h"

MqttHandler::MqttHandler() : client(espClient) {
    clientId = "ESP32-" + String(random(0xffff), HEX);
}

void MqttHandler::setup() {
    client.setServer(mqtt_server, 1883);
}

void MqttHandler::publish(String message) {
    if (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.println(client.state());
            return;
        }
    }
    client.publish(topic, message.c_str());
}

void MqttHandler::loop() {
    if (!client.connected()) {
        client.connect(clientId.c_str()); 
    }
    client.loop();
}