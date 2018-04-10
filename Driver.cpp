/* 
 * File:   Driver.cpp
 * Author: Brian Shortridge
 * 
 * Created on 05 April 2018, 16:11
 */

#include <sstream>
#include <iomanip>
#include <iostream>
#include "Driver.h"

Driver::Driver ()
{
}

Driver::~Driver ()
{
}

void Driver::number (unsigned int number)
{
  m_driverNumber = number;
}

unsigned int Driver::number ()
{
  return m_driverNumber;
}

void Driver::name (std::string name)
{
  m_driverName = name;
}

std::string Driver::name ()
{
  return m_driverName;
}

void Driver::lap (Lap lap)
{
  m_laps.push_back (lap);
}

Lap Driver::lap (unsigned int lapNumber)
{
  m_laps.at(lapNumber - 1); // Index starts at 0, lapnumber starts at 1
}

std::string Driver::toString ()
{
  std::ostringstream out;
  out << std::dec << (unsigned int) m_driverNumber << ": ";
  out << m_driverName;
  long unsigned int numLaps = m_laps.size ();
  out << " (" << numLaps << ") [ ";
  for (unsigned int i = 0; i < numLaps; i++)
  {
    out << m_laps.at (i).toString () << " ";
  }
  out << "]";
  return out.str();
}

std::string Driver::raceAnalysis ()
{
  std::ostringstream out;
  
  out << std::dec << (unsigned int) m_driverNumber << ": ";
  out << m_driverName;
  out << " ";
  unsigned long totalTime = 0;
  unsigned long fastestLap = 10 * 60 * 60 * 1000; // 10 minutes
  unsigned long slowestLap = 0;
  unsigned int laps = 0;
  long unsigned int numLaps = m_laps.size ();
  // initialise fastest and slowest laps
  for (unsigned int i = 1; i < numLaps; i++)
  { // We ignore the 1st lap because we only have the start time
    Lap l = m_laps.at (i);
    unsigned long laptime = l.laptimeInMs ();
    if (laptime > slowestLap)
    {
      if (l.notes ().find ("P") == std::string::npos)
      {
        slowestLap = laptime;
      }
    }
    if (laptime < fastestLap) { fastestLap = laptime; }
  }
  
  for (unsigned int i = 1; i < numLaps; i++)
  {
    Lap l = m_laps.at (i);
    bool pitLap = (l.notes ().find ("P") != std::string::npos) ? true : false;
    unsigned long laptime = l.laptimeInMs ();
    if ((laptime < (fastestLap * 1.1)) && !pitLap)
    { // ignore pit laps and very slow laps
      totalTime += laptime;
      laps++;
    }
    else if (laps > 0)
    {
      unsigned long average = totalTime / laps;
      out << " [" << laps << "|" << ulToLapString (average) << "] ";
      if (pitLap) { out << "P"; }
      out << l.number ();
      laps = 0;
      totalTime = 0;
    }
    else
    {
      out << "|" << l.number ();
    }
  }
  if (laps > 0)
  {
      unsigned long average = totalTime / laps;
      out << " [" << laps << "|" << ulToLapString (average);
      laps = 0;
      totalTime = 0;
  }
  out << "]";
  out << " S|" << ulToLapString (slowestLap);
  out << " F|" << ulToLapString (fastestLap);
  
  return out.str ();
}

std::string Driver::race ()
{
  std::ostringstream out;
  
  out << raceAnalysis () << std::endl;
  
  long unsigned int numLaps = m_laps.size ();
  for (unsigned int i = 1; i < numLaps; i++)
  {
    Lap l = m_laps.at (i);
    out << "[" << l.number () << "|" << l.laptime () << "]";
  }
  out << std::endl;
  
  return out.str();
}

std::map<int,std::string> Driver::lapMap ()
{
  std::map<int, std::string> lapMap;
  
  long unsigned int numLaps = m_laps.size ();
  for (unsigned int i = 1; i < numLaps; i++)
  {
    Lap l = m_laps.at (i);
    lapMap.insert (std::pair<int, std::string> (l.number (), l.laptime ()));
  }
  return lapMap;
}

std::string Driver::ulToLapString (unsigned long laptime)
{
  std::ostringstream lapSS;
  
  int minute = (laptime / 1000) / 60;
  int second = (laptime / 1000) % 60;
  int millisecond = laptime % 1000;
  lapSS.fill('0');
  lapSS << minute << ":" << std::setw(2) << second << "." 
        << std::setw(3) << millisecond;
  
  return lapSS.str ();
}