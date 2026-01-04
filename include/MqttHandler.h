#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttHandler {
private:
    const char* mqtt_server = "broker.emqx.io";
    const char* topic = "uni/project/smartplant/data";
    WiFiClient espClient;
    PubSubClient client;
    String clientId;

public:
    MqttHandler();
    void setup();
    void loop();
    void publish(String message);
};

#endif