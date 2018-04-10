/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Lap.h
 * Author: brian
 *
 * Created on 06 April 2018, 15:37
 */

#ifndef LAP_H
#define LAP_H

#include <string>

class Lap {
public:
  Lap();
  Lap(const Lap& orig);
  virtual ~Lap();
  
  void number (unsigned char lapNumber);
  int number ();
  
  void notes (std::string notes);
  std::string notes ();
  
  void lapTime (std::string lapTime);
  std::string lapTime ();
  
  void toString ();
private:
  unsigned char m_lapNumber;
  std::string   m_notes;
  std::string   m_lapTime;
};

#endif /* LAP_H */

