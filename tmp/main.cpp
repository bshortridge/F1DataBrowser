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
  const int NUM_DRIVERS = 20;
  Json::Value root;
  int driversToCompare[NUM_DRIVERS];
  bool compareIndividuals = false;
  
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0]
      << " <json File> [driver number] [driver number] ..." << std::endl;
    return 0;
  }
  else if (argc > 2)
  {
    compareIndividuals = true;
    for (int i = 2; i < argc; i++)
    {
      driversToCompare[i - 2] = std::stoi (argv[i]);
    }
  }
  const char *jsonFile = argv[1];
  
  std::ifstream file (jsonFile);
  file >> root;
  
  Json::Value drivers = root["drivers"];
  int numDrivers = drivers.size ();
  
  std::vector<Driver> driverVector;
  for (int i = 0; i < numDrivers; i++)
  {
    Driver driver;
    driver.number (drivers[i]["driverNumber"].asInt());
    driver.name (drivers[i]["driverName"].asString ());
    unsigned int laps = drivers[i]["laps"].size ();
    for (int j = 0; j < laps; j++)
    {
      Json::Value jsonLap = drivers[i]["laps"][j];
      Lap lap;
      lap.number (jsonLap["lapNo"].asUInt ());
      lap.notes (jsonLap["lapNotes"].asString ());
      lap.laptime (jsonLap["lapTime"].asString ());
      driver.lap (lap);
    }
    driverVector.push_back (driver);
  }
  
  // Now that we have the data, we can play.
  
  for (std::vector<Driver>::iterator it = driverVector.begin (); it != driverVector.end (); ++it)
  {
    if (true == compareIndividuals)
    {
      for (int i = 0; i < NUM_DRIVERS; i++)
      {
        if ((*it).number () == driversToCompare[i])
        {
          std::cout << (*it).raceAnalysis () << std::endl;
        }
      }
    }
    else
    {
      std::cout << (*it).raceAnalysis () << std::endl;
    }
  }
  return 0;
}

