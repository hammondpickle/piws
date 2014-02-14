#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <fstream>

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
  int mtreg=254;
  wiringPiI2CWrite(fd, 0x40 | (mtreg >> 5));
  wiringPiI2CWrite(fd, 0x60 | (mtreg & 0x1f));
  ofstream ofs("lux.log");
  while(1) {
    sleep(10);
    unsigned char buf[2];
    read(fd, buf, 2);
    int val=buf[0] << 8 | buf[1];
    cout << val*1.2*(69.0/mtreg)/2.0 << endl;
    ofs << val*1.2*(69.0/mtreg)/2.0 << endl;
  }
  ofs.close();
}
