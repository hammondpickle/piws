#ifndef _I2C_H_
#define _I2C_H_

#include <string>
#include <exception>
#include <stdexcept>

#include <stdint.h>

namespace I2C {
  typedef unsigned char SlaveAddress;
  const int OK=0;
 
  class Exception : public std::runtime_error {
  public:
    Exception(std::string msg) : std::runtime_error(msg) {};
  };

  class Interface {
  private:
    int fd;
    // I2C definitions
    static const int I2C_SLAVE=0x0703;
    static const int I2C_SMBUS=0x0720;
    
    static const int I2C_SMBUS_READ=1;
    static const int I2C_SMBUS_WRITE=0;
    
    // SMBus transaction types
    static const int I2C_SMBUS_QUICK=0;
    static const int I2C_SMBUS_BYTE=1;
    static const int I2C_SMBUS_BYTE_DATA=2;
    static const int I2C_SMBUS_WORD_DATA=3;
    static const int I2C_SMBUS_PROC_CALL=4;
    static const int I2C_SMBUS_BLOCK_DATA=5;
    static const int I2C_SMBUS_I2C_BLOCK_BROKEN=6;
    static const int I2C_SMBUS_BLOCK_PROC_CALL=7;
    static const int I2C_SMBUS_I2C_BLOCK_DATA=8;

    // SMBus messages
    static const int I2C_SMBUS_BLOCK_MAX=32;	/* As specified in SMBus standard */	
    static const int I2C_SMBUS_I2C_BLOCK_MAX=32;	/* Not specified but we use same structure */

    // Structures used in the ioctl() calls
    union i2c_smbus_data {
      uint8_t byte;
      uint16_t word;
      uint8_t block[I2C_SMBUS_BLOCK_MAX+2];	// block [0] is used for length + one more for PEC
    };
    struct i2c_smbus_ioctl_data {
      char read_write;
      uint8_t command;
      int size;
      union i2c_smbus_data *data;
    };

    int smbusAccess(char rw, uint8_t command, int size, union i2c_smbus_data *data);
  public:
    Interface() {fd=-1;};
    ~Interface() {if(fd>=0) close(fd);};
    void initialise(void);
    void setSlaveAddress(SlaveAddress addr);
    uint8_t read(void);
    uint8_t readRegisterByte(int reg);
    uint16_t readRegisterWord(int reg);
  };
};

#ifdef _MAIN_
static I2C::Interface i2c;
#else
extern I2C::Interface i2c;
#endif

#endif
