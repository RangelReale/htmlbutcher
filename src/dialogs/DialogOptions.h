//=============================================================================
/**
 *  @file    DialogOptions.h
 *
 *  $Id: DialogOptions.h,v 1.1 2008/02/15 13:42:08 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-15
 */
//=============================================================================
#ifndef __DIALOGOPTIONS_H__
#define __DIALOGOPTIONS_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "BUtil.h"
#include <cppcomp/wxccucontrol.h>

using namespace cppcomp;

class HTMLButcherOptionsDialog: public wxDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherOptionsDialog( wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = _("Preferences"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherOptions *options);
    void Save(ButcherOptions *options);
protected:
    wxccu_Control_ItemMap<ButcherOptions::editmode_t> editmodeimap_;

    enum {
        ID_FIRST = 1000,
        ID_EDITMODE
    };

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    ButcherOptions options_;
};

#endif //__DIALOGOPTIONS_H__
