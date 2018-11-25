#include "clock.h"

void Clock::showTime() {
  unsigned char allDigits[MAX_DIGITS] = {time.getH() / 10, time.getH() % 10, time.getM() / 10, time.getM() % 10, time.getS() / 10, time.getS() % 10};
  unsigned char digits[NR_TUBES];

  //shove all data from allDigits to digits, if less digits are actually implemented (eg: 4) only those will be actually stored to be displayed.
  //If clock only has 4 digits (hh:mm) only those will be used for future use, if it has 6 (hh:mm:ss) all will be used.
  for (unsigned char i = 0; i < NR_TUBES; i++) {
    digits[i] = allDigits[i];
  }
  
  //Apply time to the nixie tubes. Tubes themselves will check if there are any changes to a certain digit and if not will not change their output.
  for (unsigned char i = 0; i < NR_TUBES; i++) {
    tubes[i].setNumber(digits[i]);
  }
}

void Clock::changeTime(const char &unit, const short &delta, const bool &relative) {
  switch (unit) {
    case 'h':
      time.setH(delta, relative);
      break;
    case 'm':
      time.setM(delta, relative);
      break;
    case 's':
      time.setS(delta, relative);
      break;
  }
}

void Clock::loadTime() {
  //load time from the RTC unit and store it in this->time;
}
