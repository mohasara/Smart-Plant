#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttHandler {
private:
    const char* mqtt_server = "test.mosquitto.org"; 
    const char* topic_data = "uni/project/smartplant/data";
    const char* topic_command = "uni/project/smartplant/command";
    WiFiClient espClient;
    PubSubClient client;
    String clientId;

public:
    MqttHandler();
    void setup();
    void loop();
    void publish(String message);
    void setCallback(std::function<void(char*, uint8_t*, unsigned int)> callback);
};

#endif