#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <WiFi.h>

class NetworkHandler {
public:
    void connect(String ssid, String password);
    void checkConnection();
};

#endif