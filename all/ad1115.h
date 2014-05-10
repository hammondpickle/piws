#ifndef _AD1115_H_
#define _AD1115_H_

#include <stdint.h>

class AD1115 {
 public:
  void initialise();
  uint16_t read(int channel);
};

#ifdef _MAIN_
AD1115 ad1115;
#else
extern AD1115 ad1115;
#endif

#endif
