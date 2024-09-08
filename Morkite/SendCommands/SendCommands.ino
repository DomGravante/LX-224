#include <Arduino.h>
#include <lx16a-servo.h>

#include "SRVE.h"

#define TX_ENABLE 2

#define TX 18

SRVE MServo(2, TX, &Serial1, TX_ENABLE);

SRVE NServo(3, TX, &Serial1, TX_ENABLE);

void setup() {
    pinMode(TX_ENABLE, OUTPUT);

    // Set up Serials
    Serial.begin(115200);   // Computer Serial
    Serial1.begin(115200);  // Servo Serial
    Serial2.begin(115200);  // Monitoring Serial

    // Set up Buffer
    digitalWrite(TX_ENABLE, HIGH);  // Start TX_ENABLE as HIGH

    // MServo.setDebug(true);
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
    // // read position
    MServo.move(0, 1000);
    delay(550);
    NServo.move(0, 1000);
    delay(550);
    // MServo.readPosition();
    // Serial.println("ID 2 Position: " + String(MServo.pos));

    MServo.move(1000, 1000);
    delay(550);
    NServo.move(1000, 1000);
    delay(550);

    // MServo.readPosition();
    // Serial.println("ID 2 Position: " + String(MServo.pos));

    // readSerial2();

    // Read ID COMMAND
    // Serial.print("Startign");
    // MServo.move(0, 1000);
    // delay(1000);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 000: " + String(MServo.pos));

    // MServo.move(100, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 100: " + String(MServo.pos));

    // MServo.move(200, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 200: " + String(MServo.pos));

    // MServo.move(300, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 300: " + String(MServo.pos));

    // MServo.move(400, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 400: " + String(MServo.pos));

    // MServo.move(500, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 500: " + String(MServo.pos));

    // MServo.move(600, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 600: " + String(MServo.pos));

    // MServo.move(700, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 700: " + String(MServo.pos));

    // MServo.move(800, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 800: " + String(MServo.pos));

    // MServo.move(900, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 900: " + String(MServo.pos));

    // MServo.move(1000, 100);
    // delay(500);
    // // MServo.readPosition();
    // // delay(1500);
    // Serial.println("ID 2 Position: 1000: " + String(MServo.pos));

    // byte ID = MServo.readID();

    // Serial.println("ID: " + String(ID));

    // // MServo.setID(3);
    // MServo.readPosition();
    // Serial.println("ID 2 Position: " + String(MServo.pos));

    // NServo.readPosition();
    // Serial.println("ID 3 Position: " + String(NServo.pos));

    // MServo.move(0, 1000);
    // delay(1000);
    // MServo.move(2000, 1000);

    // Eavesdrop on the serial port from Serial 2
    // readSerial2();

    // // Delay
    // delay(5000);
    // Serial.println();
}
