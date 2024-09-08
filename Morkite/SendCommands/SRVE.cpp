#include "SRVE.h"

// time returns the number of ms to TX/RX n characters
uint32_t SRVE::timems(uint32_t n) {
    return n * 10 * 1000 / this->_baud;  // 10 bits per char
}

uint32_t SRVE::timeus(uint32_t n) {
    return n * 10 * 1000000 / this->_baud;  // 10 bits per char
}

/** Constructor
 * @param id The ID of the servo.
 * @param TX The TX pin (int).
 * @param serial The RX pin (int).
 * @param TX_ENABLE The TX enable pin (int).
 * @return A new SRVE object.
 */
SRVE::SRVE(int id, int TX, HardwareSerial* serial, int TX_ENABLE) {
    this->ID = id;
    this->TX = TX;
    this->serial = serial;
    this->TX_ENABLE = TX_ENABLE;
}

void SRVE::setID(byte newID) {
    // set the ID to this servo to the broadcast ID
    this->ID = LX16A_BROADCAST_ID;

    // Send the new ID to the servo.
    sendCommand(LX16A_SERVO_ID_WRITE, &newID, 1);

    // set the ID to the new ID
    this->ID = newID;

    // return void
    return;
}

int SRVE::readID() {
    byte payload[0];
    sendCommand(LX16A_SERVO_ID_READ, payload, 0);

    byte responseBuffer[1];

    readResponse(LX16A_SERVO_POS_READ, 1, responseBuffer);

    // response is set to the responseBuffer. set to this ID
    this->ID = responseBuffer[0];

    // return the ID
    return this->ID;
}

byte* SRVE::readAvailable() {
    HardwareSerial* serial = this->serial;

    byte response[10];
    int i = 0;

    while (serial->available() > 0) {
        // get the byte
        byte receivedByte = serial->read();
        if (receivedByte < 0x10)
            Serial.print("0");  // For leading zero in single-digit hex values
        response[i] = receivedByte;
        Serial.print(response[i], HEX);
        Serial.print(" ");
        i++;
    }

    return response;
}

void SRVE::clearCommandFromBuffer(byte* array, int length) {
    HardwareSerial* serial = this->serial;

    bool isDataAvailable = waitForDataAvailable();

    if (!isDataAvailable) return;

    // read the data
    // set an index
    int i = 0;

    while (serial->available() > 0) {
        byte receivedByte = serial->read();

        // check that the byte matches the given array in that position
        if (receivedByte == array[i]) {
            i++;

        } else {
            // if that doesn't work, cancel out
            return;
        }

        // if we've reached the end of the array, return
        if (i == length) {
            return;
        }
    }
}
