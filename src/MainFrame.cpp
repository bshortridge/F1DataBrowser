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
#include "gnuplot_i.hpp"

enum {
  TEXT_Main = 1,
  CTRL_Book,
  ID_Button1,
  ID_Button2,
  ID_Checkbox
};

wxBEGIN_EVENT_TABLE (MainFrame, wxFrame)
  EVT_MENU (wxID_OPEN, MainFrame::LoadFile)
  EVT_MENU (wxID_CLOSE, MainFrame::CloseFile)
  EVT_MENU (wxID_EXIT, MainFrame::Quit)
  EVT_CHECKBOX (ID_Checkbox, MainFrame::handleCheckBox)
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

MainFrame::~MainFrame ()
{
  std::cout << "MainFrame destructor called" << std::endl;
}

void MainFrame::drawGraph ()
{
  try
  {
    g1->reset_plot ();
    g1->remove_tmpfiles ();
  }
  catch (GnuplotException ge)
  {
    std::cout << ge.what() << std::endl;
  }
  std::map<int, std::string> lapMap;
  for (std::vector<Driver>::iterator it = m_driverVector.begin (); it != m_driverVector.end (); ++it)
  {
    if (false == (*it).showGraph ())
    {
      continue;
    }
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
      try
      {
        g1->plot_xy (x, y, driverLabel.str ());
        
      }
      catch (GnuplotException ge)
      {
        std::cout << ge.what() << std::endl;
      }

    }
  }
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
      m_driverVector = JsonToDrivers::convertToDriverObjects (m_CurrentDocPath.ToStdString ());

      wxGridSizer *gs = new wxGridSizer (5, 4, 3, 3);
      std::map<int, std::string> lapMap;
      for (std::vector<Driver>::iterator it = m_driverVector.begin (); it != m_driverVector.end (); ++it)
      {
        wxCheckBox *cb_driver = new wxCheckBox (m_GraphControlPanel, ID_Checkbox, (*it).name ());
        cb_driver->SetValue (true);
        (*it).showGraph (true);
        cb_driver->SetName (std::to_string ((*it).number ()));
        gs->Add ((wxControl *)cb_driver);
      }
      m_GraphControlPanel->Fit ();
      m_GraphControlPanel->SetSizer (gs);
      drawGraph ();
    }
    *g1 << "show plot";
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

void MainFrame::handleCheckBox (wxCommandEvent& event)
{
  wxCheckBox *cb = (wxCheckBox*)event.GetEventObject();
  unsigned int driverNumber = std::stoul (cb->GetName().ToStdString ());
  std::cout << driverNumber << " " << cb->GetLabel () << ": " << event.GetInt () << std::endl;

  for (std::vector<Driver>::iterator it = m_driverVector.begin (); it != m_driverVector.end (); ++it)
  {
    if ((*it).number () == driverNumber)
    {
      (*it).showGraph(cb->GetValue ());
      break;
    }
  }
  drawGraph ();
}
