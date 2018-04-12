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
              case (LAP_NOTES):
              {
                if (strlen (varStr) == 1)
                { // Currently we get a space or a P in notes
                  jsonSS << "\"lapNotes\" : \"" << varStr << "\", ";
                  state = LAP_TIME;
                  break;
                }
                else
                { // Oops, the previous was a driver number, not lap number
                  // This is a driver name, we need to edit and fix the jsonSS
                  // Save current json string
                  std::string tmpString (jsonSS.str());
                  // Extract driver number
                  std::size_t lastComma = tmpString.rfind (",");
                  std::size_t afterLastColonSpace = tmpString.rfind (": ") + 2;
                  std::string driverNumberString = 
                    tmpString.substr (afterLastColonSpace, lastComma - afterLastColonSpace);
                  // edit back to close off previous driver's laps
                  std::size_t afterLastBrace = tmpString.rfind ("}") + 1;
                  // Clear json string stream
                  jsonSS.str ("");
                  jsonSS.clear ();
                  // Copy the edited string back into the json string stream
                  jsonSS << tmpString.substr (0, afterLastBrace);
                  // Add driver number
                  jsonSS << "\n      ]";
                  jsonSS << "\n    },";
                  jsonSS << "\n    {";
                  jsonSS << "\n      \"driverNumber\" : "
                              << driverNumberString << ",\n";
                  LapNo = 0;
                  state = DRIVER_NAME;
                  // Fall through to add the driver name
                }
                // break; intentionally left out, fall through
              }
              case (DRIVER_NAME):
              {
                jsonSS << "      \"driverName\" : \"";
                if (!strcmp (varStr, "Lewis HAMILTON"))
                {
                  jsonSS << "Our Lewis";
                }
                else if (!strcmp (varStr, "Max VERSTAPPEN"))
                {
                  jsonSS << "The Dutchman";
                }
                else if (!strcmp (varStr, "Kevin MAGNUSSEN"))
                {
                  jsonSS << "The Viking";
                }
                else
                {
                  jsonSS << varStr;
                }
                jsonSS << "\",\n";
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
                    state = LAP_NOTES;
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
