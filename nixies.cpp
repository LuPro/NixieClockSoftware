#include "nixies.h"

void Nixies::init(const unsigned char &shiftDataPin, const unsigned char &shiftClockPin, const unsigned char &shiftLatchPin) {
  this->shiftDataPin = shiftDataPin;
  this->shiftClockPin = shiftClockPin;
  this->shiftLatchPin = shiftLatchPin;

  pinMode(shiftDataPin, OUTPUT);
  digitalWrite(shiftDataPin, LOW);
  pinMode(shiftClockPin, OUTPUT);
  digitalWrite(shiftClockPin, LOW);
  pinMode(shiftLatchPin, OUTPUT);
  digitalWrite(shiftLatchPin, LOW);
}

void Nixies::showTime(const DateTime &time) {
  unsigned long newTimeString = generateTimeString(time);

  //checks if the new time string is identical with the one currently displayed on the nixies.
  //if they are the same the rest gets skipped (no need to send to registers/nixies when the time is already correct)
  if (newTimeString != currentTimeString) {
    Serial.print(time.hour()); Serial.print(":"); Serial.print(time.minute()); Serial.print(":"); Serial.println(time.second());
    sendTimeToRegister(newTimeString);
    applyRegisterBuffer(); 
    currentTimeString = newTimeString;
  }  
}

unsigned long Nixies::generateTimeString(const DateTime &time) {
  unsigned long outputString = 0;

  outputString |= getLow(time.second());
  outputString = outputString << 4;
  outputString |= getHigh(time.second());
  outputString = outputString << 4;
  
  outputString |= getLow(time.minute());
  outputString = outputString << 4;
  outputString |= getHigh(time.minute());
  outputString = outputString << 4;
  
  outputString |= getLow(time.hour());
  outputString = outputString << 4;
  outputString |= getHigh(time.hour());

  return outputString;
}

void Nixies::sendTimeToRegister(unsigned long timeString) {
  unsigned char digitString = 0;
  
  for (unsigned char i = 0; i < (NR_TUBES * 4); i++) {
    //checks if there needs to be a new digit loaded into digitString (always when the old digit got sent and a new nixie needs data = always after 4 shifted bits
    if (i % 4 == 0) {
      digitString = 0;
      //this loads one digit (4 bits) into a separate variable each bit individually
      //ATTENTION: this reverses the bit pattern of that digit. This is needed because when sending the number out to the shift registers it will be reversed again
      for (unsigned char n = 0; n < 4; n++) {
        digitString = digitString << 1;
        digitString |= (timeString & 1);
        timeString = timeString >> 1;
      }
    }
    //this sends the data strings to the shift registers, each bit individually. By doing this the individual digits will have their bit pattern reversed!
    //counteracted by loading the individual digits pre-reversed so that this reversing reverses the reverse and results in a non-reversed number (as seen above)
    digitalWrite(shiftDataPin, (digitString & 1));
    digitalWrite(shiftClockPin, HIGH);
    digitString = digitString >> 1;
    delayMicroseconds(1);
    digitalWrite(shiftClockPin, LOW);
    digitalWrite(shiftDataPin, LOW);
  }
}
