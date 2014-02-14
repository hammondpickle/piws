#include <iostream>

#include "i2c.h"
#include "mlx90614.h"

#define MLX90614_ADDR 0x5a
#define MLX90614_REMOTE_TEMP 0x07
#define MLX90614_AMBIENT_TEMP 0x06

using namespace std;

//
//
int MLX90614::initialise(void)
{
  i2c.initialise();
}

//
//
int MLX90614::read(float& ambientTemp, float& remoteTemp)
{
  i2c.setSlaveAddress(MLX90614_ADDR);
  int val=i2c.readRegisterWordRS(MLX90614_REMOTE_TEMP);
  remoteTemp=(val-0.5)*0.02-273.15;
  val=i2c.readRegisterWordRS(MLX90614_AMBIENT_TEMP);
  ambientTemp=(val-0.5)*0.02-273.15;
  return 1;
}
