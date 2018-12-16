#ifndef CLOCK_H
#define CLOCK_H

#include "time.h"
#include "nixies.h"


class Clock {
public:
  //needs to be run in a loop
  void updateTime();

  void changeTime(const char &unit, const short &delta, const bool &relative = true);

private:
  void loadTime();

  //Stores time
  Time time;

  //RTC object needed

  Nixies nixies;
};

#endif //CLOCK_H
