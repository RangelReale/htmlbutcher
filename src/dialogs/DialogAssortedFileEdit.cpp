//=============================================================================
/**
 *  @file    DialogAssortedFileEdit.cpp
 *
 *  $Id: DialogAssortedFileEdit.cpp,v 1.7 2008/02/07 12:35:34 rreale Exp $
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

#include "DialogAssortedFileEdit.h"
#include "BList.h"
#include "BUtil.h"



BEGIN_EVENT_TABLE(HTMLButcherAssortedFileEditDialog, ButcherControl_Dialog)
END_EVENT_TABLE()

HTMLButcherAssortedFileEditDialog::HTMLButcherAssortedFileEditDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style), filetypemap_(),
        groups_()
{
    Init();
    CreateControls();
}

void HTMLButcherAssortedFileEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-assortedfiles.html"));
#endif

	filename_=wxEmptyString;
    filetype_=ButcherProjectAssortedFile::AFT_UNKNOWN;
    filepath_=GetProject()->FilePaths().DefaultId();
    savefilename_=wxEmptyString;
}

void HTMLButcherAssortedFileEditDialog::Load(ButcherProjectAssortedFile *afile)
{
    filename_=afile->GetFilename();
    groups_=afile->Groups();
    filetype_=afile->GetFileType();
    filepath_=afile->GetFilePath();
    savefilename_=afile->GetSaveFilename();
}

void HTMLButcherAssortedFileEditDialog::Save(ButcherProjectAssortedFile *afile)
{
    ButcherProjectBaseAutoUpdate upd(afile);

    afile->SetFilename(filename_);
    afile->Groups().Set(groups_.Get());
    afile->SetFileType(filetype_);
    afile->SetFilePath(filepath_);
    afile->SetSaveFilename(savefilename_);
}

void HTMLButcherAssortedFileEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);

    // File
    boxsizer->Add(new wxStaticText(this, wxID_STATIC, _("&File:"), wxDefaultPosition, wxSize(60, -1), 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxFilePickerCtrl *filectrl = new wxFilePickerCtrl(this, ID_FILE, wxEmptyString, _("Select a file"),
        _("All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_OPEN|wxFLP_USE_TEXTCTRL);
    boxsizer->Add(filectrl, 0, wxGROW|wxALL, 3);

    // File type
    boxsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("File &Type:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *filetypectrl=new wxComboBox(this, ID_FILETYPE, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);

    filetypemap_[ButcherProjectAssortedFile::AFT_UNKNOWN]=filetypectrl->Append(_("Unknown"));
    filetypemap_[ButcherProjectAssortedFile::AFT_IMAGE]=filetypectrl->Append(_("Image"));

    boxsizer->Add(filetypectrl, 0, wxGROW|wxALL, 3);

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

    // Groups
    boxsizer->Add(new wxStaticText(this, wxID_STATIC, _("&Groups:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxCheckListBox *groupsctrl = new wxCheckListBox(this, ID_GROUPS, wxDefaultPosition, wxDefaultSize);
    boxsizer->Add(groupsctrl, 1, wxGROW|wxALL, 3);

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
    FindWindow(ID_SAVEFILENAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &savefilename_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



bool HTMLButcherAssortedFileEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxFilePickerCtrl *filectrl=(wxFilePickerCtrl*)FindWindow(ID_FILE);
    wxComboBox *filetypectrl=(wxComboBox*)FindWindow(ID_FILETYPE);
    wxCheckListBox *groupsctrl=(wxCheckListBox*)FindWindow(ID_GROUPS);
    wxComboBox *filepathctrl = (wxComboBox*) FindWindow(ID_FILEPATH);

    filectrl->SetPath(filename_);
    filetypectrl->SetSelection(filetypemap_[filetype_]);

    int newitem;
	for (ButcherProjectAssortedFileGroups::iterator i=GetProject()->AssortedFileGroups().begin(); 
		i!=GetProject()->AssortedFileGroups().end(); i++)
	{
        newitem=groupsctrl->Append(i->GetName());
        groupsctrl->Check(newitem, groups_.Exists(i.first()));
	}

    wxString tmp;

	for (ButcherProjectFilePaths::iterator i=GetProject()->FilePaths().begin(); 
		i!=GetProject()->FilePaths().end(); i++)
	{
        tmp=wxString::Format(wxT("%s"),
            i->GetName().c_str());

        newitem=filepathctrl->Append(tmp,
            static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
        if (i.first()==filepath_)
            filepathctrl->SetSelection(newitem);
	}

    return true;
}

bool HTMLButcherAssortedFileEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxFilePickerCtrl *filectrl=(wxFilePickerCtrl*)FindWindow(ID_FILE);
    wxComboBox *filetypectrl=(wxComboBox*)FindWindow(ID_FILETYPE);
    wxCheckListBox *groupsctrl=(wxCheckListBox*)FindWindow(ID_GROUPS);
    wxComboBox *filepathctrl = (wxComboBox*) FindWindow(ID_FILEPATH);
    wxTextCtrl *savefilenamectrl = (wxTextCtrl*) FindWindow(ID_SAVEFILENAME);

    // Checks
    if (filectrl->GetPath().IsEmpty())
    {
        butil_errordialog(_("File cannot be blank"), this);
        return false;
    }
    if (filectrl->GetPath()!=filename_ && !wxFileName::FileExists(filectrl->GetPath()))
    {
        // file changed, file must exist
        butil_errordialog(_("File does not exists"), this);
        return false;
    }
    if (!savefilenamectrl->GetValue().IsEmpty() &&
        savefilenamectrl->GetValue().find_first_of(wxFileName::GetForbiddenChars())!=wxString::npos)
    {
        butil_errordialog(_("Invalid save file name"), this);
        return false;
    }

    // Save
    filename_=filectrl->GetPath();
    for (DialogFileTypeMap::const_iterator i=filetypemap_.begin(); i!=filetypemap_.end(); i++)
        if (i->second == filetypectrl->GetSelection())
        {
            filetype_=i->first;
            break;
        }

    if (filepathctrl->GetSelection()!=wxNOT_FOUND)
        filepath_=static_cast<ButcherListIdClientData*>(filepathctrl->GetClientObject(filepathctrl->GetSelection()))->GetId();
    else
        filepath_=0;

    groups_.Clear();
    int ct=0;
	for (ButcherProjectAssortedFileGroups::iterator i=GetProject()->AssortedFileGroups().begin(); 
		i!=GetProject()->AssortedFileGroups().end(); i++)
	{
        if (groupsctrl->IsChecked(ct))
            groups_.Add(i.first());
        ct++;
	}
    return true;
}



