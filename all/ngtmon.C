#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <cmath>

#include <mysql++/mysql++.h>

#define _MAIN_

#include "i2c.h"
#include "tmp102.h"
#include "ad1115.h"

using namespace std;
using namespace mysqlpp;

// Thermoelectric voltages for a Type K thermocouple (mV)
float tvolt[]={
  0.000,  0.039,  0.079,  0.119,  0.158,  0.198,  0.238,  0.277,  0.317,  0.357,
  0.397,  0.437,  0.477,  0.517,  0.557,  0.597,  0.637,  0.677,  0.718,  0.758,
  0.798,  0.838,  0.879,  0.919,  0.960,  1.000,  1.041,  1.081,  1.122,  1.163,
  1.203,  1.244,  1.285,  1.326,  1.366,  1.407,  1.448,  1.489,  1.530,  1.571,
  1.612,  1.653,  1.694,  1.735,  1.776,  1.817,  1.858,  1.899,  1.941,  1.982,
};

double timer(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec+tv.tv_usec*1.0e-6;
}

float findVoltage(float temp)
{
  if(temp<0.0 || temp>50.0) return -1.0e6;
  int i0=floor(temp);
  float f=temp-i0;
  return tvolt[i0]*(1.0-f)+tvolt[i0+1]*f;
}

float findTemperature(float volt)
{
  int i0=0;
  while(tvolt[i0+1]<volt && i0<50) i0++;
  if(i0==50) return -1;
  float r=volt-tvolt[i0];
  float d=tvolt[i0+1]-tvolt[i0];
  float f=r/d;
  return i0+f;
}

int main(int argc, char *argv[])
{
  i2c.initialise();
  i2c.setBaudRate(25000);

  // Initialise the sensors
  tmp102.initialise();
  ad1115.initialise();
  ad1115.set_gain(5);
  ad1115.set_rate(0);

  ofstream ofs("ngtmon.csv", ios::app);
  do {
    float ambTempAcc=0.0, v0Acc=0.0, v1Acc=0.0;
    int nsamples=0;
    double tbeg=timer();
    while(timer()-tbeg<5.0) {
      // Read the TMP102 temperature sensor
      float ambTemp;
      tmp102.read(ambTemp);
      ambTempAcc+=ambTemp;

      // Read the voltages
      uint16_t val=ad1115.read(0);
      v0Acc+=ad1115.convert(val); 
      val=ad1115.read(3);
      v1Acc+=ad1115.convert(val); 

      //
      nsamples++;
    }
    float ambTemp=ambTempAcc/nsamples;
    float vamb=findVoltage(ambTemp);
    float v0=v0Acc*1000.0/nsamples, v1=v1Acc*1000.0/nsamples;
    float v0a=v0+vamb, v1a=v1+vamb;
    float t0=findTemperature(v0a);
    float t1=findTemperature(v1a);
    int tval=time(NULL);
    cout << ambTemp << " " << v0 << " " << v1 << " " << t0 << " " << t1 << endl;
    ofs << tval << " " << ambTemp << " " << v0 << " " << v1 << " "
	<< t0 << " " << t1 << endl;
  } while(1);
}
