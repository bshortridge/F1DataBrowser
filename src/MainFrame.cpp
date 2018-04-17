/* 
 * File:   MainFrame.cpp
 * Author: Brian Shortridge
 * 
 * Created on 11 April 2018, 11:33
 */

#include "MainFrame.h"

#include <podofo/podofo.h>
#include "Driver.h"
#include "LaptimesToJson.h"
#include "JsonToDrivers.h"

enum {
  TEXT_Main = 1,
  CTRL_Book,
  ID_Button1,
  ID_Button2
};

wxBEGIN_EVENT_TABLE (MainFrame, wxFrame)
  EVT_MENU (wxID_OPEN, MainFrame::LoadFile)
  EVT_MENU (wxID_CLOSE, MainFrame::CloseFile)
  EVT_MENU (wxID_EXIT, MainFrame::Quit)
wxEND_EVENT_TABLE ()

MainFrame::MainFrame (const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame ((wxFrame*) NULL, -1, title, pos, size)
{
  CreateStatusBar ();

  m_MainMenu = new wxMenuBar ();
  wxMenu *FileMenu = new wxMenu ();
  FileMenu->Append (wxID_OPEN, _("&Load File\tCtrl-L"),
                    _("Load pdf or json file for processing"));
  FileMenu->Append (wxID_CLOSE, _("&Close File\tCtrl-W"),
                    _("Close current file"));
  FileMenu->AppendSeparator ();
  FileMenu->Append (wxID_EXIT, _("&Quit\tCtrl-Q"), _("Exit the program"));
  m_MainMenu->Append (FileMenu, _("&File"));
  SetMenuBar (m_MainMenu);
  
  m_Book = new wxBookCtrl (this, CTRL_Book);
  wxPanel *panel = new wxPanel (m_Book);
  wxBoxSizer *mysizer = new wxBoxSizer (wxVERTICAL);
  panel->SetSizer (mysizer);
  m_JSONdisplayBox = new wxTextCtrl (panel, TEXT_Main, _(""),
                                      wxDefaultPosition, wxDefaultSize, 
                                      wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH,
                                      wxDefaultValidator, wxTextCtrlNameStr);
  mysizer->Add (m_JSONdisplayBox, 1, wxEXPAND | wxALL, 5);
  m_Book->AddPage (panel, _T("JSON"), false);
  
  m_GraphControlPanel = new wxPanel (m_Book);
  wxGridSizer *gs = new wxGridSizer (5, 4, 3, 3);
  gs->Add (new wxCheckBox (m_GraphControlPanel, -1, _T("Kimi")));
  gs->Add (new wxCheckBox (m_GraphControlPanel, -1, _T("Vettel")));
  gs->Add (new wxCheckBox (m_GraphControlPanel, -1, _T("Kimi")));
  gs->Add (new wxCheckBox (m_GraphControlPanel, -1, _T("Vettel")));
  gs->Add (new wxCheckBox (m_GraphControlPanel, -1, _T("Kimi")));
  gs->Add (new wxCheckBox (m_GraphControlPanel, -1, _T("Vettel")));
      m_GraphControlPanel->SetSizer (gs);
  m_Book->AddPage (m_GraphControlPanel, _T("Graph Control"), true);
  
  g1 = new Gnuplot ("linespoints");
  g1->cmd ("set ydata time");
  g1->cmd ("set timefmt \"%M:%S\"");
  g1->cmd ("set format y \"%M:%.3S\"");
  g1->set_xlabel ("Lap");
  g1->set_ylabel ("Time");
  g1->set_grid();
  
  m_LastDirectory = wxEmptyString;
}

void MainFrame::LoadFile (wxCommandEvent& WXUNUSED(event))
{
  wxFileDialog *OpenDialog = new wxFileDialog (this, _("Choose a file to open"),
                              m_LastDirectory, wxEmptyString,
                              _("Pdf / JSON files (*.pdf, *.json)|*.pdf;*.json"
                                "|Pdf files (*.pdf)|*.pdf"
                                "|JSON files (*.json)|*.json"),
                              wxFD_OPEN, wxDefaultPosition);
  
  if (OpenDialog->ShowModal () == wxID_OK)
  {
    m_CurrentDocPath = OpenDialog->GetPath ();
    m_LastDirectory = OpenDialog->GetDirectory ();
    
    if (m_CurrentDocPath.rfind (".pdf") != std::string::npos)
    { // pdf entered - extract data to json
      try {
        m_CurrentDocPath = LaptimesToJson::convertPdfToJson (m_CurrentDocPath.ToStdString ());
        OpenDialog->SetPath (m_CurrentDocPath);
      } catch (const PoDoFo::PdfError &eCode) {
        eCode.PrintErrorMsg ();
        return;
      }
    }
    
    if (m_CurrentDocPath.rfind (".json") != std::string::npos)
    {
      m_JSONdisplayBox->LoadFile (m_CurrentDocPath);
      SetStatusText (OpenDialog->GetFilename ());
      
      // We have the data, now we can play
      wxGridSizer *gs = new wxGridSizer (5, 4, 3, 3);

      std::vector<Driver> driverVector;
      driverVector = JsonToDrivers::convertToDriverObjects (m_CurrentDocPath.ToStdString ());
      std::map<int, std::string> lapMap;
      for (std::vector<Driver>::iterator it = driverVector.begin (); it != driverVector.end (); ++it)
      {
        gs->Add (new wxCheckBox (m_GraphControlPanel, -1, (*it).name ()));
        std::cout << (*it).stintAnalysis () << std::endl;
        lapMap = (*it).lapMap ();            
        std::vector<int> x;
        std::vector<std::string> y;
        for (std::map<int,std::string>::iterator lm = lapMap.begin ();
              lm != lapMap.end (); ++lm)
        {
          x.push_back (lm->first);
          y.push_back (lm->second);
        }
        if (x.size ())
        {
          std::ostringstream driverLabel;
          driverLabel << (*it).number () << " " << (*it).name ();
          g1->plot_xy (x, y, driverLabel.str ());
        }
      }
      m_GraphControlPanel->SetSizer (gs);
    }
  }
}

void MainFrame::CloseFile (wxCommandEvent& event)
{
  m_JSONdisplayBox->Clear ();
  SetStatusText (_(""));
}

void MainFrame::Quit (wxCommandEvent& WXUNUSED(event))
{
  Close (true);
}
