#ifndef NIXIES_H
#define NIXIES_H

#include "RTClib.h"
#include "Arduino.h"
#include "shiftRegister.h"
#include "animations.h"

#define NR_TUBES 6
#define MAX_TUBES 6

enum MenuStates {
  noMenu,
  setSeconds,
  setMinutes,
  setHours
};

class Nixies {
public:
  //sets the pins needed for the shift register chain and initializes the pins as needed, all of those pins just need digital out capabilities (Todo: shiftOutEn and shiftClear might not be needed, check for this later on)
  void init(ShiftRegister nixieRegister, ShiftRegister specialsRegister);

  void showTime(const DateTime &time);

  inline void setMenuState(MenuStates state) {
    menuState = state;
  }

private:
  //generates a binary string of time data to be sent to the chained shift registers
  //format is ss:mm:hh, hours at lowest values. If change to just 4 tubes code can still be used and seconds just shift into nothingness (seconds get lost)
  //each unit (digit pair) gets 8 bit, resulting string is 24 bit long
  unsigned long generateTimeString(const DateTime &time);

  //takes the generated time string from generateTimeString (in a ss:mm:hh format) and outputs it into the shift registers
  //it starts with lowest bits
  void sendTimeToRegister(unsigned long timeString);

  void updateAnimations();

  //Pin for turning the high powered output (nixie power) from the power supply off
  unsigned char nixiePowerPin = 12;

  ShiftRegister *nixieRegister;
  ShiftRegister *specialsRegister;
  
  Animations animator;
  
  MenuStates menuState = noMenu;

  //stores the time string of the current displayed numbers on the nixies, used to cross check if there are changes in new time
  unsigned long currentTimeString; 

  inline unsigned char getLow(unsigned char value) const {
    return value % 10;
  }
  inline unsigned char getHigh(unsigned char value) const {
    return value / 10;
  }
};

#endif //NIXIES_H
