/* 
 * File:   main.cpp
 * Author: brian
 *
 * Created on 29 March 2018, 21:54
 */
#include <iostream>
#include <podofo/podofo.h>
<<<<<<< HEAD
=======
#include <cstdlib>
#include "Driver.h"

#include <iomanip>

enum {
    DRIVER_NUMBER = 0,
    DRIVER_NAME,
    LAP_NUMBER,
    LAP_PITTED,
    LAP_TIME
};

void ReadPdf (const char* pdfFile)
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
                std::cout << "\"lapNotes\" : \"" << varStr << "\", ";
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
>>>>>>> 2c1517a22e76563223f75ad2efde25064de2933d

#include "LaptimesToJson.h"
#include "JsonToDrivers.h"
/*
 * 
 */
int main (int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "Hello Brian!" << std::endl;
    return -1;
  }
  
  char *inputFile = argv[1];
  if (strstr (inputFile, ".pdf"))
  {
    try {
      LaptimesToJson::convertPdfToJson (inputFile);
    } catch (const PoDoFo::PdfError &eCode) {
      eCode.PrintErrorMsg ();
      return eCode.GetError ();
    }
  }
  else if (strstr (inputFile, ".json"))
  {
    std::cout << "Json file" << std::endl;
    std::vector<Driver> driverVector;
    driverVector = JsonToDrivers::convertToDriverObjects (inputFile);
    std::cout << std::endl;
    for (std::vector<Driver>::iterator it = driverVector.begin (); it != driverVector.end (); ++it)
    {
      std::cout << (*it).toString () << std::endl;
    }
  }
  else
  {
    std::cout << "Unrecognised file type" << std::endl;
  }
  
  //const char *pdfFile = argv[1];
  return 0;
}

