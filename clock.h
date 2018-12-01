#ifndef CLOCK_H
#define CLOCK_H

#define NR_TUBE_PAIRS 3
#define MAX_TUBE_PAIRS 3

#define NR_TUBES (NR_TUBE_PAIRS * 2)
#define MAX_DIGITS (MAX_TUBE_PAIRS * 2)

#include "time.h"
#include "nixiepair.h"


class Clock {
public:
  void showTime();

  void changeTime(const char &unit, const short &delta, const bool &relative = true);

private:
  void loadTime();

  //RTC object
  Time time;

  //maybe the nixie tubes have to be put into their own class, depending on how many digital pins are needed for each nixie tube (maybe have to use shift registers and put tubes together on one IC - this approach won't work in that case)
  //tubes stored from highest value digit to lowest (hh:mm:ss)
  NixiePair tubes[NR_TUBE_PAIRS];
};

#endif //CLOCK_H
