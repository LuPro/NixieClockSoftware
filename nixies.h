#ifndef NIXIES_H
#define NIXIES_H

#include "RTClib.h"
#include "Arduino.h"
#include "shiftRegister.h"
#include "animations.h"

#define NR_TUBES 6
#define MAX_TUBES 6

//make this as variable loaded from eeprom later on (so it can be changed via bluetooth and values can be stored)
#define ERROR_BLINK_DELAY 300    //in ms
#define STARTUP_ANIM_DELAY 80    //in ms


//variables for anti poison number cycling - move to eeprom later on
#define POISON_CYCLE_DELAY 200   //in ms
#define MIN_NR_CYCLES 15
#define CHANCE_NR_LOCK 20        //in %
#define INTERVALL_VARIANCE 5     //in min

enum MenuStates {
  noMenu,
  setSeconds,
  setMinutes,
  setHours,
  rtcError,
  bluetoothConnect,
  startup
};

class Nixies {
public:
  //sets the pins needed for the shift register chain and initializes the pins as needed, all of those pins just need digital out capabilities (Todo: shiftOutEn and shiftClear might not be needed, check for this later on)
  void init(ShiftRegister nixieRegister, ShiftRegister specialsRegister);

  void showTime(const DateTime &time);

  inline void setMenuState(MenuStates state) {
    menuState = state;
  }

  inline MenuStates getMenuState () {
    return menuState;
  }

  inline void startupAnimation() {
    stateBackup = menuState;
    menuState = startup;
  }

  inline void toggleNixiesOnOff() {
    digitalWrite(nixiePowerPin, !digitalRead(nixiePowerPin));
  }
  inline void setNixiesOnOff(bool on) {
    digitalWrite(nixiePowerPin, on);
  }

private:
  //generates a binary string of time data to be sent to the chained shift registers
  //format is ss:mm:hh, hours at lowest values. If change to just 4 tubes code can still be used and seconds just shift into nothingness (seconds get lost)
  //each unit (digit pair) gets 8 bit, resulting string is 24 bit long
  unsigned long generateTimeString(const DateTime &time);
  
  unsigned long generateTimeString(const unsigned char &hours, const unsigned char &minutes, const unsigned char &seconds);

  //takes the generated time string from generateTimeString (in a ss:mm:hh format) and outputs it into the shift registers
  //it starts with lowest bits
  void sendTimeToRegister(unsigned long timeString);

  //update animations that should be synchronous with time change (frame update 1x per second at best, changes exactly when seconds change)
  void updateTimeSynchronousAnimations();
  
  //update animations of other kinds (like error messages or bluetooth info)
  void updateAnimations();

  //function to cycle through all nixies to work against cathode poisoning
  unsigned long antiPoison(DateTime time);

  unsigned long calcTimeDelta(const unsigned long &timeOld, const unsigned long &timeNow);

  //Pin for turning the high powered output (nixie power) from the power supply off
  unsigned char nixiePowerPin = 12;

  ShiftRegister *nixieRegister;
  ShiftRegister *specialsRegister;
  
  Animations animator;
  
  MenuStates menuState = noMenu;
  MenuStates stateBackup = noMenu;

  //stores the time string of the current displayed numbers on the nixies, used to cross check if there are changes in new time
  unsigned long currentTimeString; 

  inline unsigned char getLow(unsigned char value) const {
    return value % 10;
  }
  inline unsigned char getHigh(unsigned char value) const {
    return value / 10;
  }
};

#endif //NIXIES_H
