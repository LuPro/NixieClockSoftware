#include "clock.h"

void Clock::updateTime() {
  //loadTime()

  nixies.showTime(time);
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

  nixies.showTime(time);
}

void Clock::loadTime() {
  //load time from the RTC unit and store it in this->time;
}
