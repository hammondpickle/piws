#include <iostream>
#include <fstream>

#define _MAIN_

#include "i2c.h"
#include "mlx90614.h"
#include "tmp102.h"
#include "tsl2561.h"
#include "bmp085.h"
#include "htu21d.h"

using namespace std;

int main(int argc, char *argv[])
{
  // Initialise the sensors
  mlx90614.initialise();
  tmp102.initialise();
  tsl2561.initialise();
  bmp085.initialise();
  htu21d.initialise();

  ofstream ofs("piws.xls");
  while(1) {
    // Read the infra-red thermometer
    float ambientTemp, remoteTemp;
    mlx90614.read(ambientTemp, remoteTemp);
    
    // Read the TMP102 temperature sensor
    float accTemp;
    tmp102.read(accTemp);
    
    // Read the light-level sensor
    float luxVisible;
    tsl2561.read(luxVisible);
    
    // Read pressure (and temperature)
    float temp085, pressure;
    bmp085.read(pressure, temp085);
    
    // Read humidity (and temperature)
    float humidity, temp21, dewpoint;
    htu21d.read(humidity, temp21, dewpoint);
    
    cout << "Tamb=" << accTemp << " Tsky=" << remoteTemp << " press=" << pressure
	 << " humid=" << humidity << " dewpt=" << dewpoint << " lux=" << luxVisible << endl;
    ofs << accTemp << "," << remoteTemp << "," << pressure << "," << humidity << ","
	<< dewpoint << "," << luxVisible << endl;
    sleep(10);
  }
}
