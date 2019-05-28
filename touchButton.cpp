#include "touchButton.h"

TouchButtonArray::TouchButtonArray () {
  MCUCR = 0b11101111;         //enable pull ups (set pull up disable to 0)
  DDRC &= RESET_BUTTONS_BITS; //set all possible touch pins to input (0), I2C pins left unchanged (1)
  PORTC &= RESET_BUTTONS_BITS;  //reset PORTC to 0 -> disable all pull-ups (I2C pins left unchanged)
  ADCSRA = 0b10000000;        //active ADC, reset everything else
}

bool TouchButtonArray::getState (const char &button) {
  if (button >= 0 && button < NR_BUTTONS) {
    return buttonStates[button];
  }
}

bool* TouchButtonArray::getStates() {
  return buttonStates;
}

void TouchButtonArray::pollButtons () {
  //check every button
  for (unsigned short i = 0; i < NR_BUTTONS; i++) {
    PORTC |= (1 << button2pinMap[i]); //enable pull up for button that will be measured
    delayMicroseconds(2);

    PORTC &= RESET_BUTTONS_BITS;  //reset PORTC to 0 -> disable all pull-ups (I2C pins left unchanged)

    unsigned int buttonValue = analogRead(14 + i);
    //ADMUX &= RESET_BUTTONS_BITS;  //reset analog channel selection
    //ADMUX |= button2pinMap[0]; //set analog channel to button
    //Serial.print("Selecting Channel for ADC conversion: "); Serial.println((int)button2pinMap[i]);

    //PRR = 0;  //power reduction register to 0 - no power reduction, all modules are online
    //ADCSRA |= 0b01000000; //start conversion
    //while ((ADCSRA & 0b01000000) == true);  //block until conversion is done

    /*unsigned int */buttonValue = ADCL | (ADCH << 8);

    //discharge capacitor after conversion
    pinMode(14 + i, OUTPUT);
    digitalWrite(14 + i, LOW);
    delayMicroseconds(5);
    pinMode(14 + i, INPUT);

    calcMeanValue(buttonValue, i);
    determineButtonState(buttonValue, i);

#if DEBUG_OUTPUT != 0
    if (i == 0) {
      digitalWrite(13, buttonStates[0]);
      Serial.print(buttonValue << 6);
      Serial.print(",");
      Serial.print(meanValue[i] << 2);
      Serial.print(",");
      Serial.print(lowValue[i]);
      Serial.print(",");
      Serial.println(highValue[i]);
    }
#endif
  }
}

void TouchButtonArray::calcMeanValue(unsigned short buttonValue, const char &button) {
  meanValue[button] = meanValue[button] - (meanValue[button] >> 4) + buttonValue;
}

void TouchButtonArray::baseLowValue(unsigned short buttonValue, const char &button) {
  if (!buttonStates[button]) {
    lowValue[button] = lowValue[button] - (lowValue[button] >> 6) + buttonValue;
  }
}

void TouchButtonArray::baseHighValue(unsigned short buttonValue, const char &button) {
  if (buttonStates[button]) {
    highValue[button] = highValue[button] - (highValue[button] >> 6) + buttonValue;
  }
}

void TouchButtonArray::determineButtonState (unsigned short buttonValue, const char &button) {
  static unsigned short i = 0, n[NR_BUTTONS], m[NR_BUTTONS];

  baseLowValue(buttonValue, button);
  baseHighValue(buttonValue, button);

  if (i == 0) {
    for (char x = 0; x < NR_BUTTONS; x++) {
      n[x] = 0;
      m[x] = 0;
    }
  }

  if (i > 100 * NR_BUTTONS) {  //first 100 runs after start up ignore button value and update base low value anyways (to "load" the system, let averages reach meaningful values)
    if (buttonStates[button]) {
      if (n[button] > BUTTON_STATE_LOCK) {
        if (meanValue[button] * 4 < highValue[button] - MARGIN_STATE_CHANGE) {
          buttonStates[button] = false;
          n[button] = 0;
        } else {
          buttonStates[button] = true;
        }
      } else {
        n[button]++;
        buttonStates[button] = true;
        highValue[button] = meanValue[button] << 2;
      }
    } else {
      if (m[button] > BUTTON_STATE_LOCK) {
        if (meanValue[button] * 4 > lowValue[button] + MARGIN_STATE_CHANGE) {
          buttonStates[button] = true;
          m[button] = 0;
        } else {
          buttonStates[button] = false;
        }
      } else {
        m[button]++;
        buttonStates[button] = false;
        lowValue[button] = meanValue[button] << 2;
      }
    }
  } else {
    i++;
    buttonStates[button] = false;
    lowValue[button] = meanValue[button] << 2;
  }
}
