#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

#include <mysql++/mysql++.h>

#define _MAIN_

#include "i2c.h"
#include "tmp102.h"
#include "ad1115.h"

using namespace std;
using namespace mysqlpp;

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
  tmp102.initialise();
  ad1115.initialise();

  ofstream ofs("ngtmon.csv", ios::app);
  double tbeg=timer();
  while(1) {
    // Read the TMP102 temperature sensor
    float ambTemp;
    tmp102.read(ambTemp);
    uint16_t val=ad1115.read(0);
    cout << ambTemp << " " << val << endl;
    sleep(1);
  }
}
