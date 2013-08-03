//=============================================================================
/**
 *  @file    DialogCSSFileEdit.h
 *
 *  $Id: DialogCSSFileEdit.h,v 1.4 2008/02/07 00:42:50 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-24
 */
//=============================================================================
#ifndef __DIALOGCSSFILEEDIT_H__
#define __DIALOGCSSFILEEDIT_H__

#include <map>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <cppcomp/wxccucontrol.h>
#include "BList.h"
#include "BControls.h"

using namespace std;
using namespace cppcomp;

class HTMLButcherCSSFileEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherCSSFileEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("CSS File"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    const wxString &GetCSSName() { return name_; }

    void Load(ButcherProjectCSSFile *afile);
    void Save(ButcherProjectCSSFile *afile);
protected:
    enum {
        ID_NAME = 1000,
        ID_CSSTEXT,
        ID_FILEPATH,
        ID_SAVEFILENAME
    };

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    ButcherProject *project_;

    wxString name_, csstext_;
    BLID_t filepathid_;
    wxString savefilename_;
};

#endif //__DIALOGCSSFILEEDIT_H__
