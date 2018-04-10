/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LaptimesToJson.cpp
 * Author: brian
 * 
 * Created on 06 April 2018, 14:39
 */
#include <podofo/podofo.h>
#include <iostream>

#include "LaptimesToJson.h"

LaptimesToJson::LaptimesToJson ()
{
}

LaptimesToJson::LaptimesToJson (const LaptimesToJson& orig)
{
}

LaptimesToJson::~LaptimesToJson ()
{
}

void LaptimesToJson::convertPdfToJson (const char* pdfFile)
{
  using namespace PoDoFo;
  int LapNo = 0;
  int state = DRIVER_NUMBER;

  PdfMemDocument pdf (pdfFile);
  std::cout << "{" << std::endl;
  std::cout << "  \"drivers\": [";
  for (int pn = 0; pn < pdf.GetPageCount (); ++pn)
  {
    PdfPage* page = pdf.GetPage(pn);
    
    PdfContentsTokenizer tok (page);
    const char* token = NULL;
    PdfVariant var;
    EPdfContentsType type;
    while (tok.ReadNext (type, token, var))
    {
      switch (type)
      {
        case ePdfContentsType_Keyword:
        {
          // process token: it contains the current command
          // pop from var stack as necessary
//          std::cout << "<" << token << "> ";
          break;
        }
        case ePdfContentsType_Variant:
        {
          // process var: push it onto a stack
          if (var.IsString ())
          {
            const char* varStr = var.GetString ().GetString ();
            switch (state)
            {
              case DRIVER_NUMBER:
              {
                int driverNumber = atoi (varStr);
                if (driverNumber)
                {
                  std::cout << "\n    {";
                  std::cout << "\n      \"driverNumber\" : " << driverNumber << ",\n";
                  state = DRIVER_NAME;
                }
                break;
              }
              case (DRIVER_NAME):
              {
                std::cout << "      \"driverName\" : \"" << varStr << "\",\n";
                std::cout << "      \"laps\" : [\n";
                state = LAP_NUMBER;
                break;
              }
              case (LAP_NUMBER):
              {
                int currentLapNo = atoi (varStr);
                if (currentLapNo)
                {
                  if (currentLapNo == LapNo + 1)
                  {
                    if (currentLapNo > 1)
                    {
                      std::cout << "," << std::endl;
                    }
                    std::cout << "        { \"lapNo\" : " << currentLapNo << ", ";
                    LapNo = currentLapNo;
                    state = LAP_PITTED;
                  }
                  else
                  { // This is a driver number
                    std::cout << "\n      ]";
                    std::cout << "\n    },";
                    std::cout << "\n    {";
                    std::cout << "\n      \"driverNumber\" : " << currentLapNo << ",\n";
                    LapNo = 0;
                    state = DRIVER_NAME;
                  }
                }
                break;
              }
              case (LAP_PITTED):
              {
                std::cout << "\"pitted\" : \"" << varStr << "\", ";
                state = LAP_TIME;
                break;
              }
              case (LAP_TIME):
              {
                //16:15:00
                //1:32.173
                std::cout << "\"lapTime\" : \"" << varStr << "\" }";
                state = LAP_NUMBER;
                break;
              }
              default:
              {
                // Shouldn't get here!
                break;
              }
            }
          }
          break;
        }
        default:
        {
          // should not happen!
          break;
        }
      }
    }
  }
  std::cout << "\n    }"
            << "\n  ]"
            << "\n}" << std::endl;
}
