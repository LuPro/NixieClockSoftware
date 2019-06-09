#include "clock.h"
#include "touchButton.h"
#include "animations.h"

#include "stdint.h"

Clock clock;
Animations animator;
TouchButtonArray buttons;

enum ButtonMapping {
  timeUp,
  timeDown,
  unitSelect,
  nixiesOnOff
};

void setup () {
  Serial.begin(9600);
  clock.init();
  clock.setMenuState(startup);

  //init buttons
  pinMode(13,OUTPUT);
}

void loop () {
  clock.updateTime();
  //buttons.pollButtons();
  buttons.pollTestButtons();

  bool *buttonStates, *changedStates;
  unsigned short *timesLastActive;
  buttonStates = buttons.getStates();
  changedStates = buttons.getChangedStates();
  timesLastActive = buttons.getTimesLastActive();

  if (buttonStates[timeUp] && changedStates[timeUp]) {
    Serial.println("time up");
    switch (clock.getMenuState()) {
      case setSeconds:
        clock.changeTime('s', 1, true);
        Serial.println("change seconds");
        break;
      case setMinutes:
        clock.changeTime('m', 1, true);
        Serial.println("change minutes");
        break;
      case setHours:
        clock.changeTime('h', 1, true);
        Serial.println("change hours");
        break;
    }
  }
  else if (buttonStates[timeDown] && changedStates[timeDown]) {
    Serial.println("time down");
    switch (clock.getMenuState()) {
      case setSeconds:
        clock.changeTime('s', -1, true);
        Serial.println("change seconds");
        break;
      case setMinutes:
        clock.changeTime('m', -1, true);
        Serial.println("change minutes");
        break;
      case setHours:
        clock.changeTime('h', -1, true);
        Serial.println("change hours");
        break;
    }
  }
  else if (buttonStates[unitSelect] && changedStates[unitSelect]) {
    Serial.println("unit select");
    if (clock.getMenuState() > MenuStates::setHours) {  //if menu state is something different than a time set state, set it to a time set state
      Serial.println("incorrect state for unit entry");
      clock.setMenuState(setSeconds);
    } else {                                            //if it is a time set state (or no menu), just cycle through
      Serial.print("cycle unit, old: "); Serial.print(clock.getMenuState());
      clock.setMenuState( (clock.getMenuState() + 1) % (MenuStates::setHours + 1) );
      Serial.print(" new: "); Serial.println(clock.getMenuState());
    }
  }
  else if (buttonStates[nixiesOnOff] && changedStates[nixiesOnOff]) {
    if (timesLastActive[nixiesOnOff] > 100) {
      Serial.println("double click: turn off automatic mode");
      clock.setAutomaticMode(false);
    } else {
      Serial.println("nixies on/off, activate automatic mode");
      clock.toggleNixiesOnOff();
      clock.setAutomaticMode(true);
    }
  }
  /*else if (buttonStates[nixiesOnOff] && changedStates[nixiesOnOff]) {
    Serial.println("nixies on/off");
    clock.setMenuState(toggleNixies);
  }*/
  //delay(500);
}
