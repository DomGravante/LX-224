#include <Arduino.h>
#include <lx16a-servo.h>

#include "MorkiteServo.h"

#define TX_ENABLE 2

#define TX 18

MorkiteServo MServo(2, TX, &Serial1, TX_ENABLE);

MorkiteServo NServo(3, TX, &Serial1, TX_ENABLE);

void setup() {
    pinMode(TX_ENABLE, OUTPUT);

    // Set up Serials
    Serial.begin(115200);   // Computer Serial
    Serial1.begin(115200);  // Servo Serial
    Serial2.begin(115200);  // Monitoring Serial

    // Set up Buffer
    digitalWrite(TX_ENABLE, HIGH);  // Start TX_ENABLE as HIGH
}

void readSerial2() {
    bool didUpdate = false;
    while (Serial2.available()) {
        didUpdate = true;
        byte receivedByte = Serial2.read();
        // Serial.print("0x");
        if (receivedByte < 0x10)
            Serial.print("0");  // For leading zero in single-digit hex values
        Serial.print(receivedByte,
                     HEX);  // Print the byte as a hexadecimal value
        Serial.print(" ");
    }

    if (didUpdate) {
        Serial.println("RX2 Data");
    }
}

void loop() {
    // Read ID COMMAND
    byte ID = MServo.readID();

    Serial.println("ID: " + String(ID));

    // MServo.setID(3);
    MServo.readPosition();
    Serial.println("ID 2 Position: " + String(MServo.pos));

    NServo.readPosition();
    Serial.println("ID 3 Position: " + String(NServo.pos));

    // Eavesdrop on the serial port from Serial 2
    readSerial2();

    // Delay
    delay(1000);
    Serial.println();
}
