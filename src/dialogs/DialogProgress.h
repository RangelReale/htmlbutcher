//=============================================================================
/**
 *  @file    DialogProgress.h
 *
 *  $Id: DialogProgress.h,v 1.5 2008/02/05 17:35:50 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-22
 */
//=============================================================================
#ifndef __DIALOGPROGRESS_H__
#define __DIALOGPROGRESS_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>

#include "ButcherProject.h"

#include <wx/gauge.h>

using namespace std;

/**
 * @class HTMLButcherProgressDialog
 *
 * @brief List edit base dialog
 */
class HTMLButcherProgressDialog: public wxFrame
{
    DECLARE_CLASS( HTMLButcherProgressDialog )
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherProgressDialog();
    HTMLButcherProgressDialog( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = _("Progress"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxFRAME_TOOL_WINDOW );
    virtual ~HTMLButcherProgressDialog() { }

    void Init();

    bool Create( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = _("Progress"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxFRAME_TOOL_WINDOW );

    void CreateControls();

    void UpdateProgress(ButcherProject *project);
private:
    wxStaticText *lstep_, *ldesc_;
    wxGauge *gauge_;
    wxWindowDisabler windisable_;
};

#endif //__DIALOGPROGRESS_H__
