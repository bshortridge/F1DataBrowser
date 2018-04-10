/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Lap.h
 * Author: coherense-dev
 *
 * Created on 09 April 2018, 09:27
 */

#ifndef LAP_H
#define LAP_H

class Lap {
public:
  Lap();
  virtual ~Lap();

  void number (unsigned int number);
  unsigned int number ();
  
  void laptime (std::string lapTime);
  std::string laptime ();
  unsigned long laptimeInMs ();
  
  void notes (std::string lapNotes);
  std::string notes ();
  
  std::string toString ();
private:
  unsigned long timeStringToMs (std::string timeString);

  unsigned int m_lapNumber;
  std::string m_lapTime;
  unsigned long m_lapTimeInMs;
  std::string m_lapNotes;
};

#endif /* LAP_H */

