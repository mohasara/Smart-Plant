#include "NetworkHandler.h"

void NetworkHandler::connect(String ssid, String password) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void NetworkHandler::checkConnection() {
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi Lost! Reconnecting...");
        WiFi.disconnect();
        WiFi.reconnect();
    }
}