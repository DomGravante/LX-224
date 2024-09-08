#include "SRVE.h"

// Includes functions for the SRV-E class to send commands to and from the servo

byte* SRVE::sendCommand(byte command, byte* payload, byte payloadLength) {
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

    this->debugPrintHex(data, 6 + payloadLength, "Sent Command");

    return data;
}

byte SRVE::calculateChecksum(byte cmd, byte* payload, byte payloadLen) {
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

void SRVE::sendData(byte data[], int length) {
    HardwareSerial* serial = this->serial;

    // Enable the RX/TX buffer
    digitalWrite(TX_ENABLE, HIGH);

    delayMicroseconds(_74HC126_DELAY_US);
    for (int i = 0; i < length; i++) {
        serial->write(data[i]);
        delayMicroseconds(this->timeus(1));
    }
    // Re-enable the RX/TX buffer
    digitalWrite(TX_ENABLE, LOW);
    delayMicroseconds(_74HC126_DELAY_US);

    // clear the command from the buffer
    clearCommandFromBuffer(data, length);
}

byte* SRVE::readResponse(byte cmd, byte expectedPayloadLength,
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

bool SRVE::waitForDataAvailable() {
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
