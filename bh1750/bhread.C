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
  int fd=wiringPiI2CSetup(0x23);
  if(fd<0) {
    cerr << "Failed to initialise I2C device" << endl;
    exit(1);
  }

  wiringPiI2CWrite(fd, 0x01);
  usleep(50000);
  wiringPiI2CWrite(fd, 0x11);
  while(1) {
    usleep(180000);
    unsigned char buf[2];
    read(fd, buf, 2);
    int val=buf[1] << 8 | buf[0];
    cout << val*1.2/2.0 << endl;
    sleep(1);
  }
}
