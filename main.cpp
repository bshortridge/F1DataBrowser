/* 
 * File:   main.cpp
 * Author: brian
 *
 * Created on 29 March 2018, 21:54
 */
#include <iostream>
#include <podofo/podofo.h>
#include <cstdlib>
#include "Driver.h"

void ReadPdf (const char* pdfFile)
{
  using namespace PoDoFo;
  int lapNo = 0;

  PdfMemDocument pdf (pdfFile);
  for (int pn = 0; pn < pdf.GetPageCount (); ++pn)
  {
    std::cout << "Page " << pn + 1 << std::endl;
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
          // process token: it contains the current command
          // pop from var stack as necessary
//          std::cout << "<" << token << "> ";
          break;
        case ePdfContentsType_Variant:
          // process var: push it onto a stack
          if (var.IsString ())
          {
            std::string varStr (var.GetString ().GetString ());
            int varInt = atoi (varStr.c_str());
            std::cout << "[" << varInt << "] ";
          }
          break;
        default:
          // should not happen!
          break;
      }
    }
    std::cout << std::endl;
  }
}

/*
 * 
 */
int main (int argc, char** argv)
{
  std::cout << "Hello Brian!" << std::endl;
  const char *pdfFile = "../../../data/2018_01_aus_f1_r0_timing_racelapanalysis_v01.pdf";
  try {
    ReadPdf (pdfFile);
  } catch (const PoDoFo::PdfError &eCode) {
    eCode.PrintErrorMsg ();
    return eCode.GetError ();
  }
  return 0;
}

