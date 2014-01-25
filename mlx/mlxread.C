#include <iostream>
#include <errno.h>
#include <string.h>

#include <wiringPiI2C.h>

using namespace std;

int main(int argc, char *argv[])
{
  int ifd=wiringPiI2CSetup(0x5a);
  if(ifd<0) {
    cerr << "Error opening I2C port: " << strerror(errno) << endl;
  }
  int val=wiringPiI2CReadReg16(ifd, 0x07);
  cout << "0x04=" << val << endl;
}
