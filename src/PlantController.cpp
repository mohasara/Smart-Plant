#include "PlantController.h"

PlantController::PlantController(int sPin, int wPin, int rPin, int dry, int wet, int lowTh, int highTh) {
    sensorPin = sPin;
    waterSensorPin = wPin; // *** جدید ***
    relayPin = rPin;
    dryValue = dry;
    wetValue = wet;
    lowThreshold = lowTh;
    highThreshold = highTh;
}

void PlantController::begin() {
    pinMode(sensorPin, INPUT);
    pinMode(waterSensorPin, INPUT);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH);
}

void PlantController::process() {
    long sum = 0;
    for (int i = 0; i < 20; i++) {
        sum += analogRead(sensorPin);
        delay(2);
    }
    int avgSoil = sum / 20;
    int soilPercent = map(avgSoil, dryValue, wetValue, 0, 100);
    soilPercent = constrain(soilPercent, 0, 100);
    lastMoisture = soilPercent;

    int waterRaw = analogRead(waterSensorPin);

    if (waterRaw > 3000) {
        isTankEmpty = true;
    } else {
        isTankEmpty = false;
    }

    Serial.print("Soil: "); Serial.print(soilPercent); Serial.print("% | ");
    Serial.print("Tank Raw: "); Serial.print(waterRaw);
    if(isTankEmpty) Serial.print(" (EMPTY!)");
    Serial.println();

    if (soilPercent < lowThreshold) {
        if (isTankEmpty) {
            Serial.println("Error: Soil is DRY but Tank is EMPTY! Pump blocked.");
        } else {
            Serial.println("Soil is DRY & Tank OK. Watering...");
            digitalWrite(relayPin, LOW);
            delay(pumpDuration);
            digitalWrite(relayPin, HIGH);
            Serial.println("Pump finished.");
            delay(2000); 
        }
    } 
    else if (soilPercent > highThreshold) {
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