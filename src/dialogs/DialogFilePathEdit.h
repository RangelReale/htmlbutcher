//=============================================================================
/**
 *  @file    DialogFilePathEdit.h
 *
 *  $Id: DialogFilePathEdit.h,v 1.3 2008/02/06 16:40:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-24
 */
//=============================================================================
#ifndef __DIALOGFILEPATHEDIT_H_
#define __DIALOGFILEPATHEDIT_H_

#include <map>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "BControls.h"

using namespace std;

class HTMLButcherFilePathEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherFilePathEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("File Path"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherProjectFilePath *filepath);
    void Save(ButcherProjectFilePath *filepath);

    const wxString &GetFilePathName() { return name_; }
protected:
    enum {
        ID_NAME = 1000,
        ID_PATH,
    };

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    wxString name_, path_;
};

#endif //__DIALOGFILEPATHEDIT_H_
