//=============================================================================
/**
 *  @file    DialogAssortedFileEdit.h
 *
 *  $Id: DialogAssortedFileEdit.h,v 1.6 2008/02/06 16:40:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-24
 */
//=============================================================================
#ifndef __DIALOGASSORTEDFILEEDIT_H__
#define __DIALOGASSORTEDFILEEDIT_H__

#include <map>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "BList.h"
#include "BControls.h"

using namespace std;

class HTMLButcherAssortedFileEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherAssortedFileEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Assorted File"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherProjectAssortedFile *afile);
    void Save(ButcherProjectAssortedFile *afile);
protected:
    typedef map< ButcherProjectAssortedFile::filetype_t, int > DialogFileTypeMap;

    enum {
        ID_FILE = 1000,
        ID_FILETYPE,
        ID_FILEPATH,
        ID_SAVEFILENAME,
        ID_GROUPS,
    };

    DialogFileTypeMap filetypemap_;

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    ButcherProject *project_;

    wxString filename_;
    ButcherIDList groups_;
    ButcherProjectAssortedFile::filetype_t filetype_;
    BLID_t filepath_;
    wxString savefilename_;
};

#endif //__DIALOGASSORTEDFILEEDIT_H__
