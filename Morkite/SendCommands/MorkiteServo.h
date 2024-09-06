
#ifndef MorkiteServo_H
#define MorkiteServo_H

#include <Arduino.h>

#define LX16A_BROADCAST_ID 0xFE  // 254 broadcast ID
#define LX16A_SERVO_MOVE_TIME_WRITE 1
#define LX16A_SERVO_MOVE_TIME_READ 2
#define LX16A_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LX16A_SERVO_MOVE_TIME_WAIT_READ 8
#define LX16A_SERVO_MOVE_START 11
#define LX16A_SERVO_MOVE_STOP 12
#define LX16A_SERVO_ID_WRITE 13
#define LX16A_SERVO_ID_READ 14
#define LX16A_SERVO_ANGLE_OFFSET_ADJUST 17
#define LX16A_SERVO_ANGLE_OFFSET_WRITE 18
#define LX16A_SERVO_ANGLE_OFFSET_READ 19
#define LX16A_SERVO_ANGLE_LIMIT_WRITE 20
#define LX16A_SERVO_ANGLE_LIMIT_READ 21
#define LX16A_SERVO_VIN_LIMIT_WRITE 22
#define LX16A_SERVO_VIN_LIMIT_READ 23
#define LX16A_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define LX16A_SERVO_TEMP_MAX_LIMIT_READ 25
#define LX16A_SERVO_TEMP_READ 26
#define LX16A_SERVO_VIN_READ 27
#define LX16A_SERVO_POS_READ 28
#define LX16A_SERVO_OR_MOTOR_MODE_WRITE 29
#define LX16A_SERVO_OR_MOTOR_MODE_READ 30
#define LX16A_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define LX16A_SERVO_LOAD_OR_UNLOAD_READ 32
#define LX16A_SERVO_LED_CTRL_WRITE 33
#define LX16A_SERVO_LED_CTRL_READ 34
#define LX16A_SERVO_LED_ERROR_WRITE 35
#define LX16A_SERVO_LED_ERROR_READ 36

class MorkiteServo {
   public:
    int ID;                  // ID of the servo, 1-253
    int TX;                  // TX pin
    HardwareSerial* serial;  // serial
    int TX_ENABLE;           // TX enable pin

    byte position[2];  // Position of the servo
    int pos;

    bool debug = false;

    /** Constructor
     * @param id The ID of the servo.
     * @param TX The TX pin (int).
     * @param serial The RX pin (int).
     * @param TX_ENABLE The TX enable pin (int).
     * @return A new MorkiteServo object.
     */
    MorkiteServo(int id, int TX, HardwareSerial* serial, int TX_ENABLE);

    /**
     * Sets the ID of the servo.
     *
     * @param newID The new ID to set.
     */
    void setID(byte newID);

    byte readID();

    byte* readAvailable();

    int readPosition();

   private:
    byte* sendCommand(byte command, byte* payload, byte payloadLength);

    // Function to send data
    void sendData(byte data[], int length);

    /**
     * Calculates the checksum for a command with the given payload.
     *
     * @param cmd The command byte.
     * @param payload The payload bytes.
     * @param payloadLen The length of the payload.
     * @param customID The ID byte. Optional, defaults to the servo ID.
     * @return The calculated checksum byte.
     */
    byte calculateChecksum(byte cmd, byte* payload, byte payloadLen);

    void clearCommandFromBuffer(byte* array, int length);

    byte* readResponse(byte cmd, byte expectedPayloadLength,
                       byte* responseBuffer);

    bool waitForDataAvailable();
};

#endif