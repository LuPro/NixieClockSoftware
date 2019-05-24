#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include "Arduino.h"
#include "math.h"

#define DEBUG_OUTPUT 1

#define NR_BUTTONS 5
#define RESET_BUTTONS_BITS 0b00110000

#define MARGIN_STATE_CHANGE 300
#define BUTTON_STATE_LOCK 1

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
  
  bool getButtonState(const char &button);

  void pollButtons();

private:
  void calcMeanValue(unsigned short buttonValue, const char &button);
  unsigned short meanValue[NR_BUTTONS];   //multiplied by 16
  void baseLowValue(unsigned short buttonValue, const char &button);
  unsigned short lowValue[NR_BUTTONS];    //multiplied by 64
  
  void baseHighValue(unsigned short buttonValue, const char &button);
  unsigned short highValue[NR_BUTTONS];   //multiplied by 64

  void determineButtonState(unsigned short buttonValue, const char &button);
  
  bool buttonStates[NR_BUTTONS];
  char button2pinMap[NR_BUTTONS] = {An0,An1,An2,An3,An6}; //Time UP, Time DOWN, Time Choose, On/Off

  //quick and dirty:
  unsigned short buttonPressed = 0;

  //bool buttonsDirty = false;  //store actual button value or store changes in button values?
};

#endif //TOUCHBUTTON_H
