#ifndef _TSL2561_H_
#define _TSL2561_H_

class TSL2561 {
 private:
  float integ, gain;
 public:
  void initialise();
  void read(float& luxVisible);
};

#ifdef _MAIN_
TSL2561 tsl2561;
#else
extern TSL2561 tsl2561;
#endif

#endif
