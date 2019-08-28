#ifndef CLOCK_H
#define CLOCK_H

#include "nixies.h"
#include "shiftRegister.h"
#include <RTClib.h>

//pins for the shift registers for the nixies (actual numbers)
/*#define NIXIES_DATA 10
#define NIXIES_CLOCK 8
#define NIXIES_LATCH 9*/
#define NIXIES_DATA 7
#define NIXIES_CLOCK 5
#define NIXIES_LATCH 6

//pins for the shift register for the special symbols (dots for nixies and colons)
/*#define SPECIALS_DATA 7
#define SPECIALS_CLOCK 5
#define SPECIALS_LATCH 6*/
#define SPECIALS_DATA 10
#define SPECIALS_CLOCK 8
#define SPECIALS_LATCH 9

//make this be in eeprom to be changed and stored in runtime
#define AUTO_ON_TIME_H 9
#define AUTO_ON_TIME_M 30
#define AUTO_OFF_TIME_H 0
#define AUTO_OFF_TIME_M 0

class Clock {
public:
  //needs some variables for the RTC module, maybe also the variable of nixies.init()? Todo: figure that shit out
  void init();

  //needs to be run in a loop
  void updateTime();

  //passthrough function
  inline void setMenuState(MenuStates menuState) {
    nixies.setMenuState(menuState);
  }
  inline void setAutomaticMode(bool on) {
    automaticMode = on;
  }

  inline MenuStates getMenuState () {
    return nixies.getMenuState();
  }
  inline bool getAutomaticMode() {
    return automaticMode;
  }

  inline void toggleNixiesOnOff() {
    nixies.toggleNixiesOnOff();
  }

  void changeTime(const char &unit, const short &delta, const bool &relative = true);
  
private:
  void loadTime();
  void setTime(const DateTime &time);

  bool automaticMode = true;
  void autoOnOff();

  DateTime time;
  RTC_DS3231 rtc;
  Nixies nixies;
};

#endif //CLOCK_H
