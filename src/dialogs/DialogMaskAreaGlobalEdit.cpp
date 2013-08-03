//=============================================================================
/**
 *  @file    DialogMaskAreaGlobalEdit.cpp
 *
 *  $Id: DialogMaskAreaGlobalEdit.cpp,v 1.11 2008/02/16 12:52:50 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-08
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
#include <wx/dcbuffer.h>
#include <wx/clrpicker.h>
#include <wx/notebook.h>

#include "DialogMaskAreaGlobalEdit.h"
#include "BList.h"
#include "DialogImageFormatEdit.h"



BEGIN_EVENT_TABLE(HTMLButcherMaskAreaGlobalEditDialog, ButcherControl_Dialog)
    EVT_BUTTON(ID_IMAGEFORMATDIALOG, HTMLButcherMaskAreaGlobalEditDialog::OnImageFormat)
    EVT_COMBOBOX(ID_FILEALTERNATE, HTMLButcherMaskAreaGlobalEditDialog::OnAlternateFileChanged)
END_EVENT_TABLE()



HTMLButcherMaskAreaGlobalEditDialog::HTMLButcherMaskAreaGlobalEditDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style),
    areamap_(), loadview_(NULL), loadarea_(NULL)
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    Init();
    CreateControls();
}

	



HTMLButcherMaskAreaGlobalEditDialog::~HTMLButcherMaskAreaGlobalEditDialog()
{
}
	




void HTMLButcherMaskAreaGlobalEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-area.html"));
#endif

	areaname_=wxEmptyString;
    areakind_=ButcherProjectAreaConfig::AK_NONE;
    imageformat_=1;
    filename_=wxEmptyString;
    filename_prepend_=true;
    alternatefile_=-1;
}
	



void HTMLButcherMaskAreaGlobalEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);

    // PREVIEW
    wxBoxSizer *previewsizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(previewsizer, 2, wxEXPAND|wxALL, 3);

    // preview image
    //wxStaticText *previewlabel = new wxStaticText(this, wxID_STATIC, _("Area:"), wxDefaultPosition, wxDefaultSize, 0);
    //previewsizer->Add(previewlabel, 0, wxALIGN_LEFT|wxALL, 3);

    previewctrl_ = new ButcherImageView(this, wxID_ANY, wxDefaultPosition, wxSize(200, 200));

    previewsizer->Add(previewctrl_, 1, wxEXPAND|wxALL, 3);


    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(fieldssizer, 0, wxEXPAND|wxALL, 3);

    // Name
    wxStaticText *namelabel = new wxStaticText(this, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(namelabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl = new wxTextCtrl(this, ID_NAME, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0);
    fieldssizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // kind
    wxStaticText *kindlabel = new wxStaticText(this, wxID_STATIC, _("&Kind:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(kindlabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *kindctrl=new wxComboBox(this, ID_KIND, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);

    areamap_[ButcherProjectAreaConfig::AK_NONE]=kindctrl->Append(_("None"));
    areamap_[ButcherProjectAreaConfig::AK_IMAGE]=kindctrl->Append(_("Image"));
    areamap_[ButcherProjectAreaConfig::AK_MASK]=kindctrl->Append(_("Mask"));
    //kindctrl->SetSelection(2);

    fieldssizer->Add(kindctrl, 0, wxGROW|wxALL, 3);

    // image format
    wxStaticText *formatlabel = new wxStaticText(this, wxID_STATIC, _("&Image Format:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(formatlabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *formatctrl=new wxComboBox(this, ID_IMAGEFORMAT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);

    fieldssizer->Add(formatctrl, 0, wxGROW|wxALL, 3);

    // Filename
    fieldssizer->Add(
        new wxStaticText(this, wxID_STATIC, _("Base &Filename: (without extension)"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *filenamectrl = new wxTextCtrl(this, ID_FILENAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(filenamectrl, 0, wxGROW|wxALL, 3);

    // filename prepend
    wxCheckBox *filenameprependctrl = new wxCheckBox(this, ID_FILENAMEPREPEND, _("Prepen&d parent area filename"), wxDefaultPosition, wxDefaultSize);
    fieldssizer->Add(filenameprependctrl, 0, wxGROW|wxALL, 3);

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        // alternate file
        wxArrayString altfchoices;
        altfchoices.Add(_("No"));
        for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        {
            altfchoices.Add(wxString::Format(_("Alternate %d"), i+1));
        }

        fieldssizer->Add(
            new wxStaticText(this, wxID_STATIC, _("Use &Alternate file:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxComboBox *altfilectrl = new wxComboBox(this, ID_FILEALTERNATE, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            altfchoices, wxCB_READONLY|wxCB_DROPDOWN);
        fieldssizer->Add(altfilectrl, 0, wxGROW|wxALL, 3);
    }

    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // image format button
    wxButton* imgformat = new wxButton ( this, ID_IMAGEFORMATDIALOG, _("Ima&ge Formats..."), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(imgformat, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

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
    FindWindow(ID_NAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &areaname_));
    //FindWindow(ID_WIDTH)->SetValidator(wxGenericValidator((int*)&maskwidth_));
    //FindWindow(ID_HEIGHT)->SetValidator(wxGenericValidator((int*)&maskheight_));
    FindWindow(ID_FILENAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &filename_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



void HTMLButcherMaskAreaGlobalEditDialog::Load(ButcherProjectView *view, ButcherProjectArea *area)
{
    ButcherProjectAreaConfig *config=area->Configs().Get(view->GetBLId());

	areaname_=area->GetName();
    areakind_=config->GetAreaKind();
    imageformat_=config->ImageInfo().GetImageFormatId();
    filename_=config->ImageInfo().GetFilename();
    filename_prepend_=config->ImageInfo().GetFilenamePrepend();
    alternatefile_=config->GetAlternateFile();

	loadview_=view;
	loadarea_=area;
}
	



void HTMLButcherMaskAreaGlobalEditDialog::Save(ButcherProjectView *view, ButcherProjectArea *area)
{
    ButcherProjectBaseAutoUpdate upd(area);

    ButcherProjectAreaConfig *config=area->Configs().Get(view->GetBLId());

    area->SetName(areaname_);
    config->SetAreaKind(areakind_);
    config->ImageInfo().SetImageFormatId(imageformat_);
    config->ImageInfo().SetFilename(filename_);
    config->ImageInfo().SetFilenamePrepend(filename_prepend_);
    config->SetAlternateFile(alternatefile_);
}
	



bool HTMLButcherMaskAreaGlobalEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxComboBox *kindctrl=(wxComboBox*)FindWindow(ID_KIND);
    //wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);
    wxCheckBox *filenameprependctrl=(wxCheckBox*)FindWindow(ID_FILENAMEPREPEND);

    kindctrl->SetSelection(areamap_[areakind_]);
    imageformats_load(imageformat_);
    filenameprependctrl->SetValue(filename_prepend_);

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        wxComboBox *altfilectrl=(wxComboBox*)FindWindow(ID_FILEALTERNATE);
        altfilectrl->SetSelection(alternatefile_+1);
    }

	preview_load();

    return true;
}
	



bool HTMLButcherMaskAreaGlobalEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxComboBox *kindctrl=(wxComboBox*)FindWindow(ID_KIND);
    wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);
    wxCheckBox *filenameprependctrl=(wxCheckBox*)FindWindow(ID_FILENAMEPREPEND);

    for (DialogAreaMap::const_iterator i=areamap_.begin(); i!=areamap_.end(); i++)
        if (i->second == kindctrl->GetSelection())
        {
            areakind_=i->first;
            break;
        }
    imageformat_=static_cast<ButcherListIdClientData*>(formatctrl->GetClientObject(formatctrl->GetSelection()))->GetId();
    filename_prepend_=filenameprependctrl->GetValue();

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        wxComboBox *altfilectrl=(wxComboBox*)FindWindow(ID_FILEALTERNATE);
        alternatefile_=altfilectrl->GetSelection()-1;
    }

    return true;
}
	




void HTMLButcherMaskAreaGlobalEditDialog::OnImageFormat(wxCommandEvent &event)
{
    HTMLButcherImageFormatListEditDialog d(this, wxID_ANY, GetProject());
    if (previewctrl_->GetImage())
        d.SetPreview(previewctrl_->GetImage()->Clone());
    d.ShowModal();

    imageformats_load(imageformat_);
}



void HTMLButcherMaskAreaGlobalEditDialog::imageformats_load(BLID_t selid)
{
    wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);
    formatctrl->Clear();
    int newi;

	for (ButcherProjectImageFormats::iterator i=GetProject()->ImageFormats().begin(); 
		i!=GetProject()->ImageFormats().end(); i++)
	{
        newi=formatctrl->Append(i->GetName(),
            static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
        if (i.first()==selid)
            formatctrl->SetSelection(newi);
	}
}



void HTMLButcherMaskAreaGlobalEditDialog::preview_load()
{
    wxComboBox *altfilectrl=(wxComboBox*)FindWindow(ID_FILEALTERNATE);

	previewctrl_->SetImage(loadview_->GetFileCurrent(altfilectrl->GetSelection()-1)->GetSubImage(
        loadarea_->GetGlobalRect()));
}

void HTMLButcherMaskAreaGlobalEditDialog::OnAlternateFileChanged(wxCommandEvent &event)
{
	preview_load();
}
