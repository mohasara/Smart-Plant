#ifndef PLANTCONTROLLER_H
#define PLANTCONTROLLER_H

#include <Arduino.h>

class PlantController {
private:
    int sensorPin;
    int relayPin;
    int dryValue;
    int wetValue;
    int lowThreshold;
    int highThreshold;

public:
    PlantController(int sPin, int rPin, int dry, int wet, int lowTh, int highTh);
    
    void begin();
    void process();
};

#endif