//=============================================================================
/**
 *  @file    DialogMaskAreaSlice.h
 *
 *  $Id: DialogMaskAreaSlice.h,v 1.2 2008/02/09 12:27:41 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-08
 */
//=============================================================================
#ifndef __DIALOGMASKAREASLICE_H__
#define __DIALOGMASKAREASLICE_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

class HTMLButcherMaskAreaSliceDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMaskAreaSliceDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Slice Area"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherProjectView *view, ButcherProjectArea *loadarea, ButcherProjectAreaConfig *loadconfig);
    void Save(ButcherProjectView *view, ButcherProjectArea *loadarea, ButcherProjectAreaConfig *loadconfig);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    ButcherImageView *previewctrl_;

    enum {
        ID_FIRST = wxID_HIGHEST,
        ID_PREVIEW,
        ID_ROWS,
        ID_COLS,
    };
private:
    void OnAfterDraw(ButcherImageViewDrawEvent &event);
    void OnChangeSize(wxSpinEvent &event);

    ButcherProjectView *view_;
    ButcherProjectArea *area_;
    ButcherProjectAreaConfig *config_;
};

#endif //__DIALOGMASKAREASLICE_H__

