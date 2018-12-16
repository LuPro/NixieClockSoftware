#ifndef TIME_H
#define TIME_H

class Time {
public:
  inline unsigned char getH() const {
    return h;
  }
  inline unsigned char getM() const {
    return m;
  }
  inline unsigned char getS() const {
    return s;
  }

  inline unsigned char getLow(unsigned char value) const {
    return value % 10;
  }
  inline unsigned char getHigh(unsigned char value) const {
    return value / 10;
  }

  inline void setH(short value, const bool &relative = false) {
    if (relative) {
      value %= 24;
      if (h + value < 0)
        24 - (h + value);
        
      h = (h + value) % 24;
    } else {
      h = value; //if erronous data gets entered maybe %24 to keep it valid? or %99 to not throw errors with nixie display
    }
  }
  inline void setM(short value, const bool &relative = false) {
    if (relative) {
      value %= 60;
      if (m + value < 0)
        60 - (m + value);
        
      m = (m + value) % 60;
    } else {
      m = value; //if erronous data gets entered maybe %60 to keep it valid? or %99 to not throw errors with nixie display
    }
  }
  inline void setS(short value, const bool &relative = false) {
    if (relative) {
      value %= 60;
      if (s + value < 0)
        60 - (s + value);
        
      s = (s + value) % 60;
    } else {
      s = value; //if erronous data gets entered maybe %60 to keep it valid? or %99 to not throw errors with nixie display
    }
  }

private:
  unsigned char h, m, s;
};

#endif //TIME_H
