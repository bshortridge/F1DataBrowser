/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LaptimesToJson.cpp
 * Author: Brian Shortridge
 * 
 * Created on 06 April 2018, 14:39
 */
#include <podofo/podofo.h>
#include <iostream>
#include <fstream>

#include "LaptimesToJson.h"

std::string LaptimesToJson::convertPdfToJson (const std::string pdfFile)
{
  using namespace PoDoFo;
  int LapNo = 0;
  int state = DRIVER_NUMBER;
  std::ostringstream jsonSS;

  PdfMemDocument pdf (pdfFile.c_str ());
  jsonSS << "{" << std::endl;
  jsonSS << "  \"drivers\": [";
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
//          jsonSS << "<" << token << "> ";
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
                  jsonSS << "\n    {";
                  jsonSS << "\n      \"driverNumber\" : " << driverNumber << ",\n";
                  state = DRIVER_NAME;
                }
                break;
              }
              case (DRIVER_NAME):
              {
                jsonSS << "      \"driverName\" : \"" << varStr << "\",\n";
                jsonSS << "      \"laps\" : [\n";
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
                      jsonSS << "," << std::endl;
                    }
                    jsonSS << "        { \"lapNo\" : " << currentLapNo << ", ";
                    LapNo = currentLapNo;
                    state = LAP_PITTED;
                  }
                  else if (currentLapNo != 1 && currentLapNo != 90)
                  { // This is a driver number
                    jsonSS << "\n      ]";
                    jsonSS << "\n    },";
                    jsonSS << "\n    {";
                    jsonSS << "\n      \"driverNumber\" : " << currentLapNo << ",\n";
                    LapNo = 0;
                    state = DRIVER_NAME;
                  }
                }
                break;
              }
              case (LAP_PITTED):
              {
                jsonSS << "\"lapNotes\" : \"" << varStr << "\", ";
                state = LAP_TIME;
                break;
              }
              case (LAP_TIME):
              {
                //16:15:00
                //1:32.173
                jsonSS << "\"lapTime\" : \"" << varStr << "\" }";
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
  jsonSS  << "\n      ]"
          << "\n    }"
          << "\n  ]"
          << "\n}" << std::endl;
  
  std::string jsonFileName (pdfFile);
  std::size_t pdfExtension = jsonFileName.find (".pdf");
  if (std::string::npos != pdfExtension)
  {
    jsonFileName.replace (pdfExtension, std::string::npos, ".json");
    std::ofstream outFile (jsonFileName);
    outFile << jsonSS.str();
    outFile.close();
  }
  
  return jsonFileName;
}
