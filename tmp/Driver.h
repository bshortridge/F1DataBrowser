/* 
 * File:   Driver.h
 * Author: coherense-dev
 *
 * Created on 05 April 2018, 16:11
 */

#ifndef DRIVER_H
#define DRIVER_H

#include <vector>
#include <map>

#include "Lap.h"

class Driver {
public:
  Driver();
  virtual ~Driver();
  
  void number (unsigned int number);
  unsigned int number ();
  
  void name (std::string name);
  std::string name ();
  
  void lap (Lap lap);
  Lap lap (unsigned int lapNumber);
  
  std::string toString ();
  
  std::string raceAnalysis ();
  std::string race ();
  std::map<int,std::string> lapMap ();
private:
  std::string ulToLapString (unsigned long laptime);
  
  unsigned char     m_driverNumber;
  std::string       m_driverName;
  std::vector<Lap>  m_laps;
};

#endif /* DRIVER_H */

