/* 
 * File:   main.cpp
 * Author: Brian Shortridge
 *
 * Created on 29 March 2018, 21:54
 */
#include <iostream>
#include <podofo/podofo.h>

#include <cstdlib>
#include "Driver.h"

#include <iomanip>

#include "LaptimesToJson.h"
#include "JsonToDrivers.h"
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
  int driversToCompare[NUM_DRIVERS] = {0};
  bool compareIndividuals = false;
  
  if (argc < 2)
  {
    std::cout << "Usage:\t" << argv[0]
      << " <pdf File> [driver number] [driver number] ..." << std::endl;
    std::cout << "Or:\t" << argv[0]
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
  
  std::string inputFile (argv[1]);
  if (inputFile.rfind (".pdf") != std::string::npos)
  {
    try {
      inputFile = LaptimesToJson::convertPdfToJson (inputFile);
    } catch (const PoDoFo::PdfError &eCode) {
      eCode.PrintErrorMsg ();
      return eCode.GetError ();
    }
  }
  
  if (inputFile.rfind (".json") != std::string::npos)
  {
    std::vector<Driver> driverVector;
    driverVector = JsonToDrivers::convertToDriverObjects (inputFile);

    // Now that we have the data we can play
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
  }
  else
  {
    std::cout << "Unrecognised file type, use lowercase extensions" << std::endl;
  }
  
  return 0;
}

