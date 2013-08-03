//=============================================================================
/**
 *  @file    DialogListEdit.cpp
 *
 *  $Id: DialogListEdit.cpp,v 1.32 2008/05/18 18:07:10 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#include "DialogListEdit.h"

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/statline.h>
#include <wx/filename.h>
#include "DialogViewEdit.h"
#include "DialogAssortedFileEdit.h"
#include "DialogFilePathEdit.h"
#include "DialogCSSFileEdit.h"
#include "BUtil.h"
#include "BConsts.h"





/////////////////////////////////
// CLASS
//      HTMLButcherListEditDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherListEditDialog, ButcherControl_Dialog)
    EVT_BUTTON(ID_ADD, HTMLButcherListEditDialog::OnAdd)
    EVT_BUTTON(ID_EDIT, HTMLButcherListEditDialog::OnEdit)
    EVT_BUTTON(ID_REMOVE, HTMLButcherListEditDialog::OnRemove)
    EVT_BUTTON(ID_DUPLICATE, HTMLButcherListEditDialog::OnDuplicate)
	EVT_LISTBOX(ID_ITEMS, HTMLButcherListEditDialog::OnSelect)
    EVT_LISTBOX_DCLICK(ID_ITEMS, HTMLButcherListEditDialog::OnEdit)
END_EVENT_TABLE()

HTMLButcherListEditDialog::HTMLButcherListEditDialog( wxWindow* parent,
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




void HTMLButcherListEditDialog::Init()
{

}




void HTMLButcherListEditDialog::CreateControls()
{
    // SIZER: TOP
    topsizer_ = new wxBoxSizer(wxVERTICAL);

    // SIZER: BODY
    bodysizer_ = new wxBoxSizer(wxHORIZONTAL);

    // SIZER: BOX
    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);

    // Items
    wxStaticText *itemslabel = new wxStaticText(this, wxID_STATIC, GetTitle()+wxT(":"), wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(itemslabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxListBox *itemsctrl = new wxListBox(this, ID_ITEMS, wxDefaultPosition, wxSize(250, 200), 0, NULL, wxLB_SORT);
    boxsizer->Add(itemsctrl, 1, wxEXPAND|wxALL, 3);

    // SIZER ADD: BOX
    bodysizer_->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // SIZER: OPBUTTON
    wxBoxSizer *opbuttonsizer = new wxBoxSizer(wxVERTICAL);

    // add button
    wxButton* add = new wxButton ( this, ID_ADD, _("&Add"), wxDefaultPosition, wxDefaultSize, 0 );
    opbuttonsizer->Add(add, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    // edit button
    wxButton* edit = new wxButton ( this, ID_EDIT, _("&Edit"), wxDefaultPosition, wxDefaultSize, 0 );
    opbuttonsizer->Add(edit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    // remove button
    wxButton* remove = new wxButton ( this, ID_REMOVE, _("&Remove"), wxDefaultPosition, wxDefaultSize, 0 );
    opbuttonsizer->Add(remove, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

    // duplicate button
    wxButton* duplicate = new wxButton ( this, ID_DUPLICATE, _("&Duplicate"), wxDefaultPosition, wxDefaultSize, 0 );
    opbuttonsizer->Add(duplicate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);
    duplicate->Enable(false);

#ifdef BUTCHER_USE_HELP
    // help button
    wxButton* help = new wxButton ( this, wxID_HELP, _("&Help"), wxDefaultPosition, wxDefaultSize, 0 );
    opbuttonsizer->Add(help, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);
#endif

    // SIZER ADD: OPBUTTON
    bodysizer_->Add(opbuttonsizer, 0, wxALIGN_CENTER|wxALL, 1);

    // SIZER ADD: BODY
    topsizer_->Add(bodysizer_, 1, wxEXPAND|wxALL, 3);

    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    topsizer_->Add(line, 0, wxEXPAND|wxALL, 3);

    // SIZER: BUTTON
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);

    // close button
    wxButton* close = new wxButton ( this, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(close, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // SIZER ADD: BUTTON
    topsizer_->Add(buttonsizer, 0, wxALIGN_CENTRE, 3);

    SetSizer(topsizer_);
    topsizer_->SetSizeHints(this);

    CentreOnScreen();
}



void HTMLButcherListEditDialog::item_add(const wxString &title, unsigned long id)
{
    wxListBox *lb = (wxListBox*) FindWindow(ID_ITEMS);
    lb->Append(title, static_cast<wxClientData *>(new ButcherListIdClientData(id)));
}




void HTMLButcherListEditDialog::item_edit(unsigned long id, const wxString &title)
{
    wxListBox *lb = (wxListBox*) FindWindow(ID_ITEMS);

    for (unsigned int i=0; i<lb->GetCount(); i++)
        if (static_cast<ButcherListIdClientData*>(lb->GetClientObject(i))->GetId() == id) {
            lb->SetString(i, title);
            return;
        }
}




void HTMLButcherListEditDialog::item_remove(unsigned long id)
{
    wxListBox *lb = (wxListBox*) FindWindow(ID_ITEMS);

    for (unsigned int i=0; i<lb->GetCount(); i++)
        if (static_cast<ButcherListIdClientData*>(lb->GetClientObject(i))->GetId() == id) {
            lb->Delete(i);
            return;
        }
}




BLID_t HTMLButcherListEditDialog::item_current()
{
    wxListBox *lb = (wxListBox*) FindWindow(ID_ITEMS);
    if (lb->GetSelection()!=wxNOT_FOUND)
        return static_cast<ButcherListIdClientData*>(lb->GetClientObject(lb->GetSelection()))->GetId();

    return 0;
}





bool HTMLButcherListEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;
    do_load();
    return true;
}




bool HTMLButcherListEditDialog::TransferDataFromWindow()
{
    return wxDialog::TransferDataFromWindow();
}




void HTMLButcherListEditDialog::OnAdd(wxCommandEvent& event)
{
    do_add();
}




void HTMLButcherListEditDialog::OnEdit(wxCommandEvent& event)
{
    wxListBox *lb = (wxListBox*) FindWindow(ID_ITEMS);

    if (lb->GetSelection() != wxNOT_FOUND) {
        do_edit(static_cast<ButcherListIdClientData*>(lb->GetClientObject(lb->GetSelection()))->GetId());
    }
}





void HTMLButcherListEditDialog::OnRemove(wxCommandEvent& event)
{
    wxListBox *lb = (wxListBox*) FindWindow(ID_ITEMS);

    if (lb->GetSelection() != wxNOT_FOUND) {
        do_remove(static_cast<ButcherListIdClientData*>(lb->GetClientObject(lb->GetSelection()))->GetId());
    }
}



void HTMLButcherListEditDialog::OnDuplicate(wxCommandEvent& event)
{
    wxListBox *lb = (wxListBox*) FindWindow(ID_ITEMS);

    if (lb->GetSelection() != wxNOT_FOUND) {
        do_duplicate(static_cast<ButcherListIdClientData*>(lb->GetClientObject(lb->GetSelection()))->GetId());
    }
}




void HTMLButcherListEditDialog::OnSelect(wxCommandEvent& event)
{
    wxListBox *lb = (wxListBox*) FindWindow(ID_ITEMS);

    if (lb->GetSelection() != wxNOT_FOUND) {
        do_select(static_cast<ButcherListIdClientData*>(lb->GetClientObject(lb->GetSelection()))->GetId());
    }
}




void HTMLButcherListEditDialog::ControlsChanged()
{
    topsizer_->SetSizeHints(this);

    CentreOnScreen();
}




void HTMLButcherListEditDialog::EnableAdd(bool enable)
{
    FindWindow(ID_ADD)->Enable(enable);
    if (!enable)
        FindWindow(ID_DUPLICATE)->Enable(false);
}

void HTMLButcherListEditDialog::EnableDuplicate()
{
    FindWindow(ID_DUPLICATE)->Enable(true);
}




/////////////////////////////////
// CLASS
//      HTMLButcherMasksDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherMasksDialog, HTMLButcherListEditDialog)
END_EVENT_TABLE()

void HTMLButcherMasksDialog::do_load()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-masks.html"));
#endif
	EnableDuplicate();
	EnableAdd(!GetProject()->Masks().IsFull());

	for (ButcherProjectMasks::iterator i=GetProject()->Masks().begin();
		i!=GetProject()->Masks().end(); i++)
	{
        item_add(make_description(&i.second()), i.first());
	}
}




void HTMLButcherMasksDialog::do_add()
{
    HTMLButcherMaskEditDialog d(this, wxID_ANY, GetProject());
    //get_configs(0, d.Configs());
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());

        unsigned long newid=GetProject()->Masks().Add(d.GetMaskName(), d.GetMaskWidth(), d.GetMaskHeight());
        //set_configs(newid, d.Configs());
        d.Save(GetProject()->Masks().Get(newid));
        item_add(make_description(GetProject()->Masks().Get(newid)), newid);

		EnableAdd(!GetProject()->Masks().IsFull());
    }
}



void HTMLButcherMasksDialog::do_edit(unsigned long id)
{
    HTMLButcherMaskEditDialog d(this, wxID_ANY, GetProject());
    d.SetMaskName(GetProject()->Masks().Get(id)->GetName());
    d.SetMaskWidth(GetProject()->Masks().Get(id)->GetWidth());
    d.SetMaskHeight(GetProject()->Masks().Get(id)->GetHeight());
/*
    d.SetMarginLeft(GetProject()->Masks().Get(id)->Areas()->GetMarginLeft());
    d.SetMarginTop(GetProject()->Masks().Get(id)->Areas()->GetMarginTop());
    d.SetMarginRight(GetProject()->Masks().Get(id)->Areas()->GetMarginRight());
    d.SetMarginBottom(GetProject()->Masks().Get(id)->Areas()->GetMarginBottom());
*/
    d.Load(GetProject()->Masks().Get(id));
    //get_configs(id, d.Configs());
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());

        GetProject()->Masks().Edit(id, d.GetMaskName(), d.GetMaskWidth(),
            d.GetMaskHeight());
        //set_configs(id, d.Configs());
