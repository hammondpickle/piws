#ifndef _TMP102_H_
#define _TMP102_H_

class TMP102 {
 public:
  void initialise();
  void read(float& temp);
};

#ifdef _MAIN_
TMP102 tmp102;
#else
extern TMP102 tmp102;
#endif

#endif
