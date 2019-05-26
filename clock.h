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

class Clock {
public:
  //needs some variables for the RTC module, maybe also the variable of nixies.init()? Todo: figure that shit out
  void init();

  //needs to be run in a loop
  void updateTime();

  //passthrough function
  inline void setMenuState(unsigned char menuState) {
    nixies.setMenuState(menuState);
  }

  void changeTime(const char &unit, const short &delta, const bool &relative = true);
  
private:
  void loadTime();
  void setTime(const DateTime &time);

  DateTime time;
  RTC_DS3231 rtc;
  Nixies nixies;

};

#endif //CLOCK_H
