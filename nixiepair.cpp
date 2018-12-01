#include "nixiepair.h"

void NixiePair::setHigh(const unsigned char &valueHigh) {
  if (this->valueHigh == valueHigh) {
    return;
  } else {
    this->valueHigh = valueHigh;

    //ToDo: update actual display of that tube
  }
}

void NixiePair::setLow(const unsigned char &valueLow) {
  if (this->valueLow == valueLow) {
    return;
  } else {
    this->valueLow = valueLow;

    //ToDo: update actual display of that tube
  }
}

void NixiePair::applyNumbers() {
  unsigned char output;

  output = (valueHigh << 4) | valueLow;

  //ToDo: output to shift register
}

