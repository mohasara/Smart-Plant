#include "ConfigLoader.h"

bool ConfigLoader::begin() {

    if (!LittleFS.begin(true)) {
        Serial.println("Error mounting LittleFS");
        return false;
    }
    return true;
}

bool ConfigLoader::loadCredentials(const char* filename) {
    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.println("Failed to open config file");
        return false;
    }

    String line = file.readStringUntil('\n');
    file.close();

    line.trim(); 

    int commaIndex = line.indexOf(',');
    if (commaIndex == -1) {
        Serial.println("Invalid CSV format");
        return false;
    }

    ssid = line.substring(0, commaIndex);
    password = line.substring(commaIndex + 1);

    Serial.println("Config Loaded from CSV.");
    return true;
}