/*
        GetProject()->Masks().Get(id)->Areas()->SetMarginLeft(d.GetMarginLeft());
        GetProject()->Masks().Get(id)->Areas()->SetMarginTop(d.GetMarginTop());
        GetProject()->Masks().Get(id)->Areas()->SetMarginRight(d.GetMarginRight());
        GetProject()->Masks().Get(id)->Areas()->SetMarginBottom(d.GetMarginBottom());
*/
        d.Save(GetProject()->Masks().Get(id));
        //GetProject()->Masks().ForceModified(id);

        item_edit(id, make_description(GetProject()->Masks().Get(id)));
    }
}




void HTMLButcherMasksDialog::do_remove(unsigned long id)
{
    // get number of views using this mask
    ButcherProjectEventNotify en(GetProject(), ButcherProjectEvent::BPE_MASKDELETED, id, 0, false);
    GetProject()->ExecuteEventNotify(en);

    wxString remmsg=_("Are you sure you want to remove this mask?");
    if (en.GetRemoveCount()>0)
        remmsg=wxString::Format(_("Removing this mask will remove %d view(s). Are you sure?"), en.GetRemoveCount());

    wxMessageDialog d(this, remmsg, _("Remove mask"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (d.ShowModal() != wxID_YES) return;

    ButcherProjectBaseAutoUpdate upd(GetProject());
    if (!GetProject()->Masks().Delete(id))
    {
        butil_errordialog(_("This mask cannot be deleted"), this);
        return;
    }
    item_remove(id);

	EnableAdd(!GetProject()->Masks().IsFull());
}




void HTMLButcherMasksDialog::do_duplicate(unsigned long id)
{
    wxString newname=wxGetTextFromUser(_("New mask name"), _("Duplicate mask"), wxEmptyString, this);
    if (!newname.IsEmpty())
    {
        ButcherProjectBaseAutoUpdate upd(GetProject());

        BLID_t newid=GetProject()->Masks().Duplicate(id, newname);
        item_add(make_description(GetProject()->Masks().Get(newid)), newid);
    }

}



wxString HTMLButcherMasksDialog::make_description(ButcherProjectMask *item)
{
    return wxString::Format(wxT("%s [%dx%d]"), item->GetName().c_str(), item->GetWidth(), item->GetHeight());
}




/////////////////////////////////
// CLASS
//      HTMLButcherViewsDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherViewsDialog, HTMLButcherListEditDialog)
END_EVENT_TABLE()

void HTMLButcherViewsDialog::do_load()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-views.html"));
#endif
	EnableAdd(!GetProject()->Views().IsFull());

	for (ButcherProjectViews::iterator i=GetProject()->Views().begin();
		i!=GetProject()->Views().end(); i++)
	{
        item_add(make_description(&i.second()), i.first());
	}
}



void HTMLButcherViewsDialog::do_add()
{
    HTMLButcherViewEditDialog d(this, wxID_ANY, GetProject());
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        unsigned long newid=GetProject()->Views().Add(d.GetViewName(), d.GetFileId(),
            d.GetMaskId());
        d.Save(newid);
        item_add(make_description(GetProject()->Views().Get(newid)), newid);

		EnableAdd(!GetProject()->Views().IsFull());
    }
}




