/* 
 * File:   Driver.cpp
 * Author: coherense-dev
 * 
 * Created on 05 April 2018, 16:11
 */

#include <sstream>

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
