//=============================================================================
/**
 *  @file    DialogMaskEdit.h
 *
 *  $Id: DialogMaskEdit.h,v 1.11 2008/02/07 12:35:34 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __DIALOGMASKEDIT_H__
#define __DIALOGMASKEDIT_H__

#include <map>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "BControls.h"

using namespace std;

class HTMLButcherMaskEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMaskEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Mask"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    const wxString &GetMaskName() { return maskname_; }
    void SetMaskName(const wxString &maskname) { maskname_=maskname; }

    unsigned int GetMaskWidth() { return maskwidth_; }
    void SetMaskWidth(unsigned int maskwidth) { maskwidth_=maskwidth; }

    unsigned int GetMaskHeight() { return maskheight_; }
    void SetMaskHeight(unsigned int maskheight) { maskheight_=maskheight; }

    void Load(ButcherProjectMask *mask);
    void Save(ButcherProjectMask *mask);

    int GetMarginLeft() { return marginleft_; }
    int GetMarginTop() { return margintop_; }
    int GetMarginRight() { return marginright_; }
    int GetMarginBottom() { return marginbottom_; }
    void SetMarginLeft(int marginleft) { marginleft_=marginleft; }
    void SetMarginTop(int margintop) { margintop_=margintop; }
    void SetMarginRight(int marginright) { marginright_=marginright; }
    void SetMarginBottom(int marginbottom) { marginbottom_=marginbottom; }
protected:
    enum {
        ID_NAME = 1000,
        ID_WIDTH,
        ID_HEIGHT,
        ID_MARGINLEFT,
        ID_MARGINTOP,
        ID_MARGINRIGHT,
        ID_MARGINBOTTOM,
    };

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    wxString maskname_;
    unsigned int maskwidth_, maskheight_;
    int marginleft_, margintop_, marginright_, marginbottom_;
};

#endif //__DIALOGMASKEDIT_H__
