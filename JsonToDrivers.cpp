/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JsonToDrivers.cpp
 * Author: brian
 * 
 * Created on 06 April 2018, 14:55
 */
#include <fstream>
#include <iostream>

#include "jsoncpp/json/json.h"

#include "JsonToDrivers.h"

std::vector<Driver> JsonToDrivers::convertToDriverObjects (const std::string jsonFile)
{
  Json::Value root;
  
  std::ifstream file (jsonFile);
  file >> root;
  
  Json::Value drivers = root["drivers"];
  int numDrivers = drivers.size ();
  
  std::vector<Driver> driverVector;
  for (int i = 0; i < numDrivers; i++)
  {
    Driver driver;
    driver.number (drivers[i]["driverNumber"].asInt());
    driver.name (drivers[i]["driverName"].asString());
    int laps = drivers[i]["laps"].size ();
    for (int j = 0; j < laps; j++)
    {
      Json::Value jsonLap = drivers[i]["laps"][j];
      Lap lap;
      int lapnum = jsonLap["lapNo"].asInt ();
      lap.number (lapnum);
      lap.notes (jsonLap["pitted"].asString ());
      lap.laptime (jsonLap["lapTime"].asString ());
      lap.toString ();
      driver.lap (lap);
    }
    driverVector.push_back (driver);
  }
  return driverVector;
}