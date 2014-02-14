#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "i2c.h"

namespace I2C {
  //
  int Interface::smbusAccess(char rw, uint8_t command, int size, union i2c_smbus_data *data)
  {
    struct i2c_smbus_ioctl_data args ;
    args.read_write=rw;
    args.command=command;
    args.size=size;
    args.data=data;
    return ioctl(fd, I2C_SMBUS, &args);
  };

  // Initialise the I2C device
  void Interface::initialise(void)
  {
    const char *device="/dev/i2c-1";
    fd=open(device, O_RDWR);
    if(fd<0) throw Exception("Cannot initialise I2C interface");
  }

  // Set the slave address of the device
  void Interface::setSlaveAddress(SlaveAddress addr)
  {
    int ret=ioctl(fd, I2C_SLAVE, addr);
    if(ret<0) throw Exception("Error setting I2C slave address");
  }

  //
  uint8_t Interface::read(void)
  {
    union i2c_smbus_data data;
    int ret=smbusAccess(I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data);
    if(ret) throw Exception("Error reading I2C bus");
    return data.byte & 0xFF;
  }

  //
  uint8_t Interface::readRegisterByte(int reg)
  {
    union i2c_smbus_data data;
    int ret=smbusAccess(I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data);
    if(ret) throw Exception("Error reading I2C device register");
    return data.byte & 0xFF;
  }

  //
  uint16_t Interface::readRegisterWord(int reg)
  {
    union i2c_smbus_data data;
    int ret=smbusAccess(I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA, &data);
    if(ret) throw Exception("Error reading I2C device register");
    return data.byte & 0xFF;
  }

};

