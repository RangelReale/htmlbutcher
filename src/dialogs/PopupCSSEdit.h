//=============================================================================
/**
 *  @file    PopupCSSEdit.h
 *
 *  $Id: PopupCSSEdit.h,v 1.3 2008/02/06 16:40:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-03
 */
//=============================================================================
#ifndef __POPUPCSSEDIT_H__
#define __POPUPCSSEDIT_H__

#include <map>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <cppcomp/wxccucontrol.h>
#include "BList.h"
#include "BControls.h"

using namespace std;
using namespace cppcomp;

class HTMLButcherPopupCSSEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherPopupCSSEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Edit CSS"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void SetCSSFileId(BLID_t cssfileid);
protected:
    enum {
        ID_NAME = 1000,
        ID_CSSTEXT
    };

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    BLID_t cssfileid_;
};

#endif //__POPUPCSSEDIT_H__
