#ifndef ANIMATIONS_H
#define ANIMATIONS_H

//contains logic for making the special animations
//animation functions should return a bit pattern for the shift register (including the bit pattern for the colon)
class Animations {
public:
  unsigned char clearAnimations();

  //makes a dot pair blink, pos can be 0, 1, 2 for selecting seconds, minutes, hours respectively
  unsigned char dotPairToggle(const unsigned char &pos);

private:
  unsigned char currentPattern = 0;
};

#endif //ANIMATIONS_H
