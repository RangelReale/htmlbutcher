//=============================================================================
/**
 *  @file    DialogFileEdit.cpp
 *
 *  $Id: DialogFileEdit.cpp,v 1.6 2008/02/10 21:26:00 rreale Exp $
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

#include "DialogFileEdit.h"
#include "BList.h"
#include "BUtil.h"




/////////////////////////////////
// CLASS
//      HTMLButcherFilesDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherFilesDialog, HTMLButcherListEditDialog)
    EVT_CHECKBOX(ID_AUTOUPDATE, HTMLButcherFilesDialog::OnAutoUpdate)
END_EVENT_TABLE()



void HTMLButcherFilesDialog::CreatePreviewControls()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-files.html"));
#endif

	// PREVIEW
    previewsizer_ = new wxBoxSizer(wxVERTICAL);

    // preview image
    previewlabel_ = new wxStaticText(this, wxID_STATIC, _("Preview:"), wxDefaultPosition, wxDefaultSize, 0);
    previewsizer_->Add(previewlabel_, 0, wxALIGN_LEFT|wxALL, 3);

    previewctrl_ = new ButcherImageView(this, wxID_ANY, wxPoint(0, 0), wxSize(300, 300));
    previewctrl_->SetBGColor(wxColour(wxT("WHITE")));
    //previewctrl_->SetFullSize(true);

    previewsizer_->Add(previewctrl_, 1, wxEXPAND|wxALL, 3);

    // AUTO UPDATE
    wxCheckBox *autoupdatectrl=new wxCheckBox(this, ID_AUTOUPDATE, _("Show preview"));
    autoupdatectrl->SetValue(false);
    previewsizer_->Add(autoupdatectrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    GetBodySizer()->Prepend(previewsizer_, 1, wxEXPAND|wxALL|wxFIXED_MINSIZE, 3);

    ControlsChanged();
}



void HTMLButcherFilesDialog::do_select(BLID_t id)
{
    wxCheckBox *cb=(wxCheckBox*)FindWindow(ID_AUTOUPDATE);
    if (cb->GetValue())
        UpdatePreview(id);
}




void HTMLButcherFilesDialog::do_load()
{
	EnableAdd(!GetProject()->Files().IsFull());

	for (ButcherProjectFiles::iterator i=GetProject()->Files().begin();
		i!=GetProject()->Files().end(); i++)
	{
        item_add(make_description(&i.second()), i.first());
	}
}




void HTMLButcherFilesDialog::do_add()
{
    wxFileDialog d(this, _("Add file"), wxEmptyString, wxEmptyString,
        _("All files|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
    if (d.ShowModal() == wxID_OK) {
        wxArrayString fls;
        d.GetPaths(fls);

        ButcherProjectBaseAutoUpdate upd(GetProject());
        for (unsigned int i=0; i<fls.Count(); i++) {
            unsigned long newid=GetProject()->Files().Add(wxEmptyString, fls[i]);
            item_add(make_description(GetProject()->Files().Get(newid)), newid);
        }

		EnableAdd(!GetProject()->Files().IsFull());
    }

}




void HTMLButcherFilesDialog::do_edit(unsigned long id)
{
/*
    wxFileDialog d(this, wxT("File"), wxEmptyString, GetProject()->Files().Get(id)->GetFilename(),
        wxT("All files|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
*/
    HTMLButcherFileEditDialog d(this, wxID_ANY, GetProject());
    d.Load(GetProject()->Files().Get(id));
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());

        d.Save(GetProject()->Files().Get(id));
        //GetProject()->Files().Edit(id, d.GetPath());
        item_edit(id, make_description(GetProject()->Files().Get(id)));
    }
}



