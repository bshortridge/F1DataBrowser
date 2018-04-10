/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyApp.h
 * Author: coherense-dev
 *
 * Created on 10 April 2018, 15:42
 */

#ifndef MYAPP_H
#define MYAPP_H

#include <wx/wxprec.h>
// For compilers that support precompilation, includes "wx/wx.h".
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MyApp: public wxApp
{
public:
    virtual bool OnInit ();
};

#endif /* MYAPP_H */

