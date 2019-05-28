#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "stdint.h"

//contains logic for making the special animations
//animation functions should return a bit pattern for the shift register (including the bit pattern for the colon)
class Animations {
public:
  uint16_t clearAnimations();

  //makes a dot pair blink, pos can be 0, 1, 2 for selecting seconds, minutes, hours respectively
  uint16_t dotPairToggle(const unsigned char &pos);

private:
  uint16_t currentPattern = 0;
};

#endif //ANIMATIONS_H