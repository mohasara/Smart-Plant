#include "NetworkHandler.h"

void NetworkHandler::connect(String ssid, String password) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid.c_str(), password.c_str());
}

void NetworkHandler::checkConnection() {
    if(WiFi.status() != WL_CONNECTED) {
        static unsigned long lastLog = 0;
        if (millis() - lastLog > 5000) {
            lastLog = millis();
            Serial.println("WiFi Disconnected. Reconnecting in background...");
            WiFi.reconnect();
        }
    }
}