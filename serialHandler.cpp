#include "serialHandler.h"

Command::Command(CommandNames command) :
command(command)
{
  
}

Command SerialHandler::poll() {
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
      return Command(CommandNames::tget);
    }
    else if (strncmp("auto", inData, 4) == 0) {
      Serial.println("Setting auto mode");
      return parseAutoParameters(&inData[sizeof("auto")]); //ToDo: validate if this is right once back with the hardware
    }
    else if (strncmp("nixie", inData, 5) == 0) {
      Serial.println("Toggling nixie display");
      return Command(CommandNames::nixie);
    } else {
      Serial.println("Command not recognized");
      return Command(CommandNames::empty);
    }

    firstByte = true;
    for (; i >= 0; i--) {
      inData[i] = 0;
    }
  }
}

Command SerialHandler::parseAutoParameters(char str[MAX_COMMAND_LENGTH - sizeof("auto")]) {
  unsigned char len = strlen(str);

  unsigned char strPos = strstr(str, ":");
  if (strPos != 0) {
    unsigned char strPos2 = strstr(&str[strPos], ":");
    if (strPos2 != 0) {
      if (strPos2 - strPos >= 4 && strPos >= 2 && strPos2 + 2 < len) {    //check if there's even enough space between and around the colons to fit the data needed
        unsigned short hoursOn, minutesOn, hoursOff, minutesOff;
        char timeStr[3];
        timeStr[2] = 0;

        //not very nice, too much copied code - is there some more elegant way?
        timeStr[0] = str[strPos - 2];
        timeStr[1] = str[strPos - 1];
        hoursOn = atoi(timeStr);

        timeStr[0] = str[strPos + 1];
        timeStr[1] = str[strPos + 2];
        minutesOn = atoi(timeStr);
        
        timeStr[0] = str[strPos2 - 2];
        timeStr[1] = str[strPos2 - 1];
        hoursOff = atoi(timeStr);

        timeStr[0] = str[strPos2 + 1];
        timeStr[1] = str[strPos2 + 2];
        minutesOff = atoi(timeStr);

        Command command(CommandNames::autoMode);
        command.parameters[0] = hoursOn;
        command.parameters[1] = minutesOn;
        command.parameters[2] = hoursOff;
        command.parameters[3] = minutesOff;
        return command;
        
      } else {
        Serial.println("Parameters invalid, not enough numbers for times");
      }
    } else {
      Serial.println("Parameters invalid, didn't find two colons");
    }
  } else {
    Serial.println("Parameters invalid, didn't find any colons");
  }
}
