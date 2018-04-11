/* 
 * File:   MainFrame.cpp
 * Author: Brian Shortridge
 * 
 * Created on 11 April 2018, 11:33
 */

#include "MainFrame.h"

#include <podofo/podofo.h>
#include "LaptimesToJson.h"

enum {
  TEXT_Main = 1,
  CTRL_Book
};

wxBEGIN_EVENT_TABLE (MainFrame, wxFrame)
  EVT_MENU (wxID_OPEN, MainFrame::LoadFile)
  EVT_MENU (wxID_CLOSE, MainFrame::CloseFile)
  EVT_MENU (wxID_EXIT, MainFrame::Quit)
wxEND_EVENT_TABLE ()

MainFrame::MainFrame (const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame ((wxFrame*) NULL, -1, title, pos, size)
{
  CreateStatusBar (2);
  
  MainMenu = new wxMenuBar ();
  
  wxMenu *FileMenu = new wxMenu ();
  FileMenu->Append (wxID_OPEN, _("&Load File\tCtrl-L"),
                    _("Load pdf or json file for processing"));
  FileMenu->Append (wxID_CLOSE, _("&Close File\tCtrl-W"),
                    _("Close current file"));

  FileMenu->AppendSeparator ();
  FileMenu->Append (wxID_EXIT, _("&Quit\tCtrl-Q"), _("Exit the program"));
  
  MainMenu->Append (FileMenu, _("&File"));
  SetMenuBar (MainMenu);
  
  book = new wxBookCtrl (this, CTRL_Book);
//  wxPanel *panel = new wxPanel (book);
//  wxBoxSizer *mysizer = new wxBoxSizer (wxVERTICAL);
//  panel->SetSizer (mysizer);
//  MainEditBox = new wxTextCtrl (panel, TEXT_Main, _(""),
//                                wxDefaultPosition, wxDefaultSize, 
//                                wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH,
//                                wxDefaultValidator, wxTextCtrlNameStr);
//  mysizer->Add (MainEditBox, 1, wxEXPAND | wxALL, 5);
//  book->AddPage (panel, _T("JSON"), false);
  
  LastDirectory = wxEmptyString;
}

void MainFrame::LoadFile (wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog *OpenDialog = new wxFileDialog (this, _("Choose a file to open"),
    LastDirectory, wxEmptyString,
    _("Pdf / JSON files (*.pdf, *.json)|*.pdf;*.json|Pdf files (*.pdf)|*.pdf"
      "|JSON files (*.json)|*.json"),
    wxFD_OPEN, wxDefaultPosition);
  
  if (OpenDialog->ShowModal () == wxID_OK)
  {
    std::string inputFile;
    CurrentDocPath = OpenDialog->GetPath ();
    LastDirectory = OpenDialog->GetDirectory ();
    
    if (CurrentDocPath.rfind (".pdf") != std::string::npos)
    { // pdf entered - extract data to json
      try {
        inputFile = LaptimesToJson::convertPdfToJson (CurrentDocPath.ToStdString ());
        CurrentDocPath = inputFile;
        OpenDialog->SetPath (CurrentDocPath);
      } catch (const PoDoFo::PdfError &eCode) {
        eCode.PrintErrorMsg ();
        return;
      }
    }
    
    if (CurrentDocPath.rfind (".json") != std::string::npos)
    {
      //book = new wxBookCtrl (mainWindow, CTRL_Book);
      wxPanel *panel = new wxPanel (book);
      wxBoxSizer *mysizer = new wxBoxSizer (wxVERTICAL);
      panel->SetSizer (mysizer);
      MainEditBox = new wxTextCtrl (panel, TEXT_Main, _(""),
                                    wxDefaultPosition, wxDefaultSize, 
                                    wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH,
                                    wxDefaultValidator, wxTextCtrlNameStr);
      mysizer->Add (MainEditBox, 1, wxEXPAND | wxALL, 5);
      book->AddPage (panel, _T("JSON"), false);
      
      MainEditBox->LoadFile (CurrentDocPath);
      SetTitle (OpenDialog->GetFilename ());
    }
  }
}

void MainFrame::CloseFile (wxCommandEvent& event)
{
  MainEditBox->Clear ();
  SetTitle (_(""));
}

void MainFrame::Quit (wxCommandEvent& WXUNUSED(event))
{
  Close (true);
}
