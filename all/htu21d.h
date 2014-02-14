#ifndef _HTU21D_H_
#define _HTU21D_H_

class HTU21D {
 public:
  void initialise();
  void read(float& humidity, float& temp, float& dewpoint);
};

#ifdef _MAIN_
HTU21D htu21d;
#else
extern HTU21D htu21d;
#endif

#endif
