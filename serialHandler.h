#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include "Arduino.h"

#define DELAY_FULL_COMMAND 50   //how long to wait for the entire command to transmit [ms]
#define MAX_COMMAND_LENGTH 20
#define MAX_PARAMETER_COUNT 4

/*
Commands:
tset h m s        //set time, parameter h, m, s
tget              //get time
auto hh:mm hh:mm  //auto mode settings, first time for auto on time, second time for auto off time, parameters hh1, mm1, hh2, mm2
nixie (on/off)     //toggle nixie display, parameters on/off
todo: animation speeds
*/

enum CommandNames {
  empty,
  tset,
  tget,
  autoMode,
  nixie
};

struct Command {
  Command(CommandNames command = CommandNames::empty);
  
  CommandNames command;
  unsigned short parameters[MAX_PARAMETER_COUNT];
};

class SerialHandler {
  public:
    Command poll();

  private:
    Command parseAutoParameters(char str[MAX_COMMAND_LENGTH - sizeof("auto")]);
};

#endif //SERIALHANDLER_H
