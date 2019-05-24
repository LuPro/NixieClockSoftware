#ifndef CLOCK_H
#define CLOCK_H

#include "time.h"
#include "nixies.h"
#include <RTClib.h>

class Clock {
public:
  //needs some variables for the RTC module, maybe also the variable of nixies.init()? Todo: figure that shit out
  void init();

  //needs to be run in a loop
  void updateTime();

  void changeTime(const char &unit, const short &delta, const bool &relative = true);

  void test();

private:
  void loadTime();

  void setTime(const DateTime &time);

  DateTime time;

  RTC_DS3231 rtc;

  Nixies nixies;
};

#endif //CLOCK_H
