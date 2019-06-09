#include "clock.h"

void Clock::init() {
  nixies.init(ShiftRegister(NIXIES_DATA, NIXIES_CLOCK, NIXIES_LATCH), ShiftRegister(SPECIALS_DATA, SPECIALS_CLOCK, SPECIALS_LATCH));

  while (!rtc.begin()); //initiates the real time clock module and halts until finished initializing
  if (rtc.lostPower()) {
    Serial.println("RTC Lost Power");
    nixies.setMenuState(rtcError);
  } else {
    //this is only for testing purposes
    //nixies.setMenuState(rtcError);
  }
  nixies.startupAnimation();
  
  loadTime();
}

void Clock::updateTime() {
  loadTime();

  nixies.showTime(time);
  autoOnOff();
}

void Clock::changeTime(const char &unit, const short &delta, const bool &relative) {
  if (relative) {
    switch (unit) {
      case 'h':
        time = time + TimeSpan(0, delta, 0, 0);
        break;
      case 'm':
        time = time + TimeSpan(0, 0, delta, 0);
        break;
      case 's':
        time = time + TimeSpan(0, 0, 0, delta);
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

void Clock::autoOnOff() {
  if (automaticMode) {
    if (time.hour() == AUTO_ON_TIME_H && time.minute() == AUTO_ON_TIME_M && time.second() == 0) {
      nixies.setNixiesOnOff(true);
      Serial.println("turned nixies automatically on");
    } else if (time.hour() == AUTO_OFF_TIME_H && time.minute() == AUTO_OFF_TIME_M && time.second() == 0) {
      nixies.setNixiesOnOff(false);
      Serial.println("turned nixies automatically off");
    }
  }
}