void HTMLButcherViewsDialog::do_edit(unsigned long id)
{
    HTMLButcherViewEditDialog d(this, wxID_ANY, GetProject());
    d.Load(id);
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        d.Save(id);
        item_edit(id, make_description(GetProject()->Views().Get(id)));
    }
}




void HTMLButcherViewsDialog::do_remove(unsigned long id)
{
    wxString remmsg=_("Are you sure you want to remove this view?");

    wxMessageDialog d(this, remmsg, _("Remove view"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (d.ShowModal() != wxID_YES) return;

    ButcherProjectBaseAutoUpdate upd(GetProject());
    if (!GetProject()->Views().Delete(id))
    {
        butil_errordialog(_("This view cannot be deleted"), this);
        return;
    }
    item_remove(id);

	EnableAdd(!GetProject()->Views().IsFull());
}



wxString HTMLButcherViewsDialog::make_description(ButcherProjectView *item)
{
    return wxString::Format(wxT("%s [%s - %s]"), item->GetName().c_str(),
        item->GetFile()->GetDisplayName().c_str(),
        item->GetMask()->GetName().c_str());
}

/////////////////////////////////
// CLASS
//      HTMLButcherAssortedFileGroupsDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherAssortedFileGroupsDialog, HTMLButcherListEditDialog)
END_EVENT_TABLE()

void HTMLButcherAssortedFileGroupsDialog::do_load()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-assortedfilegroups.html"));
#endif

	for (ButcherProjectAssortedFileGroups::iterator i=GetProject()->AssortedFileGroups().begin();
		i!=GetProject()->AssortedFileGroups().end(); i++)
	{
        item_add(make_description(&*i), i.first());
	}
}



void HTMLButcherAssortedFileGroupsDialog::do_add()
{
    wxString groupname=wxGetTextFromUser(_("Group name"), _("Add Assorted File Group"),
        wxEmptyString, this);
    if (!groupname.IsEmpty()) {
        ButcherProjectBaseAutoUpdate upd(GetProject());

        unsigned long newid=GetProject()->AssortedFileGroups().Add(groupname);
        item_add(make_description(GetProject()->AssortedFileGroups().Get(newid)), newid);
    }

}




void HTMLButcherAssortedFileGroupsDialog::do_edit(unsigned long id)
{
    wxString groupname=wxGetTextFromUser(_("Group name"), _("Add Assorted File Group"),
        GetProject()->AssortedFileGroups().Get(id)->GetName(), this);

    if (!groupname.IsEmpty()) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        GetProject()->AssortedFileGroups().Edit(id, groupname);
        item_edit(id, make_description(GetProject()->AssortedFileGroups().Get(id)));
    }
}




