/* 
 * File:   Driver.h
 * Author: coherense-dev
 *
 * Created on 05 April 2018, 16:11
 */

#ifndef DRIVER_H
#define DRIVER_H

class Driver {
public:
  Driver();
  virtual ~Driver();
  
  void number (unsigned char number);
  unsigned char number ();
  
  std::string toString ();
private:
  unsigned char m_driverNumber;
};

#endif /* DRIVER_H */

