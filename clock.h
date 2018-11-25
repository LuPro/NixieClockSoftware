#ifndef CLOCK_H
#define CLOCK_H

#define NR_TUBES 6

#include "time.h"
#include "nixie.h"

class Clock {
public:
  void showTime();

private:
  void loadTime();

  //RTC object
  Time time;

  //tubes stored from highest value digit to lowest (hh:mm:ss)
  Nixie tubes[NR_TUBES];
};

#endif //CLOCK_H
