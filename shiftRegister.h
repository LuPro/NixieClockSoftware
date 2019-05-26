#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include "Arduino.h"

class ShiftRegister {
public:
  ShiftRegister(const unsigned char &dataPin, const unsigned char &clockPin, const unsigned char &latchPin);

  clear();

  inline void sendBit(bool bit) {
    digitalWrite(dataPin, bit);
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, LOW);
  }
  void sendByte(unsigned char data, bool LSBfirst = false);

  inline void applyBuffer() {
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(latchPin, LOW);
  }
  
private:
  unsigned char dataPin = 0;
  unsigned char clockPin = 0;
  unsigned char latchPin = 0;
};

#endif //SHIFTREGISTER_H
