//=============================================================================
/**
 *  @file    DialogFileEdit.h
 *
 *  $Id: DialogFileEdit.h,v 1.4 2008/02/06 16:40:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-24
 */
//=============================================================================
#ifndef __DIALOGFILEEDIT_H__
#define __DIALOGFILEEDIT_H__

#include <map>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "BList.h"
#include "DialogListEdit.h"
#include "BImageView.h"
#include "BControls.h"

using namespace std;

/**
 * @class HTMLButcherFilesDialog
 *
 * @brief Files dialog
 */
class HTMLButcherFilesDialog: public HTMLButcherListEditDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherFilesDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Files"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX ):
        HTMLButcherListEditDialog(parent, id, project, caption, pos, size, style)
            { CreatePreviewControls(); }

    void CreatePreviewControls();
protected:
    enum {
        ID_AUTOUPDATE = ID_DIALOGLISTMAX+1
    };

    virtual void do_select(BLID_t id);
    virtual void do_load();

    virtual void do_add();
    virtual void do_edit(unsigned long id);
    virtual void do_remove(unsigned long id);

    void UpdatePreview();
    void UpdatePreview(BLID_t id);

    void OnAutoUpdate(wxCommandEvent &event);
private:
    wxStaticText *previewlabel_;
    ButcherImageView *previewctrl_;
    wxSizer *previewsizer_;

    wxString make_description(ButcherProjectFile *item);
};


/**
 * @class HTMLButcherFileEditDialog
 *
 * @brief File edit dialog
 */
class HTMLButcherFileEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherFileEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("File"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherProjectFile *file);
    void Save(ButcherProjectFile *file);
protected:
    enum {
        ID_NAME = 1000,
        ID_FILE
    };

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    ButcherProject *project_;

    wxString name_, filename_;
};

#endif //__DIALOGFILEEDIT_H__
