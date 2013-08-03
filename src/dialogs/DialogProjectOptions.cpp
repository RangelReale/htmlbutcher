//=============================================================================
/**
 *  @file    DialogProjectOptions.cpp
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-10-22
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

#include "DialogProjectOptions.h"
#include "BList.h"
#include "BUtil.h"



BEGIN_EVENT_TABLE(HTMLButcherProjectOptionsDialog, ButcherControl_Dialog)
END_EVENT_TABLE()

HTMLButcherProjectOptionsDialog::HTMLButcherProjectOptionsDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style),
	layouttypeimap_()
{
    Init();
    CreateControls();
}

void HTMLButcherProjectOptionsDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-projectoptions.html"));
#endif
	
	layouttype_=ButcherProjectConsts::LTYPE_DEFAULT;

	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_DEFAULT, _("Default (Table)"));
	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_TABLE, _("Table"));
	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_APTABLELESS, _("AP Tableless"));
}

void HTMLButcherProjectOptionsDialog::Load()
{
	layouttype_=GetProject()->ProjectOptions().GetLayoutType();
}

void HTMLButcherProjectOptionsDialog::Save()
{
	GetProject()->ProjectOptions().SetLayoutType(layouttype_);
}

void HTMLButcherProjectOptionsDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);


    // Layout type

	wxRadioBox *layouttypectrl = new wxRadioBox(this, ID_LAYOUTTYPE, _("&Layout type"), wxDefaultPosition, wxSize(300, -1),
        layouttypeimap_.Load(), 0);

    //layouttypeimap_.Load(layouttypectrl);
	
	boxsizer->Add(layouttypectrl, 0, wxGROW|wxALL, 3);


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

#ifdef BUTCHER_USE_HELP
	// help button
    wxButton* help = new wxButton ( this, wxID_HELP, _("&Help"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(help, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
#endif

    // validators

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}

bool HTMLButcherProjectOptionsDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

	wxRadioBox *layouttypectrl = (wxRadioBox*) FindWindow(ID_LAYOUTTYPE);

	// LAYOUT TYPE
	layouttypectrl->SetSelection(layouttypeimap_.GetIndex(layouttype_));

    return true;
}

bool HTMLButcherProjectOptionsDialog::TransferDataFromWindow()
{
    // check

    // save
    if (!wxDialog::TransferDataFromWindow()) return false;

	wxRadioBox *layouttypectrl = (wxRadioBox*) FindWindow(ID_LAYOUTTYPE);

	layouttype_=layouttypeimap_.GetId(layouttypectrl->GetSelection(), ButcherProjectConsts::LTYPE_DEFAULT);

    return true;
}

