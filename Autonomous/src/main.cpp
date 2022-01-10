#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

/******************************************************************/
// L298N
int motorLPin1 = 23;   // Motor L
int motorLPin2 = 22;
int enableLPin = 32;

int motorRPin1 = 16;   // Motor R
int motorRPin2 = 17;
int enableRPin = 5;

// Setting PWM properties for ESP32
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;
int dutyCycle = 200;

// Functions Delcaration
void goLeft();
void goRight();
void goForward();
void goBackward();
void Stop();

void wheelControl(int speedL, int speedR);

/******************************************************************/

const char* ssid = "realme X3";
const char* password = "0979268400";

//Your Domain name with URL path or IP address with path
// String serverName = "http://127.0.0.1:5000/direction";
String serverName = "http://af6c-112-78-69-144.ngrok.io/direction";

/******************************************************************/

void setup() {
    //
    Serial.begin(115200);

    // Motor L
    pinMode(motorLPin1, OUTPUT);   // sets the pins as outputs:
    pinMode(motorLPin2, OUTPUT);
    pinMode(enableLPin, OUTPUT);
    ledcSetup(pwmChannel1, freq, resolution);   // configure LED PWM functionalitites
    ledcAttachPin(enableLPin, pwmChannel1);     // attach the channel to the GPIO to be controlled

    // Motor R
    pinMode(motorRPin1, OUTPUT);
    pinMode(motorRPin2, OUTPUT);
    pinMode(enableRPin, OUTPUT);
    ledcSetup(pwmChannel2, freq, resolution);
    ledcAttachPin(enableRPin, pwmChannel2);

    //
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    //
    Serial.println("Autonomous Starts ... ");
}

void loop() {
    //
    String payload = "";

    // Part 1 :
    if (WiFi.status() == WL_CONNECTED) {
        //
        HTTPClient http;
        WiFiClient client;
        String serverPath = serverName;   // + "?temperature=24.37";

        // GET from the url
        // Your Domain name with URL path or IP address with path
        http.begin(client, serverPath);

        // Send HTTP GET request
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            payload = http.getString();   // extract the payload from http request
            Serial.println(payload);
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }

        // Free resources
        http.end();

    } else {
        //
        Serial.println("WiFi Disconnected");
        return;
    }

    // Part 2 : determine the direction by the payload we get
    Serial.print("Server retrun : ");
    Serial.println(payload);

    if (payload == "0") {
        Serial.println("Forward");
        goForward();
    } else if (payload == "1") {
        goBackward();
    } else if (payload == "2") {
        Serial.println("Left");
        goLeft();
    } else if (payload == "3") {
        Serial.println("Right");
        goRight();
    } else {
        Serial.println("Stop");
        Stop();
    }

    delay(500);
}

void wheelControl(int speedL, int speedR) {
    // control Left Wheel, clockwise rotation if speed is positive,
    //  else, counter-clockwise

    int L1, L2, R1, R2;

    if (speedL > 0) {
        L1 = HIGH;
        L2 = LOW;
    } else {
        L1 = LOW;
        L2 = HIGH;
    }

    // control Right Wheel
    if (speedR > 0) {
        R1 = HIGH;
        R2 = LOW;
    } else {
        R1 = LOW;
        R2 = HIGH;
    }

    //
    digitalWrite(motorLPin1, L1);
    digitalWrite(motorLPin2, L2);
    digitalWrite(motorRPin1, R1);
    digitalWrite(motorRPin2, R2);

    // PWM Output
    ledcWrite(pwmChannel1, abs(speedL) + 160);
    ledcWrite(pwmChannel2, abs(speedR) + 160);
    delay(50);

    Stop();
}

void goBackward() {
    wheelControl(-45, -55);
}

void goForward() {
    wheelControl(45, 40);
}

void goLeft() {
    goForward();
    delay(500);
    wheelControl(0, 50);
}

void goRight() {
    goForward();
    delay(500);
    wheelControl(50, 0);
}

void Stop() {
    digitalWrite(motorLPin1, LOW);
    digitalWrite(motorLPin2, LOW);
    digitalWrite(motorRPin1, LOW);
    digitalWrite(motorRPin2, LOW);
}

/*
Reference:
    1. https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
*/