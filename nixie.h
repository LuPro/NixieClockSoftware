#ifndef NIXIE_H
#define NIXIE_H

class Nixie {
public:
  void setNumber(const unsigned char &value);

private:
  unsigned char value;
  
};

#endif //NIXIE_H
