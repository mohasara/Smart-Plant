#include <Arduino.h>
#include "ConfigLoader.h"
#include "NetworkHandler.h"
#include "PlantController.h"
#include "MqttHandler.h"

int SENSORPIN = 34;
int RELAYPIN = 5;
int MINVOLTAGEOFSENSOR = 4095;
int MAXVOLTAGEOFSENSOR = 700;
int LOWERTHRESHOLD = 30;
int HIGHERTHRESHOLD = 70;

unsigned long previousMillis = 0;
const long interval = 5000;
