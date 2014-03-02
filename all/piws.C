#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>

#define _MAIN_

#include "i2c.h"
#include "mlx90614.h"
#include "tmp275.h"
#include "tsl2561.h"
#include "bmp085.h"
#include "htu21d.h"

using namespace std;

double timer(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec+tv.tv_usec*1.0e-6;
}

int main(int argc, char *argv[])
{
  i2c.initialise();
  i2c.setBaudRate(25000);

  // Initialise the sensors
  mlx90614.initialise();
  tmp275.initialise();
  tsl2561.initialise();
  bmp085.initialise();
  htu21d.initialise();

  ofstream ofs("piws.csv", ios::app);
  float accTempAcc=0.0, remoteTempAcc=0.0, pressureAcc=0.0, luxAcc=0.0,
    humidityAcc=0.0, dewpointAcc=0.0;
  int nsample=0;
  double tbeg=timer();
  while(1) {
    // Read the infra-red thermometer
    float ambientTemp, remoteTemp;
    //    do {
      mlx90614.read(ambientTemp, remoteTemp);
      //    } while(remoteTemp>100.0);
    remoteTempAcc+=remoteTemp;
    usleep(10000);

    // Read the TMP275 temperature sensor
    float accTemp;
    tmp275.read(accTemp);
    usleep(10000);
    accTempAcc+=accTemp;

    // Read the light-level sensor
    float luxVisible;
    tsl2561.read(luxVisible);
    luxAcc+=luxVisible;
    usleep(10000);
    
    // Read pressure (and temperature)
    float temp085, pressure;
    bmp085.read(pressure, temp085);
    pressureAcc+=pressure;
    usleep(10000);
    
    // Read humidity (and temperature)
    float humidity, temp21, dewpoint;
    htu21d.read(humidity, temp21, dewpoint);
    humidityAcc+=humidity;
    dewpointAcc+=dewpoint;
    usleep(10000);

    nsample++;
    double tnow=timer();
    if(tnow-tbeg>15.0) {
      cout << "Tamb=" << accTempAcc/nsample << " Tsky=" << remoteTempAcc/nsample
	   << " press=" << pressureAcc/nsample << " humid=" << humidityAcc/nsample
	   << " dewpt=" << dewpointAcc/nsample << " lux=" << luxAcc/nsample << endl;
      ofs << accTempAcc/nsample << "," << remoteTempAcc/nsample << "," << pressureAcc/nsample << ","
	  << humidityAcc/nsample << ","  << dewpointAcc/nsample << "," << luxAcc/nsample << endl;
      accTempAcc=remoteTempAcc=pressureAcc=luxAcc=humidityAcc=dewpointAcc=0.0;
      tbeg=tnow;
      nsample=0;
    }
  }
}
