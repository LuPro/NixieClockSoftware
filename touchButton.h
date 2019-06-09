#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include "Arduino.h"
#include "math.h"

#define DEBUG_OUTPUT 0

#define NR_BUTTONS 4
#define RESET_BUTTONS_BITS 0b00110000

#define MARGIN_STATE_CHANGE 300
#define BUTTON_STATE_LOCK 1

#define BUTTON_DOUBLE_TIMEOUT 1500  //how long the buttons should wait to see for a second click (ms)

enum PinsToPortOffset {
  An0 = 0,
  An1 = 1,
  An2 = 2,
  An3 = 3,
  An4 = 4,
  An5 = 5,
  An6 = 6,
  An7 = 7
};

class TouchButtonArray {
public:
  TouchButtonArray();

  bool getState(const char &button);
  bool getChangedState(const char &button);
  unsigned short getTimeLastActive(const char &button);

  bool* getStates();
  bool* getChangedStates();
  unsigned short* getTimesLastActive();

  void pollButtons();

  void pollTestButtons();

private:
  void calcMeanValue(unsigned short buttonValue, const char &button);
  unsigned short meanValue[NR_BUTTONS];   //multiplied by 16
  
  void baseLowValue(unsigned short buttonValue, const char &button);
  unsigned short lowValue[NR_BUTTONS];    //multiplied by 64

  void baseHighValue(unsigned short buttonValue, const char &button);
  unsigned short highValue[NR_BUTTONS];   //multiplied by 64

  void determineButtonState(unsigned short buttonValue, const char &button);

  //this is a copy of the function in the nixies class currently, maybe put both occurences in a utility library?
  unsigned short calcTimeDelta(const unsigned long &timeOld, const unsigned long &timeNow);

  bool buttonStates[NR_BUTTONS];
  
  //todo: if bool is changed to unsigned long it could store the time since the last activation (probably with a cap so it can't overflow) which can be used for double press logic
  //eg: 0 = no change in recent time, 1-$SOME_RANDOM_MAXIMUM_NUMBER = ms since last activation (no sense in having more than ~10-20s, so reset at some point to 0 would make sense [also: can reduce variable since in that case]])
  bool changedStates[NR_BUTTONS]; //stores true if a button state has changed since last polling
  unsigned short timeLastActive[NR_BUTTONS];  //stores ms since last time button got activated. Only resets on RISING edges (from not pressed to pressed)
  char button2pinMap[NR_BUTTONS] = {An0,An1,An2,An3/*,An6*/}; //Time UP, Time DOWN, Time Choose, On/Off
};

#endif //TOUCHBUTTON_H
