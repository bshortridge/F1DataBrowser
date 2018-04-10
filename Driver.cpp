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

Lap Driver::lap ()
{
  return m_laps.front ();
}

std::string Driver::toString ()
{
  std::ostringstream out;
  out << std::dec << (unsigned int) m_driverNumber << ": ";
  out << m_driverName;
  out << " [";
  for (std::vector<Lap>::iterator it = m_laps.begin (); it != m_laps.end (); ++it)
  {
    out << " " << (*it).lapTime ();
  }
  out << " ]";
  return out.str();
}
