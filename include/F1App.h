/* 
 * File:   F1App.h
 * Author: Brian Shortridge
 *
 * Created on 11 April 2018, 11:25
 */

#ifndef F1APP_H
#define F1APP_H
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class F1App : public wxApp
{
public:
  virtual bool OnInit ();
};

#endif /* F1APP_H */

