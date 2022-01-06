#include <Arduino.h>

// L289N
int motorL1 = 5, motorL2 = 6;
int motorR1 = 9, motorR2 = 10;

// IR Sensor
int LIRSensor = 7;
int RIRSensor = 8;

void goLeft();
void goRight();
void goForward();
void Stop();

void wheelControl(int speedL, int speedR);

void setup() {
    // put your setup code here, to run once:
    // L289N
    pinMode(motorL1, OUTPUT);
    pinMode(motorL2, OUTPUT);
    pinMode(motorR1, OUTPUT);
    pinMode(motorR2, OUTPUT);

    // IR Sensor
    pinMode(LIRSensor, INPUT);
    pinMode(RIRSensor, INPUT);
}

int left = 0, right = 0;
void loop() {
    // put your main code here, to run repeatedly:
    left = digitalRead(LIRSensor);
    right = digitalRead(RIRSensor);
    if (left == 0 && right == 0) {
        Stop();
    } else if (left == 0 && right == 1) {
        goLeft();
    } else if (left == 1 && right == 0) {
        goRight();
    } else if (left == 1 && right == 1) {
        goForward();
    }
}

void wheelControl(int speedL, int speedR) {
    // control Left Wheel, clockwise rotation if speed is positive,
    //  else, counter-clockwise
    if (speedL > 0) {
        analogWrite(motorL1, speedL);
        analogWrite(motorL2, 0);
    } else {
        analogWrite(motorL1, 0);
        analogWrite(motorL2, -speedL);
    }

    // control Right Wheel
    if (speedR > 0) {
        analogWrite(motorR1, speedR);
        analogWrite(motorR2, 0);
    } else {
        analogWrite(motorR1, 0);
        analogWrite(motorR2, -speedR);
    }
}

void goBackward() {
    // analogWrite(motorL1, 0);
    // analogWrite(motorL2, 45);
    // analogWrite(motorR1, 0);
    // analogWrite(motorR2, 55);
    wheelControl(-45, -55);
}

void goForward() {
    // analogWrite(motorL1, 45);
    // analogWrite(motorL2, 0);
    // analogWrite(motorR1, 55);
    // analogWrite(motorR2, 0);
    wheelControl(45, 55);
}

void goLeft() {
    // analogWrite(motorL1, 0);
    // analogWrite(motorL2, 90);
    // analogWrite(motorR1, 90);
    // analogWrite(motorR2, 0);
    wheelControl(-90, 90);
    delay(50);
}

void goRight() {
    // analogWrite(motorL1, 90);
    // analogWrite(motorL2, 0);
    // analogWrite(motorR1, 0);
    // analogWrite(motorR2, 90);
    wheelControl(90, -90);
    delay(50);
}

void Stop() {
    wheelControl(0, 0);
}
