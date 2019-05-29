#include "nixies.h"

void Nixies::init(ShiftRegister nixieRegister, ShiftRegister specialsRegister) {
  this->nixieRegister = &nixieRegister;
  this->specialsRegister = &specialsRegister;
  
  this->nixieRegister->clear();
  this->specialsRegister->clear();
}

void Nixies::showTime(const DateTime &time) {
  unsigned long newTimeString = generateTimeString(time);

  //checks if the new time string is identical with the one currently displayed on the nixies.
  //if they are the same the rest gets skipped (no need to send to registers/nixies when the time is already correct)
  if (newTimeString != currentTimeString) {
    Serial.print(time.hour()); Serial.print(":"); Serial.print(time.minute()); Serial.print(":"); Serial.println(time.second());
    sendTimeToRegister(newTimeString);
    currentTimeString = newTimeString;

    updateTimeSynchronousAnimations();
    
    nixieRegister->applyBuffer();
  }  
  
  updateAnimations();
}

void Nixies::updateTimeSynchronousAnimations() {
  unsigned short animationPattern = 0;
  
  switch (menuState) {
    case noMenu:
      animationPattern = animator.clearAnimations();
      specialsRegister->applyBuffer();
      break;
    case setSeconds:
      animationPattern = animator.dotPairToggle(2);
      specialsRegister->applyBuffer();
      break;
    case setMinutes:
      animationPattern = animator.dotPairToggle(1);
      specialsRegister->applyBuffer();
      break;
    case setHours:
      animationPattern = animator.dotPairToggle(0);
      specialsRegister->applyBuffer();
      break;
  }
  
  specialsRegister->sendData(animationPattern);
}

void Nixies::updateAnimations() {
  static unsigned long lastUpdate = millis();
  unsigned long now = millis();
  unsigned short animationPattern = 0;
  
  switch (menuState) {
    case noMenu:
      //add code to prevent this to be executed all the time (just once is enough)
      /*animationPattern = animator.clearAnimations();
      specialsRegister->sendData(animationPattern);
      specialsRegister->applyBuffer();*/
      break;
    case rtcError:
      if (calcTimeDelta(lastUpdate, now) > ERROR_BLINK_DELAY) {
        animationPattern = animator.errorAnimation();
        specialsRegister->sendData(animationPattern);
        specialsRegister->applyBuffer();
        lastUpdate = now;
      }
      break;
    case startup:
      if (calcTimeDelta(lastUpdate, now) > STARTUP_ANIM_DELAY) {
        animationPattern = animator.startupAnimation();
        if (animationPattern &0b1000000000000000) {
          menuState = noMenu;
          animationPattern = animator.clearAnimations();
        }
        
        specialsRegister->sendData(animationPattern);
        specialsRegister->applyBuffer();
        lastUpdate = now;
      }
      break;
  }
}

unsigned long Nixies::generateTimeString(const DateTime &time) {
  unsigned long outputString = 0;

  outputString |= getLow(time.second());
  outputString = outputString << 4;
  outputString |= getHigh(time.second());
  outputString = outputString << 4;
  
  outputString |= getLow(time.minute());
  outputString = outputString << 4;
  outputString |= getHigh(time.minute());
  outputString = outputString << 4;
  
  outputString |= getLow(time.hour());
  outputString = outputString << 4;
  outputString |= getHigh(time.hour());

  return outputString;
}

void Nixies::sendTimeToRegister(unsigned long timeString) {
  unsigned char digitString = 0;

  for (unsigned char i = 0; i < (NR_TUBES * 4); i++) {
    //checks if there needs to be a new digit loaded into digitString (always when the old digit got sent and a new nixie needs data = always after 4 shifted bits
    if (i % 4 == 0) {
      digitString = 0;
      //this loads one digit (4 bits) into a separate variable each bit individually
      //ATTENTION: this reverses the bit pattern of that digit. This is needed because when sending the number out to the shift registers it will be reversed again
      for (unsigned char n = 0; n < 4; n++) {
        digitString = digitString << 1;
        digitString |= (timeString & 1);
        timeString = timeString >> 1;
      }
    }
    //this sends the data strings to the shift registers, each bit individually. By doing this the individual digits will have their bit pattern reversed!
    //counteracted by loading the individual digits pre-reversed so that this reversing reverses the reverse and results in a non-reversed number (as seen above)
    nixieRegister->sendBit(digitString & 1);
    digitString = digitString >> 1;
  }
}

unsigned long Nixies::calcTimeDelta(const unsigned long &timeOld, const unsigned long &timeNow) {
  if (timeNow < timeOld) {
    //Time variable has overflowed since last check
    return ( ((unsigned long)-1) - timeOld ) + timeNow;
  } else {
    return timeNow - timeOld;
  }
}
