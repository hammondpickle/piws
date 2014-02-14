#ifndef _BMP085_H_
#define _BMP085_H_

class BMP085 {
 private:
  int ac1, ac2, ac3, ac4, ac5, ac6;
  int b1, b2, mb, mc, md;

 public:
  void initialise();
  void read(float& pressure, float& temp);
};

#ifdef _MAIN_
BMP085 bmp085;
#else
extern BMP085 bmp085;
#endif

#endif
