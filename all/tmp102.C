#include <iostream>
#include <stdint.h>

#include "i2c.h"
#include "tmp102.h"

// Address assumes that the ADD0 line is tied to GND
#define TMP102_ADDR 0x48
#define TMP102_REG 0x00

using namespace std;

//
//
void TMP102::initialise(void)
{
  i2c.initialise();
}

//
//
void TMP102::read(float& temp)
{
  i2c.setSlaveAddress(TMP102_ADDR);
  i2c.writeByte(TMP102_REG);
  uint8_t msb, lsb;
  i2c.readBytePair(msb, lsb);
  short int val=(msb << 8) | lsb;
  val>>=4;
  if(val & (1<<11)) val|=0xF800;
  temp=val*0.0625;
}
