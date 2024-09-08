#include "SRVE.h"

// Includes functions for the SRV-E class to debug print stuff

// set the debug flag. this will print debug messages if set to true
void SRVE::setDebug(bool debug) { this->debug = debug; }

String hexBytesToString(byte* data, int length) {
    String result = "";
    for (int i = 0; i < length; i++) {
        if (data[i] < 0x10)
            result += "0";  // For leading zero in single-digit hex values

        result += String(data[i], HEX);
        result += " ";
    }
    result.toUpperCase();  // Convert the result to uppercase

    return result;
}

// print a debug message to console if debug is set to true
void SRVE::debugPrint(String message) {
    if (this->debug) Serial.println(message);
}

// print a debug message to console if debug is set to true, along with some hex
void SRVE::debugPrintHex(byte* data, int length, String message) {
    if (!this->debug) return;

    Serial.println(message + ": " + hexBytesToString(data, length));
}