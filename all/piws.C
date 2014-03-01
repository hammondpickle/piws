#include <iostream>
#include <fstream>
#include <unistd.h>

#define _MAIN_

#include "i2c.h"
#include "mlx90614.h"
#include "tmp275.h"
#include "tsl2561.h"
#include "bmp085.h"
#include "htu21d.h"

using namespace std;

int main(int argc, char *argv[])
{
  // Initialise the sensors
  mlx90614.initialise();
  tmp275.initialise();
  tsl2561.initialise();
  bmp085.initialise();
  htu21d.initialise();

  ofstream ofs("piws.csv", ios::app);
  while(1) {
    // Read the infra-red thermometer
    float ambientTemp, remoteTemp;
    mlx90614.read(ambientTemp, remoteTemp);
    usleep(100000);

    // Read the TMP102 temperature sensor
    float accTemp;
    tmp275.read(accTemp);
    usleep(100000);

    // Read the light-level sensor
    float luxVisible;
    tsl2561.read(luxVisible);
    usleep(100000);
    
    // Read pressure (and temperature)
    float temp085, pressure;
    bmp085.read(pressure, temp085);
    usleep(100000);
    
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
