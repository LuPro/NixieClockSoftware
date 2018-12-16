#ifndef NIXIES_H
#define NIXIES_H

#include "time.h"
#include "Arduino.h"

#define NR_TUBES 6
#define MAX_TUBES 6

//SUPER IMPORTANT: FIND OUT WHAT TIMERS THE BUILT-IN ARDUINO LIBRARIES USE AND CHECK IF THIS WON'T INTERFERE WITH THE TOUCH BUTTONS

class Nixies {
public:
  //sets the pins needed for the shift register chain and initializes the pins as needed, all of those pins just need digital out capabilities (Todo: shiftOutEn and shiftClear might not be needed, check for this later on)
  void init(const unsigned char &shiftDataPin, const unsigned char &shiftClockPin, const unsigned char &shiftLatchPin, const unsigned char &shiftOutEnPin, const unsigned char &shiftClearPin);

  void showTime(const Time &time);

private:
  //generates a binary string of time data to be sent to the chained shift registers
  //format is ss:mm:hh, hours at lowest values. If change to just 4 tubes code can still be used and seconds just shift into nothingness (seconds get lost)
  //each unit (digit pair) gets 8 bit, resulting string is 24 bit long
  unsigned int generateTimeString(const Time &time);

  //takes the generated time string from generateTimeString (in a ss:mm:hh format) and outputs it into the shift registers
  //it starts with lowest bits
  void sendTimeToRegister(unsigned int timeString);

  inline void applyRegisterBuffer() {
    digitalWrite(shiftLatchPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(shiftLatchPin, LOW);
  }

  //Pin for turning the high powered output (nixie power) from the power supply off (TODO: which pin?)
  unsigned char powerOffPin = 0;

  //Pins for shift registers
  unsigned char shiftDataPin = 0;
  unsigned char shiftClockPin = 0;
  unsigned char shiftLatchPin = 0;
  
  unsigned char shiftOutEnPin = 0;  //maybe not needed, can be substituted with just writing a string of 0s into shift buffer
  unsigned char shiftClearPin = 0;  //maybe not needed, can be substituted with the same as above (albeit it would be slower)

  //stores the time string of the current displayed numbers on the nixies, used to cross check if there are changes in new time
  unsigned int currentTimeString; 
};

#endif //NIXIES_H
