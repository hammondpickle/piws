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
  set_gain(2);
  set_rate(4);
}

//
//
void AD1115::set_gain(int gain)
{
  gain_bits=gain & 0x07;
  switch(gain_bits) {
  case 0:
    fsvolt=6.144;
    break;
  case 1:
    fsvolt=4.096;
    break;
  case 2:
    fsvolt=2.048;
    break;
  case 3:
    fsvolt=1.024;
    break;
  case 4:
    fsvolt=0.512;
    break;
  case 5:
  case 6:
  case 7:
    fsvolt=0.256;
    break;
  }
}

//
//
void AD1115::set_rate(int rate)
{
  rate_bits=rate & 0x07;
 switch(rate_bits) {
 case 0:
   tsamp_usec=125000;
   break;
 case 1:
   tsamp_usec=62500;
   break;
 case 2:
   tsamp_usec=31250;
   break;
 case 3:
   tsamp_usec=15625;
   break;
 case 4:
   tsamp_usec=7813;
   break;
 case 5:
   tsamp_usec=4000;
   break;
 case 6:
   tsamp_usec=2106;
   break;
 case 7:
   tsamp_usec=1163;
   break;
 }
}


//
//
float AD1115::convert(uint16_t ival)
{
  int sval;
  if(ival & 0x8000) {
    sval=-(1<<16)+ival;
  } else {
    sval=ival;
  }
  return sval*fsvolt/32768.0;
}

//
//
uint16_t AD1115::read(int channel)
{
  i2c.setSlaveAddress(AD1115_ADDR);
  uint8_t mux=(channel & 0x07);
  valarray<uint8_t> bytes(3);
  bytes[0]=AD1115_CONFIG_REG;
  bytes[1]=128 | (mux << 4) | (gain_bits << 1);
  bytes[2]=128 | (rate_bits << 5) | 3;
  i2c.writeBytes(bytes);
  usleep(tsamp_usec);
  while((i2c.readRegisterWordMSB(1) & 0x80)==0);
  uint16_t val=i2c.readRegisterWordMSB(0);
  return val;
}
