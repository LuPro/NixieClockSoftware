#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include "Arduino.h"

#define DELAY_FULL_COMMAND 50   //how long to wait for the entire command to transmit [ms]
#define MAX_COMMAND_LENGTH 20

/*
Commands:
tset h m s        //set time
tget              //get time
auto hh:mm hh:mm  //auto mode settings, first time for auto on time, second time for auto off time
nixie (on/off)     //toggle nixie display
todo: animation speeds
*/

class SerialHandler {
  public:
    void poll();

  private:
};

#endif //SERIALHANDLER_H
