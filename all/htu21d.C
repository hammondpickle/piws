#include <iostream>
#include <cmath>

#include "i2c.h"
#include "htu21d.h"

#define HTU21D_ADDR 0x40

const float tcoeff=-0.15;

using namespace std;

//
//
void HTU21D::initialise(void)
{
  i2c.initialise();
}

//
//
void HTU21D::read(float& humidity, float& temp, float& dewpoint)
{
  i2c.setSlaveAddress(HTU21D_ADDR);

  // Trigger temperature measurement
  i2c.writeByte(0xe3);
  usleep(50000);
  uint8_t lsb, msb;
  i2c.readBytePair(msb, lsb);
  int tval=((msb << 8) | lsb) & 0xfffc;
  temp=-46.85+(175.72*tval)/65536.0;

  // Trigger humidity measurement
  i2c.writeByte(0xe5);
  usleep(16000);
  i2c.readBytePair(msb, lsb);
  int hval=((msb << 8) | lsb) & 0xfffc;
  float rh=-6.0+(125.0*hval)/65536.0;
  humidity=rh+(25.0-temp)*tcoeff;

  //
  const float A=8.1332, B=1762.39, C=235.66;
  float pp=powf(10.0, A-B/(temp+C));
  dewpoint=-(C+B/(log10(rh*pp/100.0)-A));
}
