//=============================================================================
/**
 *  @file    DialogListEdit.h
 *
 *  $Id: DialogListEdit.h,v 1.16 2008/02/07 12:35:34 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __DIALOGLISTEDIT_H__
#define __DIALOGLISTEDIT_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "DialogMaskEdit.h"
#include "BControls.h"

/**
 * @class HTMLButcherListEditDialog
 *
 * @brief List edit base dialog
 */
class HTMLButcherListEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherListEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Edit"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );
    virtual ~HTMLButcherListEditDialog() { }

    void Init();
    void CreateControls();

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    enum {
        ID_ITEMS = 1000,
        ID_ADD,
        ID_EDIT,
        ID_REMOVE,
        ID_CLEAR,
        ID_DUPLICATE,
        ID_DIALOGLISTMAX
    };

    void item_add(const wxString &title, unsigned long id);
    void item_edit(unsigned long id, const wxString &title);
    void item_remove(unsigned long id);
    BLID_t item_current();

    virtual void do_load() {}

    virtual void do_select(BLID_t id) {}

    virtual void do_add() {}
    virtual void do_edit(unsigned long id) {}
    virtual void do_remove(unsigned long id) {}
    virtual void do_duplicate(unsigned long id) {}

    wxSizer *GetBodySizer() { return bodysizer_; }

    void EnableAdd(bool enable);
    void EnableDuplicate();
    void ControlsChanged();
private:
    void OnAdd(wxCommandEvent& event);
    void OnEdit(wxCommandEvent& event);
    void OnRemove(wxCommandEvent& event);
    void OnDuplicate(wxCommandEvent& event);
    void OnSelect(wxCommandEvent& event);

    wxSizer *topsizer_, *bodysizer_;
};

/**
 * @class HTMLButcherMasksDialog
 *
 * @brief Files dialog
 */
class HTMLButcherMasksDialog: public HTMLButcherListEditDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMasksDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Masks"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX ):
        HTMLButcherListEditDialog(parent, id, project, caption, pos, size, style) {}
protected:
    virtual void do_load();

    virtual void do_add();
    virtual void do_edit(unsigned long id);
    virtual void do_remove(unsigned long id);
    virtual void do_duplicate(unsigned long id);
private:
    wxString make_description(ButcherProjectMask *item);
};

/**
 * @class HTMLButcherViewsDialog
 *
 * @brief Views dialog
 */
class HTMLButcherViewsDialog: public HTMLButcherListEditDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherViewsDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Views"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX ):
        HTMLButcherListEditDialog(parent, id, project, caption, pos, size, style) {}
protected:
    virtual void do_load();

    virtual void do_add();
    virtual void do_edit(unsigned long id);
    virtual void do_remove(unsigned long id);
private:
    wxString make_description(ButcherProjectView *item);
};

/**
 * @class HTMLButcherAssortedFileGroupsDialog
 *
 * @brief Assorted file groups dialog
 */
class HTMLButcherAssortedFileGroupsDialog: public HTMLButcherListEditDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherAssortedFileGroupsDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Assorted File Groups"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX ):
        HTMLButcherListEditDialog(parent, id, project, caption, pos, size, style) {}
protected:
    virtual void do_load();

    virtual void do_add();
    virtual void do_edit(unsigned long id);
    virtual void do_remove(unsigned long id);
private:
    wxString make_description(ButcherProjectAssortedFileGroup *item);
};

/**
 * @class HTMLButcherAssortedFilesDialog
 *
 * @brief Assorted files dialog
 */
class HTMLButcherAssortedFilesDialog: public HTMLButcherListEditDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherAssortedFilesDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Assorted Files"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX ):
        HTMLButcherListEditDialog(parent, id, project, caption, pos, size, style) {}
protected:
    virtual void do_load();

    virtual void do_add();
    virtual void do_edit(unsigned long id);
    virtual void do_remove(unsigned long id);
private:
    wxString make_description(ButcherProjectAssortedFile *item);
};

/**
 * @class HTMLButcherFilePathsDialog
 *
 * @brief File paths dialog
 */
class HTMLButcherFilePathsDialog: public HTMLButcherListEditDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherFilePathsDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("File Paths"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX ):
        HTMLButcherListEditDialog(parent, id, project, caption, pos, size, style) {}
protected:
    virtual void do_load();

    virtual void do_add();
    virtual void do_edit(unsigned long id);
    virtual void do_remove(unsigned long id);
private:
    wxString make_description(ButcherProjectFilePath *item);
};

/**
 * @class HTMLButcherCSSFilesDialog
 *
 * @brief CSS files dialog
 */
class HTMLButcherCSSFilesDialog: public HTMLButcherListEditDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherCSSFilesDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("CSS Files"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX ):
        HTMLButcherListEditDialog(parent, id, project, caption, pos, size, style) {}
protected:
    virtual void do_load();

    virtual void do_add();
    virtual void do_edit(unsigned long id);
    virtual void do_remove(unsigned long id);
private:
    wxString make_description(ButcherProjectCSSFile *item);
};


#endif //__DIALOGLISTEDIT_H__
