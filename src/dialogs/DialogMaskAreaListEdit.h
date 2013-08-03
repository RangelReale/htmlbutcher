//=============================================================================
/**
 *  @file    DialogMaskListEdit.h
 *
 *  $Id: DialogMaskAreaListEdit.h,v 1.1 2008/02/10 16:26:48 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-10
 */
//=============================================================================
#ifndef __DIALOGMASKAREALISTEDIT_H__
#define __DIALOGMASKAREALISTEDIT_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/treectrl.h>

#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

class HTMLButcherMaskAreaListDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMaskAreaListDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Areas"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherProjectView *view);
    void Save(ButcherProjectView *view);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    ButcherControl_ProjectImageView *previewctrl_;

    void OnAfterDraw(ButcherImageViewDrawEvent &event);
    void OnAreaChanged(wxTreeEvent &event);
    void OnAreaMenu(wxMouseEvent &event);

    enum {
        ID_FIRST = wxID_HIGHEST,
        ID_PREVIEW,
        ID_AREALIST,
    };
private:
    void AddSubArea(wxTreeCtrl *tree, wxTreeItemId parentitem,
        ButcherProjectMaskContainer *container, bool isglobal = false);
    wxString AreaDescription(ButcherProjectArea *area);

    ButcherProjectView *view_;
};

#endif //__DIALOGMASKAREALISTEDIT_H__

