//=============================================================================
/**
 *  @file    DialogMaskAreaMapEdit.cpp
 *
 *  $Id: DialogMaskAreaMapEdit.cpp,v 1.8 2008/02/07 12:35:34 rreale Exp $
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

#include "DialogMaskAreaMapEdit.h"
#include "BList.h"



BEGIN_EVENT_TABLE(HTMLButcherMaskAreaMapEditDialog, ButcherControl_Dialog)
END_EVENT_TABLE()




HTMLButcherMaskAreaMapEditDialog::HTMLButcherMaskAreaMapEditDialog( wxWindow* parent,
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




HTMLButcherMaskAreaMapEditDialog::~HTMLButcherMaskAreaMapEditDialog()
{

}





void HTMLButcherMaskAreaMapEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-area.html"));
#endif

	imageurl_=wxEmptyString;
    imageurltagappend_=wxEmptyString;
    imagetagappend_=wxEmptyString;
}




void HTMLButcherMaskAreaMapEditDialog::CreateControls()
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


    // FIELDS 2
    wxBoxSizer *fields2sizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(fields2sizer, 0, wxEXPAND|wxALL, 3);

    // Image URL
    wxStaticText *imageurllabel = new wxStaticText(this, wxID_STATIC, _("Map &URL: (e.g.: http://www.htmlbutcher.com)"), wxDefaultPosition, wxDefaultSize, 0);
    fields2sizer->Add(imageurllabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *imageurlctrl = new wxTextCtrl(this, ID_IMAGEURL, wxEmptyString, wxDefaultPosition, wxSize(-1, -1),
        0);
    fields2sizer->Add(imageurlctrl , 0, wxGROW|wxALL, 3);

    // Image URL Tag Append
    wxStaticText *imageurltagappendlabel = new wxStaticText(this, wxID_STATIC, _("Map &Area Tag Append: (<AREA> attributes)"), wxDefaultPosition, wxDefaultSize, 0);
    fields2sizer->Add(imageurltagappendlabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *imageurltagappendctrl = new wxTextCtrl(this, ID_IMAGEURLTAGAPPEND, wxEmptyString, wxDefaultPosition, wxSize(-1, -1),
        0);
    fields2sizer->Add(imageurltagappendctrl , 0, wxGROW|wxALL, 3);

    // Image ALT
    wxStaticText *imagetagappendlabel = new wxStaticText(this, wxID_STATIC, _("Image AL&T: (ALT attribute for <AREA> tag)"), wxDefaultPosition, wxDefaultSize, 0);
    fields2sizer->Add(imagetagappendlabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *imagetagappendctrl = new wxTextCtrl(this, ID_IMAGETAGAPPEND, wxEmptyString, wxDefaultPosition, wxSize(-1, -1),
        0);
    fields2sizer->Add(imagetagappendctrl , 0, wxGROW|wxALL, 3);



    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

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
    FindWindow(ID_IMAGEURL)->SetValidator(wxTextValidator(wxFILTER_NONE, &imageurl_));
    FindWindow(ID_IMAGEURLTAGAPPEND)->SetValidator(wxTextValidator(wxFILTER_NONE, &imageurltagappend_));
    FindWindow(ID_IMAGETAGAPPEND)->SetValidator(wxTextValidator(wxFILTER_NONE, &imagetagappend_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}




void HTMLButcherMaskAreaMapEditDialog::Load(ButcherProjectArea *loadarea, ButcherProjectAreaConfigBase *loadconfig)
{
    imageurl_=loadconfig->GetImageURL();
    imageurltagappend_=loadconfig->GetImageURLTagAppend();
    imagetagappend_=loadconfig->GetImageTagAppend();
}




void HTMLButcherMaskAreaMapEditDialog::Save(ButcherProjectArea *loadarea, ButcherProjectAreaConfigBase *loadconfig)
{
    ButcherProjectBaseAutoUpdate upd(loadarea);

    loadconfig->SetImageURL(imageurl_);
    loadconfig->SetImageURLTagAppend(imageurltagappend_);
    loadconfig->SetImageTagAppend(imagetagappend_);
}




bool HTMLButcherMaskAreaMapEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;
    return true;
}



bool HTMLButcherMaskAreaMapEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;
    return true;
}


