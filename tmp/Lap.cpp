/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Lap.cpp
 * Author: coherense-dev
 * 
 * Created on 09 April 2018, 09:27
 */
#include <sstream>
#include <string>

#include "Lap.h"

Lap::Lap ()
{
}

Lap::~Lap ()
{
}

void Lap::number (unsigned int number)
{
  m_lapNumber = number;
}

unsigned int Lap::number ()
{
  return m_lapNumber;    
}

void Lap::notes (std::string lapNotes)
{
  m_lapNotes = lapNotes;
}

std::string Lap::notes ()
{
  return m_lapNotes;
}

void Lap::laptime (std::string lapTime)
{
  m_lapTime = lapTime;
}

std::string Lap::laptime ()
{
  return m_lapTime;
}

std::string Lap::toString ()
{
  std::ostringstream out;
  
  out << "[";
  out << m_lapNumber;
  out << "|" << m_lapNotes;
  out << "|" << m_lapTime;
  out << "]";

  return out.str();
}

unsigned long Lap::timeStringToMs (std::string timeString)
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
  
  return timeInMs;
}
