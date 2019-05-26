#include "shiftRegister.h"

ShiftRegister::ShiftRegister(const unsigned char &dataPin, const unsigned char &clockPin, const unsigned char &latchPin) :
dataPin(dataPin), clockPin(clockPin), latchPin(latchPin)
{
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, LOW);
  pinMode(clockPin, OUTPUT);
  digitalWrite(clockPin, LOW);
  pinMode(latchPin, OUTPUT);
  digitalWrite(latchPin, LOW);
}

ShiftRegister::clear() {
  digitalWrite(dataPin, LOW);
  for (unsigned char i = 0; i < 8; i++) {
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(clockPin, LOW);
  }
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(latchPin, LOW);
}

void ShiftRegister::sendByte(unsigned char data, bool LSBfirst) {
  if (LSBfirst) {
    for (unsigned char i = 0; i < (sizeof(unsigned char) * 8); i++) {
      digitalWrite(dataPin, data & 1);
      digitalWrite(clockPin, HIGH);
      delayMicroseconds(1);
      digitalWrite(clockPin, LOW);
      digitalWrite(dataPin, LOW);
      data = data >> 1;
    }
  } else {
    for (unsigned char i = 0; i < (sizeof(unsigned char) * 8); i++ ) {
      digitalWrite(dataPin, data & 0b10000000);
      digitalWrite(clockPin, HIGH);
      delayMicroseconds(1);
      digitalWrite(clockPin, LOW);
      digitalWrite(dataPin, LOW);
      data = data << 1;
    }
  }
}
