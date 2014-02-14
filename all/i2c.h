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
    bool init;
    void initCheck(void);
  public:
    Interface() {init=false;};
    ~Interface();
    void initialise(void);
    void setSlaveAddress(SlaveAddress addr);
    void writeByte(uint8_t byte);
    void writeBytePair(uint8_t byte0, uint8_t byte1);
    uint8_t readByte(void);
    void readBytePair(uint8_t& byte0, uint8_t& byte1);
    uint8_t readRegisterByte(char reg);
    uint16_t readRegisterWordMSB(char reg);
    uint16_t readRegisterWordLSB(char reg);
    uint16_t readRegisterWordRS(char reg);
  };
};

#ifdef _MAIN_
I2C::Interface i2c;
#else
extern I2C::Interface i2c;
#endif

#endif
