#ifndef NIXIE_H
#define NIXIE_H

class NixiePair {
public:
  void setHigh(const unsigned char &valueHigh);

  void setLow(const unsigned char &valueLow);

private:
  void applyNumbers();

  unsigned char valueHigh, valueLow;

  //ToDo: pins for accessing the shift register
  //unsigned char 

  //add pins to talk to this nixie tubes here
  
};

#endif //NIXIE_H
