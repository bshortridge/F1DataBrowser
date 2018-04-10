/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyFrame.h
 * Author: coherense-dev
 *
 * Created on 10 April 2018, 15:44
 */

#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wxprec.h>
// For compilers that support precompilation, includes "wx/wx.h".
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MyFrame: public wxFrame
{
public:
    MyFrame (const wxString& title, const wxPoint& pos, const wxSize& size);
    
private:
    void OnHello (wxCommandEvent& event);
    void OnExit (wxCommandEvent& event);
    void OnAbout (wxCommandEvent& event);
    
    wxDECLARE_EVENT_TABLE ();
};

#endif /* MYFRAME_H */

