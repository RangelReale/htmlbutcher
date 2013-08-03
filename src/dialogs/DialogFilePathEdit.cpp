//=============================================================================
/**
 *  @file    DialogFilePathEdit.cpp
 *
 *  $Id: DialogFilePathEdit.cpp,v 1.4 2008/02/07 12:35:34 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-24
 */
//=============================================================================
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/statline.h>

#include "DialogFilePathEdit.h"
#include "BList.h"
#include "BUtil.h"



BEGIN_EVENT_TABLE(HTMLButcherFilePathEditDialog, ButcherControl_Dialog)
END_EVENT_TABLE()



HTMLButcherFilePathEditDialog::HTMLButcherFilePathEditDialog( wxWindow* parent,
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



void HTMLButcherFilePathEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-filepaths.html"));
#endif

	name_=wxEmptyString;
    path_=wxEmptyString;
}




void HTMLButcherFilePathEditDialog::Load(ButcherProjectFilePath *filepath)
{
    name_=filepath->GetName();
    path_=filepath->GetPath();
}




void HTMLButcherFilePathEditDialog::Save(ButcherProjectFilePath *filepath)
{
    ButcherProjectBaseAutoUpdate upd(filepath);

    filepath->SetName(name_);
    filepath->SetPath(path_);
}




void HTMLButcherFilePathEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);

    // Name
    boxsizer->Add(new wxStaticText(this, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxSize(60, -1), 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl= new wxTextCtrl(this, ID_NAME);
    boxsizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // Path
    boxsizer->Add(new wxStaticText(this, wxID_STATIC, _("&Path:"), wxDefaultPosition, wxSize(60, -1), 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *pathctrl= new wxTextCtrl(this, ID_PATH);
    boxsizer->Add(pathctrl, 0, wxGROW|wxALL, 3);


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
    FindWindow(ID_NAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &name_));
    FindWindow(ID_PATH)->SetValidator(wxTextValidator(wxFILTER_NONE, &path_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}




bool HTMLButcherFilePathEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;
    return true;
}




bool HTMLButcherFilePathEditDialog::TransferDataFromWindow()
{
    wxTextCtrl *namectrl=(wxTextCtrl*)FindWindow(ID_NAME);
    wxTextCtrl *pathctrl=(wxTextCtrl*)FindWindow(ID_PATH);

    // checks
    if (namectrl->GetValue().IsEmpty())
    {
        butil_errordialog(_("Name cannot be blank"), this);
        return false;
    }

    ccu_Path_wxString p(pathctrl->GetValue());
    if (!p.CheckPath())
    {
        butil_errordialog(_("Invalid path"), this);
        return false;
    }
    if (p.IsFull())
    {
        butil_errordialog(_("Path must be a relative path"), this);
        return false;
    }

    // save
    if (!wxDialog::TransferDataFromWindow()) return false;


    return true;
}


