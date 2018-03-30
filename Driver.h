/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Driver.h
 * Author: bitwise
 *
 * Created on 30 March 2018, 10:28
 */

#ifndef DRIVER_H
#define DRIVER_H

class Driver {
public:
    Driver();
    Driver(const Driver& orig);
    virtual ~Driver();
        
    void DriverNumber (int driverNum);
    int DriverNumber ();
private:
    int driverNumber;
};

#endif /* DRIVER_H */

