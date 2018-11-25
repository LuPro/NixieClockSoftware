#include "clock.h"

void Clock::showTime() {
  
  unsigned char digits[NR_TUBES] = {time.h / 10, time.h % 10, time.m / 10, time.s % 10, time.s / 10, time.s % 10};

  for (unsigned char i = 0; i < NR_TUBES; i++) {
    tubes[i].setNumber(digity[i]);
  }
}

void Clock::loadTime() {
  //load time from the RTC unit and store it in this->time;
}
