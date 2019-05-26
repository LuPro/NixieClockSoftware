#include "clock.h"

void Clock::init() {
  nixies.init(ShiftRegister(NIXIES_DATA, NIXIES_CLOCK, NIXIES_LATCH), ShiftRegister(SPECIALS_DATA, SPECIALS_CLOCK, SPECIALS_LATCH));

  while (!rtc.begin()); //initiates the real time clock module and halts until finished initializing
  if (rtc.lostPower()) {
    Serial.println("RTC Lost Power");
    //set stuff for lost power info (maybe serial print, maybe some flag to make animations or shit dunno)
  }
  
  loadTime();
}

void Clock::updateTime() {
  loadTime();

  nixies.showTime(time);
}

void Clock::changeTime(const char &unit, const short &delta, const bool &relative) {
  if (relative) {
    switch (unit) {
      case 'h':
        time + TimeSpan(0, delta, 0, 0);
        break;
      case 'm':
        time + TimeSpan(0, 0, delta, 0);
        break;
      case 's':
        time + TimeSpan(0, 0, 0, delta);
        break;
    }
  } else {
    time = rtc.now();
    switch (unit) {
      case 'h':
        time = DateTime(time.year(), time.month(), time.day(), delta, time.minute(), time.second());
        break;
      case 'm':
        time = DateTime(time.year(), time.month(), time.day(), time.hour(), delta, time.second());
        break;
      case 's':
        time = DateTime(time.year(), time.month(), time.day(), time.hour(), time.minute(), delta);
        break;
    }
  }

  nixies.showTime(time);
  setTime(time);
}

void Clock::loadTime() {
  time = rtc.now();
}

void Clock::setTime(const DateTime &time) {
  this->time = time;
  rtc.adjust(time);
}
