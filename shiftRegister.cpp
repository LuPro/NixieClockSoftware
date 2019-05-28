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

void ShiftRegister::sendData(uint16_t data, bool LSBfirst) {
  if (LSBfirst) {
    for (unsigned char i = 0; i < (sizeof(uint16_t) * 8); i++) {
      digitalWrite(dataPin, (data & 1));
      digitalWrite(clockPin, HIGH);
      delayMicroseconds(1);
      digitalWrite(clockPin, LOW);
      digitalWrite(dataPin, LOW);
      data = data >> 1;
    }
  } else {
    for (unsigned char i = 0; i < (sizeof(uint16_t) * 8); i++) {
      digitalWrite(dataPin, (data & 0b1000000000000000) ? HIGH : LOW);
      digitalWrite(clockPin, HIGH);
      delayMicroseconds(1);
      digitalWrite(clockPin, LOW);
      digitalWrite(dataPin, LOW);
      data = data << 1;
    }
  }
}
