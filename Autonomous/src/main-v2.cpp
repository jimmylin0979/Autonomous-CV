// /*********
//   Rui Santos
//   Complete project details at https://randomnerdtutorials.com
// *********/
// #include <Arduino.h>

// // Motor A
// int motor1Pin1 = 23;
// int motor1Pin2 = 22;
// int enable1Pin = 32;

// // Motor B
// int motor2Pin1 = 16;
// int motor2Pin2 = 17;
// int enable2Pin = 5;

// // Setting PWM properties
// const int freq = 30000;
// const int pwmChannel1 = 0;
// const int pwmChannel2 = 1;
// const int resolution = 8;
// int dutyCycle = 200;

// void setup() {
//     // sets the pins as outputs:
//     pinMode(motor1Pin1, OUTPUT);
//     pinMode(motor1Pin2, OUTPUT);
//     pinMode(enable1Pin, OUTPUT);

//     // configure LED PWM functionalitites
//     ledcSetup(pwmChannel1, freq, resolution);

//     // attach the channel to the GPIO to be controlled
//     ledcAttachPin(enable1Pin, pwmChannel1);

//     pinMode(motor2Pin1, OUTPUT);
//     pinMode(motor2Pin2, OUTPUT);
//     pinMode(enable2Pin, OUTPUT);
//     ledcSetup(pwmChannel2, freq, resolution);
//     ledcAttachPin(enable2Pin, pwmChannel2);

//     Serial.begin(115200);

//     // testing
//     Serial.print("Testing DC Motor...");
// }

// void loop() {
//     // // Move the DC motor forward at maximum speed
//     // Serial.println("Moving Forward");
//     // digitalWrite(motor1Pin1, LOW);
//     // digitalWrite(motor1Pin2, HIGH);
//     // delay(2000);

//     // // Move DC motor backwards at maximum speed
//     // Serial.println("Moving Backwards");
//     // digitalWrite(motor1Pin1, HIGH);
//     // digitalWrite(motor1Pin2, LOW);
//     // delay(2000);

//     // // Stop the DC motor
//     // Serial.println("Motor stopped");
//     // digitalWrite(motor1Pin1, LOW);
//     // digitalWrite(motor1Pin2, LOW);
//     // delay(1000);

//     // Move DC motor forward with increasing speed
//     digitalWrite(motor1Pin1, HIGH);
//     digitalWrite(motor1Pin2, LOW);
//     digitalWrite(motor2Pin1, LOW);
//     digitalWrite(motor2Pin2, HIGH);
//     while (dutyCycle <= 255) {
//         ledcWrite(pwmChannel1, dutyCycle);
//         ledcWrite(pwmChannel2, dutyCycle);
//         Serial.print("Forward with duty cycle: ");
//         Serial.println(dutyCycle);
//         dutyCycle = dutyCycle + 5;
//         delay(500);
//     }
//     dutyCycle = 200;

//     // Move DC motor forward with increasing speed
//     digitalWrite(motor1Pin1, LOW);
//     digitalWrite(motor1Pin2, HIGH);
//     digitalWrite(motor2Pin1, HIGH);
//     digitalWrite(motor2Pin2, LOW);
//     while (dutyCycle <= 255) {
//         ledcWrite(pwmChannel1, dutyCycle);
//         ledcWrite(pwmChannel2, dutyCycle);
//         Serial.print("Backward with duty cycle: ");
//         Serial.println(dutyCycle);
//         dutyCycle = dutyCycle + 5;
//         delay(500);
//     }
//     dutyCycle = 200;
// }