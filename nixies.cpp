#include "nixies.h"

void Nixies::init(ShiftRegister nixieRegister, ShiftRegister specialsRegister) {
  this->nixieRegister = &nixieRegister;
  this->specialsRegister = &specialsRegister;

  this->nixieRegister->clear();
  this->specialsRegister->clear();

  pinMode(nixiePowerPin, OUTPUT);
  digitalWrite(nixiePowerPin, HIGH);
}

void Nixies::showTime(const DateTime &time) {
  unsigned long newTimeString = antiPoison(time);

  //checks if the new time string is identical with the one currently displayed on the nixies.
  //if they are the same the rest gets skipped (no need to send to registers/nixies when the time is already correct)
  if (newTimeString != currentTimeString) {
    //Serial.print("RTC Time: "); Serial.print(time.hour()); Serial.print(":"); Serial.print(time.minute()); Serial.print(":"); Serial.println(time.second());
    unsigned long timeStringCopy = newTimeString;
    for (char i = 0; i < NR_TUBES; i++) {
      Serial.print(timeStringCopy & 0b1111); Serial.print(" ");
      timeStringCopy = timeStringCopy >> 4;
    }
    Serial.println();
    //Serial.println(newTimeString); Serial.println();
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
  static bool animationsDirty = true;
  unsigned long now = millis();
  unsigned short animationPattern = 0;

  switch (menuState) {
    case noMenu:
      //add code to prevent this to be executed all the time (just once is enough)
      if (animationsDirty) {
        animationPattern = animator.clearAnimations();
        specialsRegister->sendData(animationPattern);
        specialsRegister->applyBuffer();
        animationsDirty = false;
        animator.errorLoop = 0;
      }
      break;
    case rtcError:
      animationsDirty = true;
      if (calcTimeDelta(lastUpdate, now) > ERROR_BLINK_DELAY) { //todo: a way for the error to stop (maybe just pressing buttons to switch menu would work)
        animationPattern = animator.errorAnimation();
        if (animationPattern & 0b1000000000000000) {
          menuState = noMenu;
          animationPattern = animator.clearAnimations();
        }
        specialsRegister->sendData(animationPattern);
        specialsRegister->applyBuffer();
        lastUpdate = now;
      }
      break;
    case startup:
      animationsDirty = true;
      if (calcTimeDelta(lastUpdate, now) > STARTUP_ANIM_DELAY) {
        animationPattern = animator.startupAnimation();
        if (animationPattern & 0b1000000000000000) {
          menuState = stateBackup;
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

unsigned long Nixies::generateTimeString(const unsigned char &hours, const unsigned char &minutes, const unsigned char &seconds) {
  unsigned long outputString = 0;

  outputString |= getLow(seconds);
  outputString = outputString << 4;
  outputString |= getHigh(seconds);
  outputString = outputString << 4;

  outputString |= getLow(minutes);
  outputString = outputString << 4;
  outputString |= getHigh(minutes);
  outputString = outputString << 4;

  outputString |= getLow(hours);
  outputString = outputString << 4;
  outputString |= getHigh(hours);

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

unsigned long Nixies::antiPoison(DateTime time) {
  static unsigned long lastUpdate = millis();
  static DateTime nextTrigger(2019, 1, 1, 12, ( ((time.minute() + 2) / 10) + 1 ) * 10, 0);
  static bool antiPoisonActive = false;
  static char n = 0;
  static char numbers[NR_TUBES];  //compress so 2 numbers fit in one later
  
  if (time.minute() == nextTrigger.minute() && time.second() == nextTrigger.second() && antiPoisonActive == false) { //first find out if anti poison has to be done
    randomSeed(millis());
    nextTrigger = nextTrigger + TimeSpan(0, 0, random(10 - INTERVALL_VARIANCE, 10 + INTERVALL_VARIANCE), random(60));
    antiPoisonActive = true;
  }

  if (antiPoisonActive && calcTimeDelta(lastUpdate, millis()) > POISON_CYCLE_DELAY) {
    lastUpdate = millis();
    
    for (unsigned char i = 0; i < NR_TUBES; i++) {  //generate random numbers
      if (n < MIN_NR_CYCLES) {  //generate random numbers from 0-9 
        numbers[i] = random(10);
      } else {
        if (numbers[i] != 10) { //only generate a new random number if previous wasn't 10, includes 10 in rng (which means end)
          if (random(101) > CHANCE_NR_LOCK) {
            numbers[i] = random(10);
          } else {
            numbers[i] = 10;
          }
        }
      }
    }
    n++;
  }

  if (antiPoisonActive) {
    char tempNumbers[NR_TUBES];
    for (unsigned char m = 0; m < NR_TUBES; m++) {
      tempNumbers[m] = numbers[m];
    }
    bool cont = false;
    if (tempNumbers[0] == 10) {
      tempNumbers[0] = time.hour() / 10;
    } else {
      cont = true;
    }
    if (tempNumbers[1] == 10) {
      tempNumbers[1] = time.hour() % 10;
    } else {
      cont = true;
    }
    if (tempNumbers[2] == 10) {
      tempNumbers[2] = time.minute() / 10;
    } else {
      cont = true;
    }
    if (tempNumbers[3] == 10) {
      tempNumbers[3] = time.minute() % 10;
    } else {
      cont = true;
    }
    if (tempNumbers[4] == 10) {
      tempNumbers[4] = time.second() / 10;
    } else {
      cont = true;
    }
    if (tempNumbers[5] == 10) {
      tempNumbers[5] = time.second() % 10;
    } else {
      cont = true;
    }

    if (!cont) {
      antiPoisonActive = false;
      n = 0;
    }
    
    return generateTimeString( (tempNumbers[0] * 10) + tempNumbers[1], (tempNumbers[2] * 10) + tempNumbers[3], (tempNumbers[4] * 10) + tempNumbers[5] );
  } else {
    return generateTimeString(time);
  }

}

unsigned long Nixies::calcTimeDelta(const unsigned long &timeOld, const unsigned long &timeNow) {
  if (timeNow < timeOld) {
    //Time variable has overflowed since last check
    return ( ((unsigned long) - 1) - timeOld ) + timeNow;
  } else {
    return timeNow - timeOld;
  }
}
