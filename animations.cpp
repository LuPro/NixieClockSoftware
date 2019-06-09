#include "animations.h"

uint16_t Animations::clearAnimations() {
  currentPattern = 0b0011000000000000;
  return currentPattern;
}

uint16_t Animations::dotPairToggle(const unsigned char &pos) {
  return currentPattern = (~currentPattern & (0b1001 << (4 * pos)) ) | (currentPattern & 0b0011000000000000);
}
uint16_t Animations::errorAnimation() {
  if (errorLoop < ERROR_DURATION) {  
    errorLoop++;
    return currentPattern = (~currentPattern & 0b0000111111111111) | (currentPattern & 0b0011000000000000);
  } else {
    return 0b1011000000000000;
  }
}

uint16_t Animations::startupAnimation() {
  static int8_t loopCounter = -1;
  static bool up = true;

  if (loopCounter == -1) {
    currentPattern = 1;
    loopCounter = 0;
    return currentPattern;
  }

  if (loopCounter < STARTUP_ANIM_CYCLES) {  //left right shifting of light
    if (up) {
      currentPattern = currentPattern << 1;
      if (currentPattern & 0b0000100000000000) {
        up = false;
        loopCounter++;
      }
    } else {
      currentPattern = currentPattern >> 1;
      if (currentPattern & 1) {
        up = true;
      }
    }
  } else {  //blinking of colons
    if (loopCounter == STARTUP_ANIM_CYCLES) {
      currentPattern = 0b0011000000000000;
    } else if (loopCounter == STARTUP_ANIM_CYCLES + (2 * STARTUP_NR_BLINKS) + 1) {
      return 0b1011000000000000;  //MSB indicates that animation is finished and Nixies class should turn off startup mode (dirty and ew, maybe fix in the future)
    }
    loopCounter++;
    return currentPattern = (~currentPattern & 0b0011000000000000);
  }

  return currentPattern;
}
