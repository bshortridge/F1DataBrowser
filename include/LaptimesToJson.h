/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LaptimesToJson.h
 * Author: Brian Shortridge
 *
 * Created on 06 April 2018, 14:39
 */

#ifndef LAPTIMESTOJSON_H
#define LAPTIMESTOJSON_H

enum {
    DRIVER_NUMBER = 0,
    DRIVER_NAME,
    LAP_NUMBER,
    LAP_NOTES,
    LAP_TIME
};

class LaptimesToJson {
public:
  LaptimesToJson();
  LaptimesToJson(const LaptimesToJson& orig);
  virtual ~LaptimesToJson();
  
  static std::string convertPdfToJson (const std::string pdfFile);
private:

};

#endif /* LAPTIMESTOJSON_H */

