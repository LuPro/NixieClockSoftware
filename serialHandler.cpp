#include "serialHandler.h"

void SerialHandler::poll() {
  static unsigned long timeFirstByte = 0;
  static bool firstByte = true;
  
  if (Serial.available() && firstByte == true) {
    timeFirstByte = millis();
    firstByte = false;
  }

  if (firstByte == false && millis() - timeFirstByte > DELAY_FULL_COMMAND) {
    char inData[MAX_COMMAND_LENGTH];

    char i = 0;
    while (Serial.available() && i < MAX_COMMAND_LENGTH) {
      inData[i] = Serial.read();
      i++;
    }
    for (unsigned char n = 0; n < i; n++) {
      Serial.print(inData[n]);
    }
    Serial.println();
    if (strncmp("tset", inData, 4) == 0) {
      Serial.println("Setting time");
    }
    else if (strncmp("tget", inData, 4) == 0) {
      Serial.println("Getting time");
    }
    else if (strncmp("auto", inData, 4) == 0) {
      Serial.println("Setting auto mode");
    }
    else if (strncmp("nixie", inData, 5) == 0) {
      Serial.println("Toggling nixie display");
    } else {
      Serial.println("Command not recognized");
    }

    firstByte = true;
    for (; i >= 0; i--) {
      inData[i] = 0;
    }
  }
}