void HTMLButcherFilesDialog::do_remove(unsigned long id)
{
    // get number of views using this mask
    ButcherProjectEventNotify en(GetProject(), ButcherProjectEvent::BPE_FILEDELETED, id, 0, false);
    GetProject()->ExecuteEventNotify(en);

    wxString remmsg=_("Are you sure you want to remove this file?");
    if (en.GetRemoveCount()>0)
        remmsg=wxString::Format(_("Removing this file will remove %d view(s). Are you sure?"), en.GetRemoveCount());
    else if (en.GetChangeCount()>0)
        remmsg=wxString::Format(_("Removing this file will affect %d item(s). Are you sure?"), en.GetChangeCount());

    wxMessageDialog d(this, remmsg, _("Remove path"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (d.ShowModal() != wxID_YES) return;

    ButcherProjectBaseAutoUpdate upd(GetProject());

    if (!GetProject()->Files().Delete(id))
    {
        butil_errordialog(_("This file cannot be deleted"), this);
        return;
    }

    item_remove(id);

	EnableAdd(!GetProject()->Files().IsFull());
}




wxString HTMLButcherFilesDialog::make_description(ButcherProjectFile *item)
{
    return wxString::Format(wxT("%s [%dx%d]"), item->GetDisplayName().c_str(), item->GetImageWidth(), item->GetImageHeight());
}




void HTMLButcherFilesDialog::UpdatePreview()
{
    if (item_current() > 0)
        UpdatePreview(item_current());
}




void HTMLButcherFilesDialog::UpdatePreview(BLID_t id)
{
    SetCursor(*wxHOURGLASS_CURSOR);
    try
    {
        previewctrl_->SetImage(GetProject()->Files().Get(id)->CreateImage(), true);
    } catch(...) {
        SetCursor(*wxSTANDARD_CURSOR);
        throw;
    }
    SetCursor(*wxSTANDARD_CURSOR);
}




void HTMLButcherFilesDialog::OnAutoUpdate(wxCommandEvent &event)
{
    if (event.IsChecked())
        UpdatePreview();
    event.Skip();
}




/////////////////////////////////
// CLASS
//      HTMLButcherFileEditDialog
/////////////////////////////////

BEGIN_EVENT_TABLE(HTMLButcherFileEditDialog, ButcherControl_Dialog)
END_EVENT_TABLE()

HTMLButcherFileEditDialog::HTMLButcherFileEditDialog( wxWindow* parent,
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



void HTMLButcherFileEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-files.html"));
#endif

	name_=wxEmptyString;
    filename_=wxEmptyString;
}




void HTMLButcherFileEditDialog::Load(ButcherProjectFile *file)
{
    name_=file->GetName();
    filename_=file->GetFilename();
}




void HTMLButcherFileEditDialog::Save(ButcherProjectFile *file)
{
    ButcherProjectBaseAutoUpdate upd(file);

    file->SetName(name_);
    file->SetFilename(filename_);
}




void HTMLButcherFileEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);

    // Name
    boxsizer->Add(new wxStaticText(this, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxSize(60, -1), 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl = new wxTextCtrl(this, ID_NAME);
    boxsizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // File
    boxsizer->Add(new wxStaticText(this, wxID_STATIC, _("&File:"), wxDefaultPosition, wxSize(250, -1), 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxFilePickerCtrl *filectrl = new wxFilePickerCtrl(this, ID_FILE, wxEmptyString, _("Select a file"),
        _("All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_OPEN|wxFLP_USE_TEXTCTRL);
    boxsizer->Add(filectrl, 0, wxGROW|wxALL, 3);



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

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



bool HTMLButcherFileEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxFilePickerCtrl *filectrl=(wxFilePickerCtrl*)FindWindow(ID_FILE);

    filectrl->SetPath(filename_);
    return true;
}




bool HTMLButcherFileEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxFilePickerCtrl *filectrl=(wxFilePickerCtrl*)FindWindow(ID_FILE);

    // checks
    if (filectrl->GetPath()!=filename_ && !wxFileName::FileExists(filectrl->GetPath()))
    {
        // file changed, file must exist
        butil_errordialog(_("File does not exists"), this);
        return false;
    }

    // save
    filename_=filectrl->GetPath();
    return true;
}


