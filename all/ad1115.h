#ifndef _AD1115_H_
#define _AD1115_H_

#include <stdint.h>

class AD1115 {
  float fsvolt;
  uint8_t gain_bits, mux_bits, rate_bits;
  int tsamp_usec;
 public:
  void initialise();
  uint16_t read(int channel);
  void set_gain(int gain);
  void set_rate(int rate);
  float convert(uint16_t val);
};

#ifdef _MAIN_
AD1115 ad1115;
#else
extern AD1115 ad1115;
#endif

#endif
