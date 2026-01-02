#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <Arduino.h>
#include <LittleFS.h>

class ConfigLoader {
public:
    String ssid;
    String password;

    bool begin();
    bool loadCredentials(const char* filename);
};

#endif