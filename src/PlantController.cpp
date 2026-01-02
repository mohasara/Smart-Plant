#include "PlantController.h"

PlantController::PlantController(int sPin, int rPin, int dry, int wet, int lowTh, int highTh) {
    sensorPin = sPin;
    relayPin = rPin;
    dryValue = dry;
    wetValue = wet;
    lowThreshold = lowTh;
    highThreshold = highTh;
}

void PlantController::begin() {
    pinMode(sensorPin, INPUT);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH);
}

void PlantController::process() {

    long sum = 0;
    int samples = 50;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(sensorPin);
        delay(5);
    }
    int averageRaw = sum / samples;

    int moisturePercent = map(averageRaw, dryValue, wetValue, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100);

    Serial.print("Raw: "); //remove it at the end
    Serial.print(averageRaw); // remove it at the end
    Serial.print(" | Moisture: ");
    Serial.print(moisturePercent);
    Serial.println("%");

    if (moisturePercent < lowThreshold) {
        Serial.println("Warning: Soil is DRY! Turning on pump...");
        digitalWrite(relayPin, LOW);
        
        delay(2000);
        
        digitalWrite(relayPin, HIGH);
        Serial.println("Pump paused for absorption...");
        delay(3000); 
    } 
    else if (moisturePercent > highThreshold) {
        digitalWrite(relayPin, HIGH);
    }
}