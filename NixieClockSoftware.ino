#include "clock.h"
#include "touchButton.h"
#include "animations.h"

#include "stdint.h"

Clock clock;
Animations animator;
TouchButtonArray buttons;

enum ButtonMapping {
  
};

void setup () {
  Serial.begin(9600);
  clock.init();
  clock.setMenuState(1);
  
  //init buttons
  //pinMode(13,OUTPUT);
}

void loop () {
  clock.updateTime();
  //buttons.pollButtons();

  bool *buttonStates;
  buttonStates = buttons.getStates();

  for (unsigned char i = 0; i < NR_BUTTONS; i++) {
    buttonStates[i];
  }
}
