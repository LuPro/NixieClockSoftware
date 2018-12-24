#include "clock.h"

void Clock::init() {
  nixies.init(1234, 1337, 42, 666, 420);  //Todo: enter actual pins here

  while (!rtc.begin()); //initiates the real time clock module and halts until finished initializing
  //Todo: rest of the init function (initialize Todo: finish writing todos in the future
  //loadTime();
}

void Clock::updateTime() {
  loadTime()

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
  //rtc.setTime(time);
}

void Clock::loadTime() {
  //load time from the RTC unit and store it in this->time;
}

