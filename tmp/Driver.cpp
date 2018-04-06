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

std::string Driver::toString ()
{
  std::ostringstream out;
  out << std::dec << (unsigned int) m_driverNumber << ": ";
  return out.str();
}
