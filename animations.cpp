#include "animations.h"

uint16_t Animations::clearAnimations() {
  return 0b0011000000000000;
}

uint16_t Animations::dotPairToggle(const unsigned char &pos) {
  return currentPattern = (~currentPattern & (0b1001 << (4 * pos)) ) | (currentPattern & 0b0011000000000000);
}
