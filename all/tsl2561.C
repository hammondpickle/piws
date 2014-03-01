#include <iostream>
#include <cmath>
#include <unistd.h>

#include "i2c.h"
#include "tsl2561.h"

#define TSL2561_ADDR 0x39
#define TSL2561_COMMAND 0x80

#define TSL2561_REG_CONTROL 0x00
#define TSL2561_REG_TIMING_GAIN 0x01
#define TSL2561_REG_DATA0LOW 0x0C
#define TSL2561_REG_DATA0HIGH 0x0D
#define TSL2561_REG_DATA1LOW 0x0E
#define TSL2561_REG_DATA1HIGH 0x0F

#define TSL2561_CMD_POWERUP 0x03

using namespace std;

//
//
void TSL2561::initialise(void)
{
  i2c.initialise();
  i2c.setSlaveAddress(TSL2561_ADDR);
  i2c.writeByte(TSL2561_COMMAND | TSL2561_REG_CONTROL);
  i2c.writeByte(TSL2561_CMD_POWERUP);
  i2c.writeByte(TSL2561_COMMAND | TSL2561_REG_TIMING_GAIN);
  i2c.writeByte(0x12);
  usleep(400000);
  integ=402.0;
  gain=1.0;
}

//
//
void TSL2561::read(float& luxVisible)
{
  i2c.setSlaveAddress(TSL2561_ADDR);
  i2c.writeByte(TSL2561_COMMAND | TSL2561_REG_DATA0LOW);
  int d0=i2c.readByte();
  i2c.writeByte(TSL2561_COMMAND | TSL2561_REG_DATA0HIGH);
  d0 |= i2c.readByte() << 8;
  i2c.writeByte(TSL2561_COMMAND | TSL2561_REG_DATA1LOW);
  int d1=i2c.readByte();
  i2c.writeByte(TSL2561_COMMAND | TSL2561_REG_DATA1HIGH);
  d1 |= i2c.readByte() << 8;

  //
  double ch0=d0*402.0/(integ*gain);
  double ch1=d1*402.0/(integ*gain);
  double ratio=ch1/ch0;
  float lux;
  if(ratio<=0.5) lux=0.0304*ch0-0.062*ch0*powf(ratio, 1.4);
  else if(ratio<=0.61) lux=0.0224*ch0-0.031*ch1;
  else if(ratio<=0.80) lux=0.0128*ch0-0.0153*ch1;
  else if(ratio<=1.30) lux=0.00146*ch0-0.00112*ch1;
  else lux=0.0;
  luxVisible=lux;
}
