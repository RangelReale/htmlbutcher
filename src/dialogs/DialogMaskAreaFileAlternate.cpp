//=============================================================================
/**
 *  @file    DialogMaskAreaFileAlternate.cpp
 *
 *  $Id: DialogMaskAreaFileAlternate.cpp,v 1.5 2008/02/06 16:40:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-19
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
#include <wx/radiobox.h>

#include "DialogMaskAreaFileAlternate.h"
#include "BList.h"
#include "DialogMaskAreaImageFormat.h"



BEGIN_EVENT_TABLE(HTMLButcherMaskAreaFileAlternateDialog, ButcherControl_Dialog)
    EVT_BUTTON(ID_BTN_LOADPREVIEW, HTMLButcherMaskAreaFileAlternateDialog::OnLoadPreview)
END_EVENT_TABLE()

HTMLButcherMaskAreaFileAlternateDialog::HTMLButcherMaskAreaFileAlternateDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style),
    view_(NULL), area_(NULL), config_(NULL), altimageinfo_()
{
    Init();
    CreateControls();
}

void HTMLButcherMaskAreaFileAlternateDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-alternatefiles.html"));
#endif
}



void HTMLButcherMaskAreaFileAlternateDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);

/*
    // PREVIEW
    wxBoxSizer *previewsizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(previewsizer, 2, wxEXPAND|wxALL, 3);

    // preview image
    //wxStaticText *previewlabel = new wxStaticText(this, wxID_STATIC, _("Area:"), wxDefaultPosition, wxDefaultSize, 0);
    //previewsizer->Add(previewlabel, 0, wxALIGN_LEFT|wxALL, 3);

    previewctrl_ = new ButcherImageView(this, wxID_ANY, wxDefaultPosition, wxSize(200, 200));

    previewsizer->Add(previewctrl_, 1, wxEXPAND|wxALL, 3);
*/

    // FIELDS

    //wxBoxSizer *alternatescrsizer = new wxBoxSizer(wxVERTICAL);
    //bodysizer->Add(alternatescrsizer, 0, wxGROW|wxALL, 3);

    wxScrolledWindow *alternatescroll = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxVSCROLL);
    //alternatescroll->SetVirtualSizeHints(400, 400);
    alternatescroll->SetMaxSize(wxSize(-1, 400));
    alternatescroll->SetVirtualSize(400, 400);
    bodysizer->Add(alternatescroll, 0, wxGROW|wxALL, 3);

    wxGridSizer *alternatesizer = new wxGridSizer(2);
    alternatescroll->SetSizer(alternatesizer);

    int altidadd;
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
    {
        altidadd=(ID_ALTERNATE_END-ID_ALTERNATE_BEGIN)*i;

        wxPanel *ialtpanel=new wxPanel(alternatescroll, wxID_ANY);
        alternatesizer->Add(ialtpanel, 0, wxGROW|wxALL, 3);

        wxStaticBoxSizer *ialtsizer=new wxStaticBoxSizer(wxHORIZONTAL, ialtpanel, wxString::Format(_("Alternate File &%d"), i+1));
        //alternatesizer->Add(ialtsizer, 0, wxGROW|wxALL, 3);
        ialtpanel->SetSizer(ialtsizer);

        // PREVIEW
        wxBoxSizer *previewsizer = new wxBoxSizer(wxVERTICAL);
        ialtsizer->Add(previewsizer, 0, wxEXPAND|wxALL, 3);

        // preview image
        ButcherImageView *previewctrl = new ButcherImageView(ialtpanel, ID_ALTERNATE_PREVIEW+altidadd, wxDefaultPosition, wxSize(100, 100));
        previewctrl->SetEmptyTitle(_("Unavailable"));
        previewsizer->Add(previewctrl, 1, wxEXPAND|wxALL, 3);


        // FIELDS
        wxBoxSizer *ialtfieldssizer=new wxBoxSizer(wxVERTICAL);
        ialtsizer->Add(ialtfieldssizer, 0, wxEXPAND|wxALL);

        // enabled sizer
        wxBoxSizer *enabledsizer = new wxBoxSizer(wxHORIZONTAL);
        ialtfieldssizer->Add(enabledsizer, 0, wxGROW|wxALL, 3);

        // enabled
        wxCheckBox *ialtenabledctrl = new wxCheckBox(ialtpanel, ID_ALTERNATE_ENABLED+altidadd, _("Enabled"), wxDefaultPosition, wxDefaultSize);
        ialtenabledctrl->Connect(ID_ALTERNATE_ENABLED+altidadd, wxEVT_COMMAND_CHECKBOX_CLICKED,
            wxCommandEventHandler(HTMLButcherMaskAreaFileAlternateDialog::OnEnabledClick), NULL, this);
        enabledsizer->Add(ialtenabledctrl, 0, wxGROW|wxALL, 3);

        // image format button
        wxButton *ialtformatbtn = new wxButton(ialtpanel, ID_ALTERNATE_IMAGEFORMATBUTTON+altidadd, _("Format"), wxDefaultPosition, wxDefaultSize);
        ialtformatbtn->Connect(ID_ALTERNATE_IMAGEFORMATBUTTON+altidadd, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(HTMLButcherMaskAreaFileAlternateDialog::OnAlternateImageFormat), NULL, this);
        enabledsizer->Add(ialtformatbtn, 0, wxGROW|wxALL|wxALIGN_RIGHT, 3);

        // image format
        ialtfieldssizer->Add(
            new wxStaticText(ialtpanel, wxID_STATIC, _("Image Format:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxComboBox *ialtformatctrl=new wxComboBox(ialtpanel, ID_ALTERNATE_IMAGEFORMAT+altidadd, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxArrayString(), wxCB_READONLY);

        ialtfieldssizer->Add(ialtformatctrl, 0, wxGROW|wxALL, 3);

        // Filename
        ialtfieldssizer->Add(
            new wxStaticText(ialtpanel, wxID_STATIC, _("Base Filename: (without extension)"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *ialtfilenamectrl = new wxTextCtrl(ialtpanel, ID_ALTERNATE_FILENAME+altidadd, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
        ialtfieldssizer->Add(ialtfilenamectrl, 0, wxGROW|wxALL, 3);

        // filename prepend
        wxCheckBox *ialtfilenameprependctrl = new wxCheckBox(ialtpanel, ID_ALTERNATE_FILENAMEPREPEND+altidadd, _("Prepend parent area filename"), wxDefaultPosition, wxDefaultSize);
        ialtfieldssizer->Add(ialtfilenameprependctrl, 0, wxGROW|wxALL, 3);

        ialtsizer->SetSizeHints(ialtpanel);
    }

    alternatescroll->SetScrollRate(0, 10);
    //alternatescroll->Layout();
    alternatesizer->SetSizeHints(alternatescroll);


    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // load preview button
    wxButton* loadpreviewbtn = new wxButton ( this, ID_BTN_LOADPREVIEW, _("Load &Preview"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(loadpreviewbtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

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

void HTMLButcherMaskAreaFileAlternateDialog::Load(ButcherProjectView *view, ButcherProjectArea *loadarea,
    ButcherProjectAreaConfig *loadconfig)
{
    view_=view;
    area_=loadarea;
    config_=loadconfig;

    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
    {
        altimageinfo_.push_back(std::shared_ptr<ButcherProjectAreaImageInfo>(new ButcherProjectAreaImageInfo(loadconfig->AlternateImageInfo(i))));
        altimageinfo_[i]->ResetAreaConfig();
    }
}



void HTMLButcherMaskAreaFileAlternateDialog::Save(ButcherProjectView *view,
    ButcherProjectArea *loadarea, ButcherProjectAreaConfig *loadconfig)
{
    ButcherProjectBaseAutoUpdate upd(loadarea);

    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
    {
        loadconfig->AlternateImageInfo(i).SetEnabled(altimageinfo_[i]->GetEnabled());
        loadconfig->AlternateImageInfo(i).SetImageFormatId(altimageinfo_[i]->GetImageFormatId());
        loadconfig->AlternateImageInfo(i).SetFilename(altimageinfo_[i]->GetFilename());
        loadconfig->AlternateImageInfo(i).SetFilenamePrepend(altimageinfo_[i]->GetFilenamePrepend());
        loadconfig->AlternateImageInfo(i).TransparentColorsModify()=altimageinfo_[i]->TransparentColors();
    }
}

bool HTMLButcherMaskAreaFileAlternateDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    imageformats_load();

    int altidadd;
    if (altimageinfo_.size()>0)
        for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        {
            altidadd=(ID_ALTERNATE_END-ID_ALTERNATE_BEGIN)*i;

            ButcherImageView *previewctrl=(ButcherImageView*)FindWindow(ID_ALTERNATE_PREVIEW+altidadd);
            wxCheckBox *altenabledctrl=(wxCheckBox*)FindWindow(ID_ALTERNATE_ENABLED+altidadd);
            wxTextCtrl *altfilenamectrl=(wxTextCtrl*)FindWindow(ID_ALTERNATE_FILENAME+altidadd);
            wxCheckBox *altfilenameprependctrl=(wxCheckBox*)FindWindow(ID_ALTERNATE_FILENAMEPREPEND+altidadd);

            if (view_->GetFileAlternateId(i)>0)
                previewctrl->SetEmptyTitle(_("Preview"));

            altenabledctrl->SetValue(altimageinfo_[i]->GetEnabled());
            altfilenamectrl->SetValue(altimageinfo_[i]->GetFilename());
            altfilenameprependctrl->SetValue(altimageinfo_[i]->GetFilenamePrepend());
        }

/*
    // preview
    previewctrl_->SetImage(
        view_->GetFile()->GetImage()->SubImage(area_->GetGlobalRect())
    );
*/

    ProcessEnabled();

    return true;
}

bool HTMLButcherMaskAreaFileAlternateDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    int altidadd;
    if (altimageinfo_.size()>0)
        for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        {
            altidadd=(ID_ALTERNATE_END-ID_ALTERNATE_BEGIN)*i;

            wxCheckBox *altenabledctrl=(wxCheckBox*)FindWindow(ID_ALTERNATE_ENABLED+altidadd);
            wxComboBox *altformatctrl=(wxComboBox*)FindWindow(ID_ALTERNATE_IMAGEFORMAT+altidadd);
            wxTextCtrl *altfilenamectrl=(wxTextCtrl*)FindWindow(ID_ALTERNATE_FILENAME+altidadd);
            wxCheckBox *altfilenameprependctrl=(wxCheckBox*)FindWindow(ID_ALTERNATE_FILENAMEPREPEND+altidadd);

            altimageinfo_[i]->SetEnabled(altenabledctrl->GetValue());
            if (altformatctrl->GetSelection()!=wxNOT_FOUND)
                altimageinfo_[i]->SetImageFormatId(static_cast<ButcherListIdClientData*>(altformatctrl->GetClientObject(altformatctrl->GetSelection()))->GetId());
            altimageinfo_[i]->SetFilename(altfilenamectrl->GetValue());
            altimageinfo_[i]->SetFilenamePrepend(altfilenameprependctrl->GetValue());
        }

    return true;
}

void HTMLButcherMaskAreaFileAlternateDialog::imageformats_load(int afileid)
{
    wxComboBox *altformatctrl;

    int newi;
	for (ButcherProjectImageFormats::iterator i=view_->GetProject()->ImageFormats().begin(); 
		i!=view_->GetProject()->ImageFormats().end(); i++)
	{
        int altidadd;
        for (int alti=0; alti<BUTCHERCONST_VIEW_MAXALTERNATE; alti++)
        {
            if (afileid!=-1 && afileid!=alti) continue;

            altidadd=(ID_ALTERNATE_END-ID_ALTERNATE_BEGIN)*alti;
            altformatctrl=(wxComboBox*)FindWindow(ID_ALTERNATE_IMAGEFORMAT+altidadd);

            if (altformatctrl->GetCount()==0)
                altformatctrl->Append(wxEmptyString,
                    static_cast<wxClientData *>(new ButcherListIdClientData(0)));

            newi=altformatctrl->Append(i->GetName(),
                static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
            if (altimageinfo_.size()>0 && i.first()==altimageinfo_[alti]->GetImageFormatId())
                altformatctrl->SetSelection(newi);
        }
	}
}

void HTMLButcherMaskAreaFileAlternateDialog::OnAlternateImageFormat(wxCommandEvent &event)
{
    int aid=(event.GetId()-ID_ALTERNATE_ENABLED)/(ID_ALTERNATE_END-ID_ALTERNATE_BEGIN);

    // load current image format
    int altidadd=(ID_ALTERNATE_END-ID_ALTERNATE_BEGIN)*aid;
    wxComboBox *altformatctrl=(wxComboBox*)FindWindow(ID_ALTERNATE_IMAGEFORMAT+altidadd);
    if (altformatctrl->GetSelection()!=wxNOT_FOUND)
        altimageinfo_[aid]->SetImageFormatId(static_cast<ButcherListIdClientData*>(altformatctrl->GetClientObject(altformatctrl->GetSelection()))->GetId());

    // dialog
    HTMLButcherMaskAreaImageFormatDialog d(this, wxID_ANY, GetProject());
    d.SetArea(view_->GetFileAlternate(aid), area_);
    d.Load(&*altimageinfo_[aid]);
    if (d.ShowModal()==wxID_OK)
    {
        d.Save(&*altimageinfo_[aid]);
        imageformats_load(aid);
    }
}

void HTMLButcherMaskAreaFileAlternateDialog::OnEnabledClick(wxCommandEvent &event)
{
    ProcessEnabled();
    event.Skip();
}

void HTMLButcherMaskAreaFileAlternateDialog::ProcessEnabled()
{
    int altidadd;
    if (altimageinfo_.size()>0)
        for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        {
            altidadd=(ID_ALTERNATE_END-ID_ALTERNATE_BEGIN)*i;

            wxCheckBox *altenabledctrl=(wxCheckBox*)FindWindow(ID_ALTERNATE_ENABLED+altidadd);
            wxComboBox *altimageformatctrl=(wxComboBox*)FindWindow(ID_ALTERNATE_IMAGEFORMAT+altidadd);
            wxTextCtrl *altfilenamectrl=(wxTextCtrl*)FindWindow(ID_ALTERNATE_FILENAME+altidadd);
            wxCheckBox *altfilenameprependctrl=(wxCheckBox*)FindWindow(ID_ALTERNATE_FILENAMEPREPEND+altidadd);
            wxButton *altformatbtn=(wxButton*)FindWindow(ID_ALTERNATE_IMAGEFORMATBUTTON+altidadd);

            altimageformatctrl->Enable(altenabledctrl->GetValue());
            altfilenamectrl->Enable(altenabledctrl->GetValue());
            altfilenameprependctrl->Enable(altenabledctrl->GetValue());
            altformatbtn->Enable(view_->GetFileAlternate(i) && altenabledctrl->GetValue());
        }
}



void HTMLButcherMaskAreaFileAlternateDialog::OnLoadPreview(wxCommandEvent &event)
{
    ButcherProjectBaseAutoProgress prog(GetProject(), _("Loading preview, please wait"), ButcherProject::UF_GROUP);

    int altidadd;
    if (altimageinfo_.size()>0)
        for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        {
            altidadd=(ID_ALTERNATE_END-ID_ALTERNATE_BEGIN)*i;

            ButcherImageView *previewctrl=(ButcherImageView*)FindWindow(ID_ALTERNATE_PREVIEW+altidadd);

            if (view_->GetFileAlternateId(i)>0)
                previewctrl->SetImage(view_->CreateAreaImageDefault(area_, i));
        }
    FindWindow(ID_BTN_LOADPREVIEW)->Enable(false);
}


