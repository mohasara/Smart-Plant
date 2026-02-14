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
    client.publish(topic_data, message.c_str());
}

void MqttHandler::loop() {
    if (!client.connected()) {
        client.connect(clientId.c_str());
        if (client.connect(clientId.c_str())) {
            Serial.println("MQTT Connected");
            client.subscribe("uni/project/smartplant/command"); 
        client.subscribe(topic_command);
            Serial.println("Subscribed to command topic.");
            
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000); 
            return;
        }
    }
    client.loop();
}

void MqttHandler::setCallback(std::function<void(char*, uint8_t*, unsigned int)> callback) {
    client.setCallback(callback);
}