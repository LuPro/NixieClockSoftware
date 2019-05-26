#include "animations.h"

unsigned char Animations::clearAnimations() {
  return 0b11000000;
}

unsigned char Animations::dotPairToggle(const unsigned char &pos) {
  return currentPattern = (~currentPattern & (0b11 << (2 * pos)) ) | (currentPattern & 0b11000000);
}
