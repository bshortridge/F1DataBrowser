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

class MainFrame : public wxFrame
{
public:
  MainFrame (const wxString &title, const wxPoint &pos, const wxSize &size);
  
private:
  void LoadFile   (wxCommandEvent& event);
  void CloseFile  (wxCommandEvent& event);
  void Quit       (wxCommandEvent& event);
  
  wxBookCtrl  *book;
  wxTextCtrl  *JSONdisplayBox;
  wxMenuBar   *MainMenu;
  
  wxString    LastDirectory;
  wxString    CurrentDocPath;
  
  wxDECLARE_EVENT_TABLE ();
};

#endif /* MAINFRAME_H */

