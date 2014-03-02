#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "i2c.h"
#include "bcm2835.h"

using namespace std;

namespace I2C {
  Interface::~Interface()
  {
    init=false;
    bcm2835_i2c_end();
    bcm2835_close();
  }

  void Interface::initCheck(void)
  {
    if(!init) throw Exception("I2C interface not initialised");
  }

  // Initialise the I2C device
  void Interface::initialise(void)
  {
    if(init) return;
    bcm2835_init();
    bcm2835_i2c_begin();
    init=true;
  }

  // Set baud rate
  void Interface::setBaudRate(uint32_t rate)
  {
    bcm2835_i2c_set_baudrate(rate);
  }

  // Set the slave address of the device
  void Interface::setSlaveAddress(SlaveAddress addr)
  {
    initCheck();
    bcm2835_i2c_setSlaveAddress(addr);
  }

  //
  void Interface::writeByte(uint8_t byte)
  {
    bcm2835_i2c_write((char*)&byte, 1);
  }

  //
  void Interface::writeBytePair(uint8_t byte0, uint8_t byte1)
  {
    uint8_t buf[2];
    buf[0]=byte0;
    buf[1]=byte1;
    bcm2835_i2c_write((char*)&buf, 2);
  }

  //
  uint8_t Interface::readByte(void)
  {
    uint8_t val;
    bcm2835_i2c_read((char*)&val, 1);
    return val;
  }

  //
  void Interface::readBytePair(uint8_t& byte0, uint8_t& byte1)
  {
    uint8_t val[2];
    bcm2835_i2c_read((char*)&val[0], 2);
    byte0=val[0];
    byte1=val[1];
  }

  //
  uint8_t Interface::readRegisterByte(char reg)
  {
    initCheck();
    char buf;
    bcm2835_i2c_write(&reg, 1);
    bcm2835_i2c_read(&buf, 1);
    return buf;
  }

  uint16_t Interface::readRegisterWordMSB(char reg)
  {
    initCheck();
    char buf[2];
    bcm2835_i2c_write(&reg, 1);
    bcm2835_i2c_read(&buf[0], 2);
    return (buf[0] << 8) | buf[1];
  }

  uint16_t Interface::readRegisterWordLSB(char reg)
  {
    initCheck();
    char buf[2];
    bcm2835_i2c_write(&reg, 1);
    bcm2835_i2c_read(&buf[0], 2);
    return (buf[1] << 8) | buf[0];
  }

  //
  uint16_t Interface::readRegisterWordRS(char reg)
  {
    initCheck();
    char buf[3];
    int ok=bcm2835_i2c_read_register_rs(&reg, &buf[0], 3);
    if(ok) cout << "I2C read error " << ok << endl;
    return (buf[1]<<8) | buf[0];
  }

};

