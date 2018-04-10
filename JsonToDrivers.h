/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JsonToDrivers.h
 * Author: brian
 *
 * Created on 06 April 2018, 14:55
 */

#ifndef JSONTODRIVERS_H
#define JSONTODRIVERS_H

#include "Driver.h"

class JsonToDrivers {
public:
  JsonToDrivers();
  JsonToDrivers(const JsonToDrivers& orig);
  virtual ~JsonToDrivers();
  
  static std::vector<Driver> convertToDriverObjects (const char *jsonFile);
private:

};

#endif /* JSONTODRIVERS_H */