void HTMLButcherAssortedFileGroupsDialog::do_remove(unsigned long id)
{
    wxString remmsg=_("Are you sure you want to remove this group?");

    wxMessageDialog d(this, remmsg, _("Remove group"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (d.ShowModal() != wxID_YES) return;

    ButcherProjectBaseAutoUpdate upd(GetProject());
    if (!GetProject()->AssortedFileGroups().Delete(id))
    {
        butil_errordialog(_("This group cannot be deleted"), this);
        return;
    }
    item_remove(id);
}




wxString HTMLButcherAssortedFileGroupsDialog::make_description(ButcherProjectAssortedFileGroup *item)
{
    return item->GetName();
}



/////////////////////////////////
// CLASS
//      HTMLButcherAssortedFilesDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherAssortedFilesDialog, HTMLButcherListEditDialog)
END_EVENT_TABLE()

void HTMLButcherAssortedFilesDialog::do_load()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-assortedfiles.html"));
#endif
	for (ButcherProjectAssortedFiles::iterator i=GetProject()->AssortedFiles().begin();
		i!=GetProject()->AssortedFiles().end(); i++)
	{
        item_add(make_description(&*i), i.first());
	}
}




void HTMLButcherAssortedFilesDialog::do_add()
{
    wxFileDialog d(this, _("Add assorted file"), wxEmptyString, wxEmptyString,
        _("All files|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
    if (d.ShowModal() == wxID_OK) {
        wxArrayString fls;
        d.GetPaths(fls);
        ButcherProjectBaseAutoUpdate upd(GetProject());
        for (unsigned int i=0; i<fls.Count(); i++) {
            unsigned long newid=GetProject()->AssortedFiles().Add(fls[i]);
            item_add(make_description(GetProject()->AssortedFiles().Get(newid)), newid);
        }
    }

}




void HTMLButcherAssortedFilesDialog::do_edit(unsigned long id)
{
    HTMLButcherAssortedFileEditDialog d(this, wxID_ANY, GetProject());

    d.Load(GetProject()->AssortedFiles().Get(id));

/*
    wxFileDialog d(this, _("Assorted File"), wxEmptyString, GetProject()->Files().Get(id)->GetFilename(),
        _("All files|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
*/
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        //GetProject()->AssortedFiles().Edit(id, d.GetPath());
        d.Save(GetProject()->AssortedFiles().Get(id));

        item_edit(id, make_description(GetProject()->AssortedFiles().Get(id)));
    }
}



void HTMLButcherAssortedFilesDialog::do_remove(unsigned long id)
{
    wxString remmsg=_("Are you sure you want to remove this file?");

    wxMessageDialog d(this, remmsg, _("Remove file"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (d.ShowModal() != wxID_YES) return;

    ButcherProjectBaseAutoUpdate upd(GetProject());
    if (!GetProject()->AssortedFiles().Delete(id))
    {
        butil_errordialog(_("This file cannot be deleted"), this);
        return;
    }
    item_remove(id);
}




wxString HTMLButcherAssortedFilesDialog::make_description(ButcherProjectAssortedFile *item)
{
    return item->GetPathFile();
}




/////////////////////////////////
// CLASS
//      HTMLButcherFilePathsDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherFilePathsDialog, HTMLButcherListEditDialog)
END_EVENT_TABLE()

void HTMLButcherFilePathsDialog::do_load()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-filepaths.html"));
#endif
	for (ButcherProjectFilePaths::iterator i=GetProject()->FilePaths().begin();
		i!=GetProject()->FilePaths().end(); i++)
	{
        item_add(make_description(&*i), i.first());
	}
}




void HTMLButcherFilePathsDialog::do_add()
{
    HTMLButcherFilePathEditDialog d(this, wxID_ANY, GetProject());

    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        unsigned long newid=GetProject()->FilePaths().Add(d.GetFilePathName());
        d.Save(GetProject()->FilePaths().Get(newid));

        item_add(make_description(GetProject()->FilePaths().Get(newid)), newid);
    }
}




void HTMLButcherFilePathsDialog::do_edit(unsigned long id)
{
    HTMLButcherFilePathEditDialog d(this, wxID_ANY, GetProject());

    d.Load(GetProject()->FilePaths().Get(id));

    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        //GetProject()->AssortedFiles().Edit(id, d.GetPath());
        d.Save(GetProject()->FilePaths().Get(id));

        item_edit(id, make_description(GetProject()->FilePaths().Get(id)));
    }
}




void HTMLButcherFilePathsDialog::do_remove(unsigned long id)
{
    // get number of views using this mask
    ButcherProjectEventNotify en(GetProject(), ButcherProjectEvent::BPE_FILEPATHDELETED, id, 0, false);
    GetProject()->ExecuteEventNotify(en);

    wxString remmsg=_("Are you sure you want to remove this path?");
    if (en.GetRemoveCount()>0||en.GetChangeCount()>0)
        remmsg=wxString::Format(_("Removing this path will affect %d item(s). Are you sure?"), en.GetRemoveCount()+en.GetChangeCount());

    wxMessageDialog d(this, remmsg, _("Remove path"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (d.ShowModal() != wxID_YES) return;

    ButcherProjectBaseAutoUpdate upd(GetProject());
    if (!GetProject()->FilePaths().Delete(id))
    {
        butil_errordialog(_("This path cannot be deleted"), this);
        return;
    }
    item_remove(id);
}




wxString HTMLButcherFilePathsDialog::make_description(ButcherProjectFilePath *item)
{
    return item->GetName();
}




/////////////////////////////////
// CLASS
//      HTMLButcherCSSFilesDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherCSSFilesDialog, HTMLButcherListEditDialog)
END_EVENT_TABLE()

void HTMLButcherCSSFilesDialog::do_load()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-cssfile.html"));
#endif
	for (ButcherProjectCSSFiles::iterator i=GetProject()->CSSFiles().begin();
		i!=GetProject()->CSSFiles().end(); i++)
	{
        item_add(make_description(&*i), i.first());
	}
}



void HTMLButcherCSSFilesDialog::do_add()
{
    HTMLButcherCSSFileEditDialog d(this, wxID_ANY, GetProject());

    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());

        BLID_t newid=GetProject()->CSSFiles().Add(d.GetCSSName());
        d.Save(GetProject()->CSSFiles().Get(newid));

        item_add(make_description(GetProject()->CSSFiles().Get(newid)), newid);
    }

}




