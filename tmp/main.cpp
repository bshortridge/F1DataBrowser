/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: coherense-dev
 *
 * Created on 05 April 2018, 15:25
 */

#include <fstream>
#include <iostream>

#include "jsoncpp/json/json.h"

#include "Driver.h"

/*
 * 
 */
int main (int argc, char** argv)
{
  Json::Value root;
  const char *jsonFile = argv[1];
  
  std::ifstream file (jsonFile);
  file >> root;
  std::cout << root;
  std::cout << "\nHello Brian!" << std::endl;
  
  Json::Value drivers = root["drivers"];
  int numDrivers = drivers.size ();
  
  std::vector<Driver> driverVector;
  for (int i = 0; i < numDrivers; i++)
  {
    Driver driver;
    driver.number (drivers[i]["driverNumber"].asInt());
    driver.name (drivers[i]["driverName"].asString ());
    unsigned int laps = drivers[i]["laps"].size ();
    std::cout << " laps: " << laps << " [ ";
    for (int j = 0; j < laps; j++)
    {
      Json::Value jsonLap = drivers[i]["laps"][j];
      Lap lap;
      lap.number (jsonLap["lapNo"].asUInt ());
      lap.notes (jsonLap["lapNotes"].asString ());
      lap.laptime (jsonLap["lapTime"].asString ());
      driver.lap (lap);
    }
    std::cout << "]" << std::endl;
    driverVector.push_back (driver);
  }
  for (std::vector<Driver>::iterator it = driverVector.begin (); it != driverVector.end (); ++it)
  {
    std::cout << (*it).toString () << std::endl;
  }
  std::cout << Lap::timeStringToMs ("01:53.453") << std::endl;
  return 0;
}

