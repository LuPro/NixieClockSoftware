#include "nixie.h"

void Nixie::setNumber(const unsigned char &value) {
  if (this->value == value) {
    return;
  } else {
    this->value = value;

    //ToDo: update actual display of that tube
  }
}
