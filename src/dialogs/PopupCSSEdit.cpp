
//=============================================================================
/**
 *  @file    PopupCSSEdit.cpp
 *
 *  $Id: PopupCSSEdit.cpp,v 1.3 2008/02/06 16:40:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-03
 */
//=============================================================================
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/filepicker.h>
#include <wx/checklst.h>
#include <wx/statline.h>

#include "PopupCSSEdit.h"
#include "BList.h"
#include "BControls.h"





BEGIN_EVENT_TABLE(HTMLButcherPopupCSSEditDialog, ButcherControl_Dialog)
END_EVENT_TABLE()

HTMLButcherPopupCSSEditDialog::HTMLButcherPopupCSSEditDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style)
{
    Init();
    CreateControls();
}

void HTMLButcherPopupCSSEditDialog::Init()
{
    cssfileid_=0;
}

void HTMLButcherPopupCSSEditDialog::SetCSSFileId(BLID_t cssfileid)
{
    cssfileid_=cssfileid;
}

void HTMLButcherPopupCSSEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);

    // Name
    boxsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl = new wxTextCtrl(this, ID_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    namectrl->Enable(false);
    boxsizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // CSS Text
    boxsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&CSS Text:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    ButcherControl_FmtTextCtrl *csstextctrl = new ButcherControl_FmtTextCtrl(this, ID_CSSTEXT, wxDefaultPosition, wxSize(450, 300));
    csstextctrl->SetEditFormat(ButcherControl_FmtTextCtrl::FMT_CSS);
    boxsizer->Add(csstextctrl, 1, wxGROW|wxALL, 3);


    // divider line
    wxStaticLine *line2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line2, 0, wxGROW|wxALL|wxALIGN_BOTTOM, 3);

    // BUTTONS
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // cancel button
    wxButton* cancel = new wxButton ( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // validators

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



bool HTMLButcherPopupCSSEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxTextCtrl *namectrl=(wxTextCtrl*)FindWindow(ID_NAME);
    namectrl->SetValue(GetProject()->CSSFiles().Get(cssfileid_)->GetName());

    ButcherControl_FmtTextCtrl *csstextctrl=(ButcherControl_FmtTextCtrl*)FindWindow(ID_CSSTEXT);
    csstextctrl->SetValue(GetProject()->CSSFiles().Get(cssfileid_)->GetCSSText());

    return true;
}

bool HTMLButcherPopupCSSEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    ButcherControl_FmtTextCtrl *csstextctrl=(ButcherControl_FmtTextCtrl*)FindWindow(ID_CSSTEXT);
    GetProject()->CSSFiles().Get(cssfileid_)->SetCSSText(csstextctrl->GetValue());

    return true;
}



