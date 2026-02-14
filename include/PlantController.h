#ifndef PLANTCONTROLLER_H
#define PLANTCONTROLLER_H

#include <Arduino.h>
#include <ArduinoJson.h>

class PlantController {
private:
    int sensorPin;
    int relayPin;
    int dryValue;
    int wetValue;
    int lowThreshold;
    int highThreshold;

public:
    int lastMoisture = 0;
    int pumpDuration = 2000;  
    PlantController(int sPin, int rPin, int dry, int wet, int lowTh, int highTh);
    void updateThresholds(int low, int high);
    void begin();
    void process();
    void handleCommand(String jsonCommand);
};

#endif