//=============================================================================
/**
 *  @file    DialogAdjustSize.h
 *
 *  $Id: DialogAdjustSize.h,v 1.7 2008/02/25 21:52:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-09
 */
//=============================================================================
#ifndef __DIALOGADJUSTSIZE_H__
#define __DIALOGADJUSTSIZE_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/spinctrl.h>

#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

class HTMLButcherAdjustSizeDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherAdjustSizeDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Adjust Size"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void SetArea(ButcherProjectFile *file, ButcherProjectArea *area) {
        file_=file; area_=area; line_=NULL;
    }
    void SetLine(ButcherProjectFile *file, ButcherProjectLine *line) {
        file_=file; area_=NULL; line_=line;
    }

    void RevertToDefault();
protected:
    enum {
        ID_LEFT=1000,
        ID_LEFTGLOBAL,
        ID_TOP,
        ID_TOPGLOBAL,
        ID_RIGHT,
        ID_RIGHTGLOBAL,
        ID_BOTTOM,
        ID_BOTTOMGLOBAL,
        ID_MOVELEFT,
        ID_MOVETOP,
        ID_WIDTH,
        ID_HEIGHT,

        ID_REVERT
    };

    void OnChangeLeft(wxSpinEvent &event);
    void OnChangeTop(wxSpinEvent &event);
    void OnChangeRight(wxSpinEvent &event);
    void OnChangeBottom(wxSpinEvent &event);
    void OnChangeMoveLeft(wxSpinEvent &event);
    void OnChangeMoveTop(wxSpinEvent &event);
    void OnChangeWidth(wxSpinEvent &event);
    void OnChangeHeight(wxSpinEvent &event);

    void OnRevert(wxCommandEvent &event);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    void UpdateDisplay(bool isfirst=false);
    void SetSpinRange(ButcherProjectLine *line, wxSpinCtrl *spinctrl, bool isend);
    void SetSpinRange(ButcherProjectLine *start, ButcherProjectLine *end, wxSpinCtrl *spinctrl);

    ButcherProjectLineMinMax MovementMinMax(ButcherProjectLine *start, ButcherProjectLine *end);

    ButcherImageView *previewctrl_;

    ButcherProjectFile *file_;
    ButcherProjectArea *area_;
    ButcherProjectLine *line_;
    wxRect defaultvalues_;
    bool updating_;
};

#endif //__DIALOGADJUSTSIZE_H__
