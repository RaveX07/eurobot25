#include <Arduino.h>

#define ENC_LEFT_A_PHASE 2
#define ENC_LEFT_B_PHASE 3
#define ENC_RIGHT_A_PHASE 18
#define ENC_RIGHT_B_PHASE 19

int counterLeft = 0;
int counterRight = 0;

int loops = 0;
long lastMillis = millis();

void changeLeft(){
    if(digitalRead(ENC_LEFT_B_PHASE) != digitalRead(ENC_LEFT_A_PHASE)){
        counterLeft++;
    } else {
        counterLeft --;
    }
}

void changeRight(){
    if(digitalRead(ENC_RIGHT_B_PHASE) != digitalRead(ENC_RIGHT_A_PHASE)){
        counterRight++;
    } else {
        counterRight --;
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(ENC_LEFT_A_PHASE, INPUT_PULLUP);
    pinMode(ENC_LEFT_B_PHASE, INPUT_PULLUP);
    pinMode(ENC_RIGHT_A_PHASE, INPUT_PULLUP);
    pinMode(ENC_RIGHT_B_PHASE, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ENC_LEFT_A_PHASE), changeLeft, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_RIGHT_A_PHASE), changeRight, CHANGE);
}

void loop() {
    Serial.print("Counter Left: ");
    Serial.println(counterLeft);
    Serial.print("Counter Right: ");
    Serial.println(counterRight);

    delay(100);
}
