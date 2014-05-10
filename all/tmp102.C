#include <iostream>
#include <stdint.h>

#include "i2c.h"
#include "tmp102.h"

#define TMP102_ADDR 0x49
#define TMP102_TEMP_REG 0x00
#define TMP102_CTRL 0x01

using namespace std;

//
//
void TMP102::initialise(void)
{
  i2c.initialise();

  // Put device into highest resolution mode
  i2c.setSlaveAddress(TMP102_ADDR);
  i2c.writeBytePair(TMP102_CTRL, 0x60);
}

//
//
void TMP102::read(float& temp)
{
  i2c.setSlaveAddress(TMP102_ADDR);
  i2c.writeByte(TMP102_TEMP_REG);
  uint8_t msb, lsb;
  i2c.readBytePair(msb, lsb);
  short int val=(msb << 8) | lsb;
  val>>=4;
  if(val & (1<<11)) val|=0xF800;
  temp=(val+0.5)*0.0625;
}
