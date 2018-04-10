/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Driver.h
 * Author: bitwise
 *
 * Created on 30 March 2018, 10:28
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
  Lap lap ();
  
  std::string toString ();
private:
  unsigned char m_driverNumber;
  std::string m_driverName;
  std::vector<Lap> m_laps;  
};

#endif /* DRIVER_H */

