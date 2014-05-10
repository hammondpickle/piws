#include <iostream>
#include <stdint.h>

#include "i2c.h"
#include "ad1115.h"

#define AD1115_ADDR 0x48
#define AD1115_CONFIG_REG 0x01

using namespace std;

//
//
void AD1115::initialise(void)
{
  i2c.initialise();

  // Put device into highest resolution mode
  i2c.setSlaveAddress(AD1115_ADDR);
}

//
//
uint16_t AD1115::read(int channel)
{
  i2c.setSlaveAddress(AD1115_ADDR);
  uint8_t mux;
  switch(channel) {
  case 0:
    mux=4;
    break;
  case 1:
    mux=3;
    break;
  }
  uint8_t gain=1;

  valarray<uint8_t> bytes(3);
  bytes[0]=AD1115_CONFIG_REG;
  bytes[1]=128 | (mux << 4) | (gain << 1);
  bytes[2]=3;
  i2c.writeBytes(bytes);
  usleep(150000);
  uint16_t val=i2c.readRegisterWordMSB(0);
  return val;
}
