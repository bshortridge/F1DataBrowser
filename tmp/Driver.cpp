/* 
 * File:   Driver.cpp
 * Author: coherense-dev
 * 
 * Created on 05 April 2018, 16:11
 */

#include <sstream>
#include <iomanip>

#include "Driver.h"

Driver::Driver ()
{
}

Driver::~Driver ()
{
}

void Driver::number (unsigned char number)
{
  m_driverNumber = number;
}

unsigned char Driver::number ()
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
  for (unsigned int i = 1; i < numLaps; i++)
  { // We ignore the 1st lap because we only have the start time
    Lap l = m_laps.at (i);
    unsigned long laptime = Lap::timeStringToMs (l.laptime ());
    if (laptime > slowestLap) { slowestLap = laptime; }
    if (laptime < fastestLap) { fastestLap = laptime; }
    if (laptime < (fastestLap * 1.1))
    { // ignore pit laps and very slow laps
      totalTime += laptime;
      laps++;
    }
    else if (laps > 0)
    {
      unsigned long average = totalTime / laps;
      int minute = (average / 1000) / 60;
      int second = (average / 1000) % 60;
      int millisecond = average % 1000;
      out.fill('0');
      out << " [" << laps << "|" << minute << ":" << std::setw(2) << second << "."
        << std::setw(3) << millisecond << "] " << l.number ();
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
      int minute = (average / 1000) / 60;
      int second = (average / 1000) % 60;
      int millisecond = average % 1000;
      out.fill('0');
      out << " [" << laps << "|" << minute << ":"
        << std::setw(2) << second << "." << std::setw(3) << millisecond;
      laps = 0;
      totalTime = 0;
  }
  out << "]";
  int minute = (slowestLap / 1000) / 60;
  int second = (slowestLap / 1000) % 60;
  int millisecond = slowestLap % 1000;
  out << " S|" << minute << ":" << std::setw(2) << second << "."
        << std::setw(3) << millisecond;
  minute = (fastestLap / 1000) / 60;
  second = (fastestLap / 1000) % 60;
  millisecond = fastestLap % 1000;
  out << " F|" << minute << ":" << std::setw(2) << second << "."
        << std::setw(3) << millisecond;
  return out.str ();
  
}
