#include "SRVE.h"

// reads the position of the servo
// returns the position, 0-1000. 0 is 0 degrees, 1000 is 270 degrees
int SRVE::readPosition() {
    byte payload[0];
    sendCommand(LX16A_SERVO_POS_READ, payload, 0);

    byte responseBuffer[2];

    readResponse(LX16A_SERVO_POS_READ, 2, responseBuffer);

    // convert response to a signed short int
    signed short int reported_pos =
        responseBuffer[0] + (responseBuffer[1] << 8);

    this->pos = reported_pos;  // * 24;

    return this->pos;
}

/**
 * Move the servo to a position in a given time. Does NOT block.
 * @param position The position to move to, 0-1000. 0 is 0 degrees, 1000 is 270
 * degrees
 * @param time The time to move to the position in ms (0-30000). Might change
 * this to be speed.
 * @return void
 *
 */
void SRVE::move(int position, int time) {
    byte payload[3];

    // position
    payload[0] = position & 0xff;
    payload[1] = (position >> 8) & 0xff;

    debugPrintHex(payload, 2, "Move Payload to position " + String(position));

    // time
    payload[2] = time & 0xff;
    payload[3] = (time >> 8) & 0xff;

    sendCommand(LX16A_SERVO_MOVE_TIME_WRITE, payload, 4);
}

/**
 * Move the servo to a position in a given time. Blocks until the move is
 * complete. Checks the position of the servo after the move.
 */
void SRVE::moveBlocking(int position, int time,
                        boolean doReadPosition = false) {
    this->move(position, time);
    delay(time + 50);  // wait for the move to complete

    // read the position, if requested
    if (doReadPosition) {
        this->readPosition();
    }
}