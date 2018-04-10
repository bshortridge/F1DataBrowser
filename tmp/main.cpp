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
#include "gnuplot_i.hpp"

void wait_for_key ()
{
  std::cout << std::endl << "Press ENTER to continue..." << std::endl;

  std::cin.clear();
  std::cin.ignore(std::cin.rdbuf()->in_avail());
  std::cin.get();
  return;
}

/*
 * 
 */
int main (int argc, char** argv)
{
  const int NUM_DRIVERS = 20;
  Json::Value root;
  int driversToCompare[NUM_DRIVERS] = {0};
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
  Gnuplot g1("linespoints");
  g1.cmd ("set ydata time");
  g1.cmd ("set timefmt \"%M:%S\"");
  g1.cmd ("set format y \"%M:%.3S\"");
  g1.set_xlabel ("Lap");
  g1.set_ylabel ("Time");
  g1.set_grid();
  
  std::map<int, std::string> lapMap;
  
  for (std::vector<Driver>::iterator it = driverVector.begin (); it != driverVector.end (); ++it)
  {
    if (true == compareIndividuals)
    {
      for (int i = 0; i < NUM_DRIVERS; i++)
      {
        if ((*it).number () == driversToCompare[i])
        {
          std::cout << (*it).race () << std::endl;
          
          lapMap = (*it).lapMap ();
          std::vector<int> x;
          std::vector<std::string> y;
          for (std::map<int,std::string>::iterator lm = lapMap.begin ();
                lm != lapMap.end (); ++lm)
          {
            x.push_back (lm->first);
            y.push_back (lm->second);
          }
          if (x.size ())
          {
            std::ostringstream driverLabel;
            driverLabel << (*it).number () << " " << (*it).name ();
            g1.plot_xy (x, y, driverLabel.str ());
          }
        }
      }
    }
    else
    {
      std::cout << (*it).raceAnalysis () << std::endl;
      lapMap = (*it).lapMap ();
      std::vector<int> x;
      std::vector<std::string> y;
      for (std::map<int,std::string>::iterator lm = lapMap.begin ();
            lm != lapMap.end (); ++lm)
      {
        x.push_back (lm->first);
        y.push_back (lm->second);
      }
      if (x.size ())
      {
        std::ostringstream driverLabel;
        driverLabel << (*it).number () << " " << (*it).name ();
        g1.plot_xy (x, y, driverLabel.str ());
      }
    }
  }
  wait_for_key ();
  return 0;
}

