#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "stdint.h"
#include "Arduino.h"

#define STARTUP_ANIM_CYCLES 2
#define STARTUP_NR_BLINKS 2

#define ERROR_DURATION 50        //in cycles

//contains logic for making the special animations
//animation functions should return a bit pattern for the shift register (including the bit pattern for the colon)
class Animations {
public:
  uint16_t clearAnimations();

  //makes a dot pair blink, pos can be 0, 1, 2 for selecting seconds, minutes, hours respectively
  uint16_t dotPairToggle(const unsigned char &pos);

  uint16_t errorAnimation();
  unsigned char errorLoop = 0;

  uint16_t startupAnimation();

private:
  uint16_t currentPattern = 0;

};

#endif //ANIMATIONS_H
