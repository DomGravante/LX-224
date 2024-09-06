#include "MorkiteServo.h"

uint32_t _baud = 115200;
unsigned int _74HC126_DELAY_US = 1;  // 1us  // actualy something like 315ns

// time returns the number of ms to TX/RX n characters
uint32_t time(uint32_t n) {
    return n * 10 * 1000 / _baud;  // 10 bits per char
}

uint32_t timeus(uint32_t n) {
    return n * 10 * 1000000 / _baud;  // 10 bits per char
}

MorkiteServo::MorkiteServo(int id, int TX, HardwareSerial* serial,
                           int TX_ENABLE) {
    this->ID = id;
    this->TX = TX;
    this->serial = serial;
    this->TX_ENABLE = TX_ENABLE;
}

void MorkiteServo::setID(byte newID) {
    // set the ID to this servo to the broadcast ID
    this->ID = LX16A_BROADCAST_ID;

    // Send the new ID to the servo.
    sendCommand(LX16A_SERVO_ID_WRITE, &newID, 1);

    // set the ID to the new ID
    this->ID = newID;

    // return void
    return;
}

int MorkiteServo::readID() {
    byte payload[0];
    sendCommand(LX16A_SERVO_ID_READ, payload, 0);

    byte responseBuffer[1];

    readResponse(LX16A_SERVO_POS_READ, 1, responseBuffer);

    // response is set to the responseBuffer. set to this ID
    this->ID = responseBuffer[0];

    // return the ID
    return this->ID;
}

byte* MorkiteServo::sendCommand(byte command, byte* payload,
                                byte payloadLength) {
    byte data[6 + payloadLength];

    // Header
    data[0] = 0x55;
    data[1] = 0x55;

    // ID
    data[2] = ID;

    // Data length
    data[3] = 3 + payloadLength;

    // Command
    data[4] = command;

    // payload
    for (int i = 0; i < payloadLength; i++) {
        data[5 + i] = payload[i];
    }

    // checksum
    data[5 + payloadLength] =
        calculateChecksum(command, payload, payloadLength);

    sendData(data, 6 + payloadLength);

    return data;
}

byte MorkiteServo::calculateChecksum(byte cmd, byte* payload, byte payloadLen) {
    // datalen is the length of the payload, plus 3 (ID, DataLen, Cmd)
    byte id = this->ID;

    byte dataLen = 3 + payloadLen;

    // add ID, length, and cmd to checksum
    byte checksum = id + dataLen + cmd;

    // add payload to checksum
    for (int i = 0; i < payloadLen; i++) {
        checksum += payload[i];
    }
    // return the negation of the checksum
    return ~checksum;
}

void MorkiteServo::sendData(byte data[], int length) {
    HardwareSerial* serial = this->serial;

    // Enable the RX/TX buffer
    digitalWrite(TX_ENABLE, HIGH);

    delayMicroseconds(_74HC126_DELAY_US);
    for (int i = 0; i < length; i++) {
        serial->write(data[i]);
        delayMicroseconds(timeus(1));
    }
    // Re-enable the RX/TX buffer
    digitalWrite(TX_ENABLE, LOW);
    delayMicroseconds(_74HC126_DELAY_US);

    // clear the command from the buffer
    clearCommandFromBuffer(data, length);
}

byte* MorkiteServo::readAvailable() {
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

int MorkiteServo::readPosition() {
    byte payload[0];
    sendCommand(LX16A_SERVO_POS_READ, payload, 0);

    byte responseBuffer[2];

    readResponse(LX16A_SERVO_POS_READ, 2, responseBuffer);

    // convert response to a signed short int
    signed short int reported_pos =
        responseBuffer[0] + (responseBuffer[1] << 8);

    // convert to centdegrees

    this->pos = reported_pos * 24;

    return this->pos;
}

byte* MorkiteServo::readResponse(byte cmd, byte expectedPayloadLength,
                                 byte* responseBuffer) {
    HardwareSerial* serial = this->serial;

    // maximum times to loop
    bool isDataAvailable = waitForDataAvailable();

    if (!isDataAvailable) return;

    // byte response[expectedPayloadLength];

    // read the data
    int i = 0;
    int i_payload = 0;
    int index_checksum = 6 + expectedPayloadLength - 1;  //-
    // 1;  // 6 bytes for header(x2), ID, data length, command, checksum
    if (this->debug) Serial.println("Reading Response: ");

    while (serial->available() > 0) {
        byte receivedByte = serial->read();
        if (this->debug) Serial.print(receivedByte, HEX);
        delayMicroseconds(timeus(1));

        // naive approach: ignore bytes other than payload
        if (i < 5) {
            i++;
            if (this->debug) Serial.print("I ");
            continue;
        }

        // either payload or checksum
        if (i == index_checksum) {
            if (this->debug) Serial.print("C ");
            if (this->debug) Serial.println();
            return responseBuffer;
        }

        // payload
        responseBuffer[i_payload] = receivedByte;
        i++;
        i_payload++;
        if (this->debug) Serial.print("P ");
    }
}

bool MorkiteServo::waitForDataAvailable() {
    HardwareSerial* serial = this->serial;

    // maximum times to loop
    int maxLoops = 100;  // 100x * 10ms = 1s

    delayMicroseconds(timeus(2));

    // wait for data to be available
    while (serial->available() == 0) {
        delay(10);
        Serial.print(".");
        maxLoops--;
        // return false if we've waited too long
        if (maxLoops == 0) {
            return false;
        }
    }

    return true;
}

void MorkiteServo::clearCommandFromBuffer(byte* array, int length) {
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
