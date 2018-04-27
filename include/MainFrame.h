/* 
 * File:   MainFrame.h
 * Author: Brian Shortridge
 *
 * Created on 11 April 2018, 11:33
 */

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/bookctrl.h>
#include "Driver.h"

class Gnuplot;

class MainFrame : public wxFrame
{
public:
  MainFrame (const wxString &title, const wxPoint &pos, const wxSize &size);
  ~MainFrame ();
  
private:
  void LoadFile   (wxCommandEvent& event);
  void CloseFile  (wxCommandEvent& event);
  void Quit       (wxCommandEvent& event);
  void handleCheckBox (wxCommandEvent& event);
  
  void drawGraph ();
  
  wxMenuBar     *m_MainMenu;
  wxBookCtrl    *m_Book;
  wxTextCtrl    *m_JSONdisplayBox;
  wxPanel       *m_GraphControlPanel;
  Gnuplot       *g1;
  
  wxString    m_LastDirectory;
  wxString    m_CurrentDocPath;
  
  std::vector<Driver> m_driverVector;
  
  wxDECLARE_EVENT_TABLE ();
};

#endif /* MAINFRAME_H */

