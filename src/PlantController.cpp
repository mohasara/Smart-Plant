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

    lastMoisture = moisturePercent;    
    
    Serial.print("Raw: "); //remove it at the end
    Serial.print(averageRaw); // remove it at the end
    Serial.print(" | Moisture: ");
    Serial.print(moisturePercent);
    Serial.println("%");

    if (moisturePercent < lowThreshold) {
        Serial.println("Warning: Soil is DRY! Turning on pump...");
        digitalWrite(relayPin, LOW);        

        delay(pumpDuration);
        
        digitalWrite(relayPin, HIGH);
        Serial.println("Pump paused for absorption...");
        delay(3000); 
    } 
    else if (moisturePercent > highThreshold) {
        digitalWrite(relayPin, HIGH);
    }
}

    void PlantController::updateThresholds(int low, int high) {
    lowThreshold = low;
    highThreshold = high;
    Serial.print("New Thresholds Set -> Low: ");
    Serial.print(low);
    Serial.print(", High: ");
    Serial.println(high);
}

void PlantController::handleCommand(String jsonCommand) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonCommand);
    if (error) return;

    if (doc.containsKey("low") && doc.containsKey("high")) {
        lowThreshold = doc["low"];
        highThreshold = doc["high"];
    }

    if (doc.containsKey("pTime")) {
        pumpDuration = doc["pTime"];
        Serial.print("New Pump Duration: ");
        Serial.println(pumpDuration);
    }
}