/* 
 * File:   Driver.cpp
 * Author: bitwise
 * 
 * Created on 30 March 2018, 10:28
 */

#include "Driver.h"

Driver::Driver() {
}

Driver::Driver(const Driver& orig) {
}

Driver::~Driver() {
}

void Driver::DriverNumber (int driverNum)
{
  driverNumber = driverNum;
}

int Driver::DriverNumber ()
{
  return driverNumber;
}
