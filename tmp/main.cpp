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
unsigned long timeStringToMs (std::string timeString)
{
  unsigned long timeInMs;
  std::string hourString ("0"), minuteString ("0"), secondString ("0");
  std::string millisecondString ("0");
  std::size_t startIdx = 0;
  std::size_t colonPos = timeString.find (":");
  std::size_t decimalPos = timeString.find (".");
  if (std::string::npos == decimalPos)
  {
    hourString = timeString.substr (startIdx, colonPos);
    startIdx = colonPos + 1;
    colonPos = timeString.find (":", startIdx);
    decimalPos = timeString.length ();
  }
  
  minuteString = timeString.substr (startIdx, colonPos - startIdx);
  secondString = timeString.substr (colonPos + 1, decimalPos - colonPos);
  if (decimalPos != timeString.length ())
  {
    millisecondString = timeString.substr (decimalPos + 1);
  }
  
  int hours = std::stoi (hourString);
  int minutes = std::stoi (minuteString);
  int seconds = std::stoi (secondString);
  int milliseconds = std::stoi (millisecondString);
  
  timeInMs = (hours * 60 * 60 * 1000) + (minutes * 60 * 1000)
              + (seconds * 1000) + milliseconds;
  
  std::cout << hours << " hours, " << minutes << " minutes, "
    << seconds << " seconds, " << milliseconds << " milliseconds" << std::endl;
  
  return timeInMs;
}

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
    std::cout << drivers[i]["driverNumber"] << ": " << drivers[i]["driverName"]
      << " laps: " << drivers[i]["laps"].size ();
    std::cout << std::endl;
    driverVector.push_back (driver);
  }
  for (std::vector<Driver>::iterator it = driverVector.begin (); it != driverVector.end (); ++it)
  {
    std::cout << (*it).toString () << std::endl;
  }
  return 0;
}

