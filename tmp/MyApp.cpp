/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyApp.cpp
 * Author: coherense-dev
 * 
 * Created on 10 April 2018, 15:42
 */

#include "MyApp.h"
#include "MyFrame.h"

bool MyApp::OnInit ()
{
    MyFrame *frame = new MyFrame ("Hello World", wxPoint (50, 50), wxSize (450, 340));
    frame->Show (true);
    return true;
}

