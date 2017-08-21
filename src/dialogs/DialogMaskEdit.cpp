//=============================================================================
/**
 *  @file    DialogMaskEdit.cpp
 *
 *  $Id: DialogMaskEdit.cpp,v 1.16 2008/02/16 12:52:50 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/valgen.h>

#include "DialogMaskEdit.h"
#include "BList.h"
#include "BUtil.h"



BEGIN_EVENT_TABLE(HTMLButcherMaskEditDialog, ButcherControl_Dialog)
END_EVENT_TABLE()

HTMLButcherMaskEditDialog::HTMLButcherMaskEditDialog( wxWindow* parent,
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

void HTMLButcherMaskEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-masks.html"));
#endif
	
	maskname_=wxEmptyString;
    maskwidth_=100;
    maskheight_=100;
    marginleft_=0;
    margintop_=0;
    marginright_=0;
    marginbottom_=0;
}

void HTMLButcherMaskEditDialog::Load(ButcherProjectMask *mask)
{
}

void HTMLButcherMaskEditDialog::Save(ButcherProjectMask *mask)
{
}



void HTMLButcherMaskEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALL|wxGROW, 3);

    // Name
    wxStaticText *namelabel = new wxStaticText(this, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(namelabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl = new wxTextCtrl(this, ID_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(namectrl, 0, wxGROW|wxALL, 3);


    // SIZE
    wxBoxSizer *sizesizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(sizesizer, 0, wxGROW|wxALL, 3);


    // width
    wxStaticText *widthlabel = new wxStaticText(this, wxID_STATIC, _("Width:"), wxDefaultPosition, wxDefaultSize, 0);
    sizesizer->Add(widthlabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *widthctrl = new wxSpinCtrl(this, ID_WIDTH, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 1, 10000, 1);
    sizesizer->Add(widthctrl, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // height
    wxStaticText *heightlabel = new wxStaticText(this, wxID_STATIC, _("Height:"), wxDefaultPosition, wxDefaultSize, 0);
    sizesizer->Add(heightlabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *heightctrl = new wxSpinCtrl(this, ID_HEIGHT, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 1, 10000, 1);
    sizesizer->Add(heightctrl, wxALIGN_CENTER_VERTICAL|wxALL, 3);


/*
    // MARGINS
    wxStaticBoxSizer *marginbox = new wxStaticBoxSizer(wxVERTICAL, this, _("Margins"));
    boxsizer->Add(marginbox, 0, wxGROW|wxALL, 3);

    wxFlexGridSizer *marginsizer = new wxFlexGridSizer(4, 4);
    marginbox->Add(marginsizer, 0, wxGROW|wxALL, 3);

    marginsizer->AddGrowableCol(1, 1);
    marginsizer->AddGrowableCol(3, 1);


    // margin left
    wxStaticText *mleftlabel = new wxStaticText(this, wxID_STATIC, _("Left:"), wxDefaultPosition, wxDefaultSize, 0);
    marginsizer->Add(mleftlabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mleftctrl = new wxSpinCtrl(this, ID_MARGINLEFT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxSP_ARROW_KEYS, 0, 10000, 0);
    marginsizer->Add(mleftctrl, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // margin top
    wxStaticText *mtoplabel = new wxStaticText(this, wxID_STATIC, _("Top:"), wxDefaultPosition, wxDefaultSize, 0);
    marginsizer->Add(mtoplabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mtopctrl = new wxSpinCtrl(this, ID_MARGINTOP, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxSP_ARROW_KEYS, 0, 10000, 0);
    marginsizer->Add(mtopctrl, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // margin right
    wxStaticText *mrightlabel = new wxStaticText(this, wxID_STATIC, _("Right:"), wxDefaultPosition, wxDefaultSize, 0);
    marginsizer->Add(mrightlabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mrightctrl = new wxSpinCtrl(this, ID_MARGINRIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxSP_ARROW_KEYS, 0, 10000, 0);
    marginsizer->Add(mrightctrl, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // margin bottom
    wxStaticText *mbottomlabel = new wxStaticText(this, wxID_STATIC, _("Bottom:"), wxDefaultPosition, wxDefaultSize, 0);
    marginsizer->Add(mbottomlabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mbottomctrl = new wxSpinCtrl(this, ID_MARGINBOTTOM, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxSP_ARROW_KEYS, 0, 10000, 0);
    marginsizer->Add(mbottomctrl, wxALIGN_CENTER_VERTICAL|wxALL, 3);
*/


    // divider line
    wxStaticLine *line2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line2, 0, wxGROW|wxALL, 3);

    // BUTTONS
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALL, 3);

    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(ok, 0, wxALL, 3);

    // cancel button
    wxButton* cancel = new wxButton ( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(cancel, 0, wxALL, 3);

    // validators

    FindWindow(ID_NAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &maskname_));
    FindWindow(ID_WIDTH)->SetValidator(wxGenericValidator((int*)&maskwidth_));
    FindWindow(ID_HEIGHT)->SetValidator(wxGenericValidator((int*)&maskheight_));

/*
    FindWindow(ID_MARGINLEFT)->SetValidator(wxGenericValidator((int*)&marginleft_));
    FindWindow(ID_MARGINTOP)->SetValidator(wxGenericValidator((int*)&margintop_));
    FindWindow(ID_MARGINRIGHT)->SetValidator(wxGenericValidator((int*)&marginright_));
    FindWindow(ID_MARGINBOTTOM)->SetValidator(wxGenericValidator((int*)&marginbottom_));
*/

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



bool HTMLButcherMaskEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    return true;
}

bool HTMLButcherMaskEditDialog::TransferDataFromWindow()
{
    // check
    wxTextCtrl *namectrl=(wxTextCtrl*)FindWindow(ID_NAME);
    if (namectrl->GetValue().IsEmpty())
    {
        butil_errordialog(_("Name cannot be blank"), this);
        return false;
    }

    // save
    if (!wxDialog::TransferDataFromWindow()) return false;

    return true;
}


