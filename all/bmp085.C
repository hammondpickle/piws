#include <iostream>
#include <cmath>
#include <unistd.h>

#include "i2c.h"
#include "bmp085.h"

#define BMP085_ADDR 0x77

using namespace std;

signed short readSS(int reg)
{
  i2c.writeByte(reg);
  uint8_t lsb, msb;
  i2c.readBytePair(msb, lsb);
  return (msb<<8) | lsb;
}

unsigned short readUS(int reg)
{
  uint8_t lsb, msb;
  i2c.writeByte(reg);
  i2c.readBytePair(msb, lsb);
  return (msb<<8) | lsb;
}

int read24(int reg)
{
  i2c.writeByte(reg);
  uint8_t lsb, msb;
  i2c.readBytePair(msb, lsb);
  i2c.writeByte(reg+2);
  uint8_t xlsb=i2c.readByte();
  return (msb<<16) | (lsb<<8) | xlsb;
}

//
//
void BMP085::initialise(void)
{
  i2c.initialise();
  i2c.setSlaveAddress(BMP085_ADDR);
  ac1=readSS(0xAA);
  ac2=readSS(0xAC);
  ac3=readSS(0xAE);
  ac4=readUS(0xB0);
  ac5=readUS(0xB2);
  ac6=readUS(0xB4);
  b1=readSS(0xB6);
  b2=readSS(0xB8);
  mb=readSS(0xBA);
  mc=readSS(0xBC);
  md=readSS(0xBE);
}

//
//
void BMP085::read(float& pressure, float& temp)
{
  i2c.setSlaveAddress(BMP085_ADDR);

  // Start temperature conversion
  i2c.writeBytePair(0xf4, 0x2e);
  usleep(5000);
  long ut=readUS(0xF6);
  long x1=(((long)ut-(long)ac6)*(long)ac5) >> 15;
  long x2=((long)mc << 11)/(x1+md);
  long b5=x1+x2;
  long t=(b5+8) >> 4;
  temp=t*0.1;

  // Start pressure conversion
  //
  int oss=3;
  i2c.writeBytePair(0xf4, 0x34 | (oss << 6));
  usleep(30000);
  long up=read24(0xF6) >> (8-oss);

  long b6=b5-4000;
  x1=(b2*(b6*b6) >> 12) >> 11;
  x2=(ac2*b6) >> 11;
  long x3=x1+x2;
  long b3=(((ac1*4+x3) << oss)+2) >> 2;
  x1=(ac3*b6) >> 13;
  x2=(b1*((b6*b6)>>12)) >> 16;
  x3=((x1+x2)+2)/4;
  unsigned long b4=(ac4*(unsigned long)(x3+32768)) >> 15;
  unsigned long b7=((unsigned long)up-b3)*(50000 >> oss);
  long p=(b7<0x80000000)?(b7*2)/b4:(b7/b4)*2;
  x1=(p >> 8)*(p >> 8);
  x1=(x1*3038) >> 16;
  x2=(-7357*p) >> 16;
  p+=(x1+x2+3791) >> 4;
  pressure=p*0.01;

  float p0=101325;
  float alt=44330.8*(1.0-powf((float)p/p0, 1.0/5.255));
  //  cout << "alt=" << alt << endl;
}
