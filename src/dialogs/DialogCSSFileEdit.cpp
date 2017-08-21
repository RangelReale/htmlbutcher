
//=============================================================================
/**
 *  @file    DialogCSSFileEdit.cpp
 *
 *  $Id: DialogCSSFileEdit.cpp,v 1.6 2008/02/08 13:24:46 rreale Exp $
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

#include <wx/filepicker.h>
#include <wx/checklst.h>
#include <wx/statline.h>
#include <wx/filename.h>

#include "DialogCSSFileEdit.h"
#include "BList.h"
#include "BControls.h"
#include "BUtil.h"



BEGIN_EVENT_TABLE(HTMLButcherCSSFileEditDialog, ButcherControl_Dialog)
END_EVENT_TABLE()



HTMLButcherCSSFileEditDialog::HTMLButcherCSSFileEditDialog( wxWindow* parent,
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



void HTMLButcherCSSFileEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-cssfile.html"));
#endif

	name_=wxEmptyString;
    csstext_=wxEmptyString;
    filepathid_=GetProject()->FilePaths().DefaultId();
    savefilename_=wxEmptyString;
}




void HTMLButcherCSSFileEditDialog::Load(ButcherProjectCSSFile *afile)
{
    name_=afile->GetName();
    csstext_=afile->GetCSSText();
    filepathid_=afile->GetFilePathId();
    savefilename_=afile->GetSaveFilename();
}




void HTMLButcherCSSFileEditDialog::Save(ButcherProjectCSSFile *afile)
{
    ButcherProjectBaseAutoUpdate upd(afile);

    afile->SetName(name_);
    afile->SetCSSText(csstext_);
    afile->SetFilePathId(filepathid_);
    afile->SetSaveFilename(savefilename_);
}




void HTMLButcherCSSFileEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALL|wxGROW, 3);

    // Name
    boxsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl = new wxTextCtrl(this, ID_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // CSS Text
    boxsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&CSS Text:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    ButcherControl_FmtTextCtrl *csstextctrl = new ButcherControl_FmtTextCtrl(this, ID_CSSTEXT, wxDefaultPosition, wxSize(450, 300));
    csstextctrl->SetEditFormat(ButcherControl_FmtTextCtrl::FMT_CSS);
    boxsizer->Add(csstextctrl, 1, wxGROW|wxALL, 3);


    // Image dir
    boxsizer->Add(new wxStaticText(this, wxID_STATIC, _("File &path:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *filepathctrl = new wxComboBox(this, ID_FILEPATH, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT);
    boxsizer->Add(filepathctrl, 0, wxGROW|wxALL, 3);

    // SaveFilename
    boxsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Save filename:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *savefilenamectrl = new wxTextCtrl(this, ID_SAVEFILENAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(savefilenamectrl, 0, wxGROW|wxALL, 3);



    // divider line
    wxStaticLine *line2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line2, 0, wxGROW|wxALL, 3);

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
    FindWindow(ID_NAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &name_));
    FindWindow(ID_CSSTEXT)->SetValidator(ButcherControl_GenericValidator(&csstext_));
    FindWindow(ID_SAVEFILENAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &savefilename_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



bool HTMLButcherCSSFileEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxComboBox *filepathctrl = (wxComboBox*) FindWindow(ID_FILEPATH);

    wxString tmp;

	for (ButcherProjectFilePaths::iterator i=GetProject()->FilePaths().begin(); 
		i!=GetProject()->FilePaths().end(); i++)
	{
        tmp=wxString::Format(wxT("%s"),
            i->GetName().c_str());

        wxccu_control_append_select(filepathctrl, tmp, i.first(), filepathid_);
	}

    return true;
}




bool HTMLButcherCSSFileEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxTextCtrl *namectrl = (wxTextCtrl*) FindWindow(ID_NAME);
    wxComboBox *filepathctrl = (wxComboBox*) FindWindow(ID_FILEPATH);
    wxTextCtrl *savefilenamectrl = (wxTextCtrl*) FindWindow(ID_SAVEFILENAME);

    // check
    if (namectrl->GetValue().IsEmpty())
    {
        butil_errordialog(_("Name cannot be blank"));
        return false;
    }
    if (!savefilenamectrl->GetValue().IsEmpty() &&
        savefilenamectrl->GetValue().find_first_of(wxFileName::GetForbiddenChars())!=wxString::npos)
    {
        butil_errordialog(_("Invalid save file name"), this);
        return false;
    }

    // save
    filepathid_=wxccu_control_getselectedid(filepathctrl, 0);

    return true;
}