void HTMLButcherCSSFilesDialog::do_edit(unsigned long id)
{
    HTMLButcherCSSFileEditDialog d(this, wxID_ANY, GetProject());

    d.Load(GetProject()->CSSFiles().Get(id));

/*
    wxFileDialog d(this, _("CSS File"), wxEmptyString, GetProject()->Files().Get(id)->GetFilename(),
        _("All files|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
*/
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        //GetProject()->CSSFiles().Edit(id, d.GetPath());
        d.Save(GetProject()->CSSFiles().Get(id));

        item_edit(id, make_description(GetProject()->CSSFiles().Get(id)));
    }
}



void HTMLButcherCSSFilesDialog::do_remove(unsigned long id)
{
    // get number of views using this mask
    ButcherProjectEventNotify en(GetProject(), ButcherProjectEvent::BPE_CSSFILEDELETED, id, 0, false);
    GetProject()->ExecuteEventNotify(en);

    wxString remmsg=_("Are you sure you want to remove this CSS?");
    if (en.GetRemoveCount()>0||en.GetChangeCount()>0)
        remmsg=wxString::Format(_("Removing this CSS will affect %d item(s). Are you sure?"), en.GetRemoveCount()+en.GetChangeCount());

    wxMessageDialog d(this, remmsg, _("Remove CSS"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (d.ShowModal() != wxID_YES) return;

    ButcherProjectBaseAutoUpdate upd(GetProject());
    if (!GetProject()->CSSFiles().Delete(id))
    {
        butil_errordialog(_("This CSS cannot be deleted"), this);
        return;
    }
    item_remove(id);
}




wxString HTMLButcherCSSFilesDialog::make_description(ButcherProjectCSSFile *item)
{
    return item->GetName();
}


