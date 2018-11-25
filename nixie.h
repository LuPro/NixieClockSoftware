#ifndef NIXIE_H
#define NIXIE_H

class Nixie {
public:
  void setNumber(const unsigned char &value);

private:
  unsigned char value;

  //add pins to talk to this nixie tubes here
  
};

#endif //NIXIE_H
