/* 
 * File:   F1App.cpp
 * Author: Brian Shortridge
 * 
 * Created on 11 April 2018, 11:25
 */

#include "F1App.h"
#include "MainFrame.h"

bool F1App::OnInit ()
{
  MainFrame *MainWin = new MainFrame (_("F1 Data Analyser"), wxPoint (1, 1), 
                                      wxSize (700, 900));
  MainWin->Show (true);
  SetTopWindow (MainWin);
  return true;
}