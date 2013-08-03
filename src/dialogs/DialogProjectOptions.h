//=============================================================================
/**
 *  @file    DialogProjectOptions.h
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-10-22
 */
//=============================================================================
#ifndef __DIALOGPROJECTOPTIONS_H__
#define __DIALOGPROJECTOPTIONS_H__

#include <map>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <cppcomp/wxccucontrol.h>
#include "BControls.h"

using namespace std;

class HTMLButcherProjectOptionsDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherProjectOptionsDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Project Options"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();
    void Load();
    void Save();
protected:
	wxccu_Control_ItemMap<ButcherProjectConsts::layouttype_t> layouttypeimap_;

	enum {
        ID_LAYOUTTYPE = 1000,
    };

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
	ButcherProjectConsts::layouttype_t layouttype_;
};

#endif //__DIALOGPROJECTOPTIONS_H__
