//=============================================================================
/**
 *  @file    DialogMaskAreaMapEdit.h
 *
 *  $Id: DialogMaskAreaMapEdit.h,v 1.7 2008/02/06 16:40:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-08
 */
//=============================================================================
#ifndef __DIALOGMASKAERAMAPEDIT_H__
#define __DIALOGMASKAREAMAPEDIT_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

class HTMLButcherMaskAreaMapEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMaskAreaMapEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Area"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );
    virtual ~HTMLButcherMaskAreaMapEditDialog();

    void Init();
    void CreateControls();

    void Load(ButcherProjectArea *loadarea, ButcherProjectAreaConfigBase *loadconfig);
    void Save(ButcherProjectArea *loadarea, ButcherProjectAreaConfigBase *loadconfig);

    void SetPreview(ButcherImage *preview) { previewctrl_->SetImage(preview); }

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    ButcherImageView *previewctrl_;

    void OnImageFormat(wxCommandEvent &event);

    enum {
        ID_IMAGEURL = 1000,
        ID_IMAGETAGAPPEND,
        ID_IMAGEURLTAGAPPEND,
    };
private:
    wxString imageurl_, imagetagappend_, imageurltagappend_;
};

#endif //__DIALOGMASKAERAEDIT_H__

