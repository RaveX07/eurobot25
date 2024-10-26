#include <math.h>
#include <Arduino.h>
#ifndef MYHEADER_H
#define MYHEADER_H


#define ENC_LEFT_A_PHASE 2
#define ENC_LEFT_B_PHASE 3
#define ENC_RIGHT_A_PHASE 18
#define ENC_RIGHT_B_PHASE 19

#define WHEELBASE_ENC 127
#define WHEELBASE_DRIVE 207
#define PULSES_PER_REV 1200
#define ENC_WHEEL_DIAMETER 50                                                                           
#define MOTOR_WHEEL_DIAMETER 70

const float encWheelScope = ENC_WHEEL_DIAMETER * M_PI;
const float pulsesPerMM = PULSES_PER_REV / encWheelScope;

int counterLeft = 0;
int counterRight = 0;

int lastCounterLeft = 0;
int lastCounterRight = 0;

float x = 0;            // x in mm
float y = 0;            // y in mm
float theta = 0;        // theta in RAD

// update ticks left
void changeLeft(){
    if(digitalRead(ENC_LEFT_B_PHASE) != digitalRead(ENC_LEFT_A_PHASE)){
        counterLeft++;
    } else {
        counterLeft --;
    }
}

// update ticks right
void changeRight(){
    if(digitalRead(ENC_RIGHT_B_PHASE) != digitalRead(ENC_RIGHT_A_PHASE)){
        counterRight++;
    } else {
        counterRight --;
    }
}

void updatePos(){
    // get tick differnce for each wheel
    float leftWheelDif = counterLeft - lastCounterLeft;
    float rightWheelDif = counterRight - lastCounterRight;

    // update last pos
    lastCounterLeft = counterLeft;
    lastCounterRight = counterRight;

    // distance per wheel
    float distanceLeft = leftWheelDif / pulsesPerMM;
    float distanceRight = rightWheelDif / pulsesPerMM;
    float distance = (distanceLeft + distanceRight) / 2;    //distance center has travelled

    // global position change
    float dtheta = (distanceLeft - distanceRight) / WHEELBASE_ENC;
    float dx = distance * cos(theta + dtheta);
    float dy = distance * sin(theta + dtheta);

    // update global pos
    x = x + dx;
    y = y + dy;
    theta = theta + dtheta;
    while (theta > 2 * M_PI) theta -= 2 * M_PI;
    while (theta < -2 * M_PI) theta += 2 * M_PI;
    if(theta < 0) theta = 2 * M_PI - theta;

    // print global pos
    Serial.print("X: ");
    Serial.println(x);
    Serial.print("Y: ");
    Serial.println(y);
    Serial.print("Theta: ");
    Serial.println(theta * (180.0 / M_PI)); // theta in DEG

    Serial.println("####################");
}

#endif