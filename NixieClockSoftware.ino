#include "clock.h"

Clock clock;

void setup () {
  clock.init();
  //init buttons
}

void loop () {
  clock.updateTime();
  //check for buttons
}
