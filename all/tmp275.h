#ifndef _TMP275_H_
#define _TMP275_H_

class TMP275 {
 public:
  void initialise();
  void read(float& temp);
};

#ifdef _MAIN_
TMP275 tmp275;
#else
extern TMP275 tmp275;
#endif

#endif
