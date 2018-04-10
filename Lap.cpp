/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Lap.cpp
 * Author: brian
 * 
 * Created on 06 April 2018, 15:37
 */

#include "Lap.h"
#include <iostream>

Lap::Lap ()
{
}

Lap::Lap (const Lap& orig)
{
}

Lap::~Lap ()
{
}

void Lap::number (unsigned char lapNumber)
{
  m_lapNumber = lapNumber;
}

int Lap::number ()
{
  return m_lapNumber;
}

void Lap::notes (std::string notes)
{
  m_notes = notes;
}

std::string Lap::notes ()
{
  return m_notes;
}

void Lap::lapTime (std::string lapTime)
{
  m_lapTime = lapTime;
}

std::string Lap::lapTime ()
{
  return m_lapTime;
}

void Lap::toString ()
{
  std::cout << "#" << std::dec << m_lapNumber << ": " << m_notes << " " << m_lapTime << std::endl;
}