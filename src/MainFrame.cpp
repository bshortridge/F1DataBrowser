/* 
 * File:   MainFrame.cpp
 * Author: Brian Shortridge
 * 
 * Created on 11 April 2018, 11:33
 */

#include "MainFrame.h"

#include <podofo/podofo.h>
#include "LaptimesToJson.h"

#include <wx/xy/xyplot.h>
#include <wx/xy/xysimpledataset.h>
#include <wx/xy/xylinerenderer.h>

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
  // serie xy data
  double data[][2] = {
      { 10, 20, },
      { 13, 16, },
      { 7, 30, },
      { 15, 34, },
      { 25, 4, },
  };

  // first step: create plot
  XYPlot *plot = new XYPlot();

  // create dataset
  XYSimpleDataset *dataset = new XYSimpleDataset();

  // and add serie to it
  dataset->AddSerie((double *) data, WXSIZEOF(data));

  // set line renderer to dataset
  dataset->SetRenderer(new XYLineRenderer());

  // create left and bottom number axes
  NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
  NumberAxis *bottomAxis = new NumberAxis(AXIS_BOTTOM);

  // optional: set axis titles
  leftAxis->SetTitle(wxT("X"));
  bottomAxis->SetTitle(wxT("Y"));

  // add axes and dataset to plot
  plot->AddObjects(dataset, leftAxis, bottomAxis);

  // and finally create chart
  Chart *chart = new Chart(plot, _T("Simple XY demo"));
  
  CreateStatusBar ();
  
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
  wxPanel *panel = new wxPanel (book);
  wxBoxSizer *mysizer = new wxBoxSizer (wxVERTICAL);
  panel->SetSizer (mysizer);
  JSONdisplayBox = new wxTextCtrl (panel, TEXT_Main, _(""),
                                wxDefaultPosition, wxDefaultSize, 
                                wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH,
                                wxDefaultValidator, wxTextCtrlNameStr);
  mysizer->Add (JSONdisplayBox, 1, wxEXPAND | wxALL, 5);
  book->AddPage (panel, _T("JSON"), false);
  
  panel = new wxPanel (book);
  wxChartPanel *chartPanel = new wxChartPanel (panel, wxID_ANY, chart, wxPoint (0, 0), wxSize (1, 1));
  mysizer = new wxBoxSizer (wxVERTICAL);
  panel->SetSizer (mysizer);
  mysizer->Clear();
  mysizer->Add (chartPanel, 1, wxGROW | wxALL, 5);
  mysizer->Layout ();
//  Chart *chart = new Chart (plot, GetName ());
//  wxChartPanel *m_chartPanel = new wxChartPanel (panel, wxID_ANY, chart);
//  m_chartPanel->SetChart (chart);
  book->AddPage (panel, _T("Tab1"), true);
  
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
      JSONdisplayBox->LoadFile (CurrentDocPath);
      SetStatusText (OpenDialog->GetFilename ());
    }
  }
}

void MainFrame::CloseFile (wxCommandEvent& event)
{
  JSONdisplayBox->Clear ();
  SetStatusText (_(""));
}

void MainFrame::Quit (wxCommandEvent& WXUNUSED(event))
{
  Close (true);
}
