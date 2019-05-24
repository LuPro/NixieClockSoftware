#include "clock.h"
#include "touchButton.h"

Clock clock;
TouchButtonArray buttons;

void setup () {
  Serial.begin(9600);
  clock.init();
  
  //init buttons
  //pinMode(13,OUTPUT);
}

void loop () {
  clock.updateTime();
  //buttons.pollButtons();
}
