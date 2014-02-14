#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <wiringPiI2C.h>

using namespace std;

signed short readSS(int fd, int reg)
{
  unsigned char msb=wiringPiI2CReadReg8(fd, reg);
  unsigned char lsb=wiringPiI2CReadReg8(fd, reg+1);
  return (msb<<8) | lsb;
}

unsigned short readUS(int fd, int reg)
{
  unsigned char msb=wiringPiI2CReadReg8(fd, reg);
  unsigned char lsb=wiringPiI2CReadReg8(fd, reg+1);
  return (msb<<8) | lsb;
}

int read24(int fd, int reg)
{
  unsigned char msb=wiringPiI2CReadReg8(fd, reg);
  unsigned char lsb=wiringPiI2CReadReg8(fd, reg+1);
  unsigned char xlsb=wiringPiI2CReadReg8(fd, reg+2);
  return (msb<<16) | (lsb<<8) | xlsb;
}

int main(int argc, char *argv[])
{
  int fd=wiringPiI2CSetup(0x77);
  if(fd<0) {
    cerr << "Failed to initialise I2C device" << endl;
    exit(1);
  }

  int ac1=readSS(fd, 0xAA);
  int ac2=readSS(fd, 0xAC);
  int ac3=readSS(fd, 0xAE);
  int ac4=readUS(fd, 0xB0);
  int ac5=readUS(fd, 0xB2);
  int ac6=readUS(fd, 0xB4);
  int b1=readSS(fd, 0xB6);
  int b2=readSS(fd, 0xB8);
  int mb=readSS(fd, 0xBA);
  int mc=readSS(fd, 0xBC);
  int md=readSS(fd, 0xBE);
  cout << ac1 << " " << ac2 << " " << ac3 << " " << ac4 << " " << ac5 << " " << ac6 << endl;
  cout << b1 << " " << b2 << " " << mb << " " << mc << " " << md << endl;

  //
  wiringPiI2CWriteReg8(fd, 0xF4, 0x2E);
  usleep(5000);
  long ut=readUS(fd, 0xF6);
  cout << "ut=" << ut << endl;
  long x1=(((long)ut-(long)ac6)*(long)ac5) >> 15;
  long x2=((long)mc << 11)/(x1+md);
  long b5=x1+x2;
  long t=(b5+8) >> 4;
  cout << "t=" << t*0.1 << endl;

  //
  int oss=3;
  wiringPiI2CWriteReg8(fd, 0xF4, 0x34 | (oss << 6));
  usleep(30000);
  long up=read24(fd, 0xF6) >> (8-oss);

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
  cout << "p=" << p*0.01 << endl;

  float p0=101325;
  float alt=44330.8*(1.0-powf((float)p/p0, 1.0/5.255));
  //  cout << "alt=" << alt << endl;
}
