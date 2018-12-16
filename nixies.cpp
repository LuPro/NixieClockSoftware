#include "nixies.h"

void Nixies::init(const unsigned char &shiftDataPin, const unsigned char &shiftClockPin, const unsigned char &shiftLatchPin, const unsigned char &shiftOutEnPin, const unsigned char &shiftClearPin) {
  this->shiftDataPin = shiftDataPin;
  this->shiftClockPin = shiftClockPin;
  this->shiftLatchPin = shiftLatchPin;
  this->shiftOutEnPin = shiftOutEnPin;
  this->shiftClearPin = shiftClearPin;

  //Todo: actually initialize the pins as needed (outputs, low state)
  //How? either with arduino built in libraries (easy but not terribly quick) or using arduino registers (harder, easier on processor but less flexible to processor changes)
}

void Nixies::showTime(const Time &time) {
  unsigned int newTimeString = generateTimeString(time);

  //checks if the new time string is identical with the one currently displayed on the nixies.
  //if they are the same the rest gets skipped (no need to send to registers/nixies when the time is already correct)
  if (newTimeString != currentTimeString) {
    sendTimeToRegister(newTimeString);
    applyRegisterBuffer(); 
    currentTimeString = newTimeString;
  }  
}

unsigned int Nixies::generateTimeString(const Time &time) {
  unsigned int outputString = 0;

  outputString |= time.getLow(time.getS());
  outputString << 4;
  outputString |= time.getHigh(time.getS());
  outputString << 4;
  
  outputString |= time.getLow(time.getM());
  outputString << 4;
  outputString |= time.getHigh(time.getM());
  outputString << 4;
  
  outputString |= time.getLow(time.getH());
  outputString << 4;
  outputString |= time.getHigh(time.getH());
  outputString << 4;

  return outputString;
}

void Nixies::sendTimeToRegister(unsigned int timeString) {
  unsigned char digitString = 0;
  
  for (unsigned char i = 0; i < (NR_TUBES * 4); i++) {
    //checks if there needs to be a new digit loaded into digitString (always when the old digit got sent and a new nixie needs data = always after 4 shifted bits
    if (i % 4 == 0) {
      digitString = 0;
      //this loads one digit (4 bits) into a separate variable each bit individually
      //ATTENTION: this reverses the bit pattern of that digit. This is needed because when sending the number out to the shift registers it will be reversed again
      for (unsigned char n = 0; n < 4; n++) {
        digitString |= (timeString & 1);
        timeString >> 1;
      }
    }
    //this sends the data strings to the shift registers, each bit individually. By doing this the individual digits will have their bit pattern reversed!
    //counteracted by loading the individual digits pre-reversed so that this reversing reverses the reverse and results in a non-reversed number (as seen above)
    digitalWrite(shiftDataPin, (timeString & 1));
    digitalWrite(shiftClockPin, HIGH);
    digitString >> 1;
    delayMicroseconds(1);
    digitalWrite(shiftClockPin, LOW);
    digitalWrite(shiftDataPin, LOW);
  }
}
