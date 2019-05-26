#include "clock.h"
#include "touchButton.h"
#include "animations.h"

Clock clock;
Animations animator;
TouchButtonArray buttons;

void setup () {
  Serial.begin(9600);
  clock.init();
  clock.setMenuState(3);
  
  //init buttons
  //pinMode(13,OUTPUT);
}

void loop () {
  clock.updateTime();
  //buttons.pollButtons();
}
