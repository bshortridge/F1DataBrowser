/* 
 * File:   Driver.h
 * Author: coherense-dev
 *
 * Created on 05 April 2018, 16:11
 */

#ifndef DRIVER_H
#define DRIVER_H

#include <vector>

#include "Lap.h"

class Driver {
public:
  Driver();
  virtual ~Driver();
  
  void number (unsigned char number);
  unsigned char number ();
  
  void name (std::string name);
  std::string name ();
  
  void lap (Lap lap);
  Lap lap (unsigned int lapNumber);
  
  std::string toString ();
private:
  unsigned char     m_driverNumber;
  std::string       m_driverName;
  std::vector<Lap>  m_laps;
};

#endif /* DRIVER_H */

