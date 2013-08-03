//=============================================================================
/**
 *  @file    DialogOptions.cpp
 *
 *  $Id: DialogOptions.cpp,v 1.1 2008/02/15 13:42:08 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-15
 */
//=============================================================================
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/notebook.h>
#include <wx/statline.h>

#include "DialogOptions.h"



BEGIN_EVENT_TABLE(HTMLButcherOptionsDialog, wxDialog)
END_EVENT_TABLE()



HTMLButcherOptionsDialog::HTMLButcherOptionsDialog( wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style ) :
    wxDialog(parent, id, caption, pos, size, style),
    editmodeimap_(),
    options_()
{
    Init();
    CreateControls();
}




void HTMLButcherOptionsDialog::Init()
{
    editmodeimap_.Add(ButcherOptions::EM_BASIC, _("Basic"));
    editmodeimap_.Add(ButcherOptions::EM_NORMAL, _("Normal"));
    editmodeimap_.Add(ButcherOptions::EM_ADVANCED, _("Advanced"));
}




void HTMLButcherOptionsDialog::Load(ButcherOptions *options)
{
    options_=*options;
}




void HTMLButcherOptionsDialog::Save(ButcherOptions *options)
{
    *options=options_;
}



void HTMLButcherOptionsDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALL|wxGROW, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);


    // NOTEBOOK
    wxNotebook *notebook=new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE);
    bodysizer->Add(notebook, 1, wxEXPAND|wxALL, 3);

    wxPanel *nbedit = new wxPanel(notebook, wxID_ANY);

    notebook->AddPage(nbedit, _("Edit"), true);

    //*****
    //* EDIT
    //*****

    // EDIT
    wxBoxSizer *nbeditsizer = new wxBoxSizer(wxVERTICAL);
    nbedit->SetSizer(nbeditsizer);

    // edit mode
    wxRadioBox *editmodectrl=new wxRadioBox(nbedit, ID_EDITMODE, _("Edit Mode"), wxDefaultPosition, wxDefaultSize,
        editmodeimap_.Load());
    nbeditsizer->Add(editmodectrl, 0, wxEXPAND|wxALL, 3);




    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    // BUTTONS
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // cancel button
    wxButton* cancel = new wxButton ( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}




bool HTMLButcherOptionsDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxRadioBox *editmodectrl=(wxRadioBox*)FindWindow(ID_EDITMODE);

    editmodectrl->SetSelection(editmodeimap_.GetIndex(options_.GetEditMode()));

    return true;
}



bool HTMLButcherOptionsDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxRadioBox *editmodectrl=(wxRadioBox*)FindWindow(ID_EDITMODE);

    options_.SetEditMode(editmodeimap_.GetId(editmodectrl->GetSelection(), ButcherOptions::EM_ADVANCED));

    return true;
}


