//=============================================================================
/**
 *  @file    HTMLButcherApp.h
 *
 *  $Id: HTMLButcherApp.h,v 1.2 2007/12/02 16:36:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-11-29
 */
//=============================================================================

#ifndef HTMLBUTCHERAPP_H
#define HTMLBUTCHERAPP_H

#include "BConsts.h"
#include <wx/app.h>
#ifdef BUTCHER_USE_HELP
#include <wx/help.h>
#include <wx/cshelp.h>
#include <wx/html/helpctrl.h>
#endif

class HTMLButcherApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

    virtual void OnFatalException();
    virtual bool OnExceptionInMainLoop();
    virtual void OnUnhandledException();
    virtual int OnRun();

#if defined(__WXGTK__) || defined(__WXMAC__)
    virtual void HandleEvent(wxEvtHandler *handler,
                             wxEventFunction func,
                             wxEvent& event) const;
#endif
protected:
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
private:
	wxString licfile_;
    wxString openfile_;
	wxLocale locale_;
#ifdef BUTCHER_USE_HELP
#ifdef __WXMSW__
    wxCHMHelpController help_;
#else
    wxHtmlHelpController help_;
#endif
#endif
};

#endif // HTMLBUTCHERAPP_H
