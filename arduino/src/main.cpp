#include <Arduino.h>
#include "odometry.h"
// #include "pathfinder.h"

// Position start(1500, 500);
// Position target(500, 1500);

void setup() {
    Serial.begin(115200);

    pinMode(ENC_LEFT_A_PHASE, INPUT_PULLUP);
    pinMode(ENC_LEFT_B_PHASE, INPUT_PULLUP);
    pinMode(ENC_RIGHT_A_PHASE, INPUT_PULLUP);
    pinMode(ENC_RIGHT_B_PHASE, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENC_LEFT_A_PHASE), changeLeft, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_RIGHT_A_PHASE), changeRight, CHANGE);

    // Pathfinder pathfinder(start, target);
    // pathfinder.plan();
}

void loop() {
    updatePos();
    sendData();
    delay(8);
}