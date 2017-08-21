//=============================================================================
/**
 *  @file    WizNewView.cpp
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-10-16
 */
//=============================================================================
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <memory>

#include <wx/statline.h>
#include <wx/xrc/xmlres.h>
#include <wx/filepicker.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/filename.h>
#include <cppcomp/wxccucontrol.h>

#include "WizNewView.h"
#include "BImage.h"



/////////////////////////////////
// CLASS
//      HTMLButcherNewViewWizard
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherNewViewWizard, wxWizard)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY, HTMLButcherNewViewWizard::OnWizardChanging)
	EVT_WIZARD_FINISHED(wxID_ANY, HTMLButcherNewViewWizard::OnWizardFinished)
END_EVENT_TABLE()

HTMLButcherNewViewWizard::HTMLButcherNewViewWizard( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    //wxWizard(parent, id, caption, wxNullBitmap, pos, style), 
	project_(project), viewid_(0)
{
    wxLogNull lnull;
	wxBitmap rbitmap(wxXmlResource::Get()->LoadBitmap(wxT("img_wizard")));

	Create(parent, id, caption, rbitmap, pos, style);

	Init();
    CreateControls();
}

void HTMLButcherNewViewWizard::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	//SetHelpSection(wxT("wizard-newfile.html"));
#endif
}

void HTMLButcherNewViewWizard::CreateControls()
{
	intro_ = new HTMLButcherNewViewIntroWizardPage(this);

	file_ = new HTMLButcherNewViewFileWizardPage(this);
	mask_ = new HTMLButcherNewViewMaskWizardPage(this);
	view_ = new HTMLButcherNewViewViewWizardPage(this);
	last_ = new HTMLButcherNewViewLastWizardPage(this);

	wxWizardPageSimple::Chain(intro_, file_);
	wxWizardPageSimple::Chain(file_, mask_);
	wxWizardPageSimple::Chain(mask_, view_);
	wxWizardPageSimple::Chain(view_, last_);

	GetPageAreaSizer()->Add(intro_);
}

wxWizardPage *HTMLButcherNewViewWizard::GetFirstPage() const
{
	return intro_;
}

void HTMLButcherNewViewWizard::OnWizardFinished(wxWizardEvent& event)
{
	// add file
	BLID_t fileid=project_->Files().Add(wxEmptyString, file_->GetFilename());

	// add mask
	BLID_t maskid;
		
	if (mask_->IsNew())
		maskid=project_->Masks().Add(mask_->GetMaskName(), mask_->GetMaskWidth(), mask_->GetMaskHeight());
	else
		maskid=mask_->GetMaskId();

	// add view
	viewid_=project_->Views().Add(view_->GetViewName(), fileid, maskid);
	project_->Views()[viewid_]->SetFilename(view_->GetFileName());
}

void HTMLButcherNewViewWizard::OnWizardChanging(wxWizardEvent& event)
{
	if (event.GetPage()==file_)
		mask_->SetInfo(file_->GetFilename(), file_->GetImageWidth(), file_->GetImageHeight());
	else if (event.GetPage()==mask_)
		view_->SetInfo(file_->GetFilename());
	else if (event.GetPage()==view_)
		last_->SetInfo(file_->GetFilename(), 
		mask_->GetMaskName(), 
		mask_->GetMaskWidth(), mask_->GetMaskHeight(),
		view_->GetViewName(), view_->GetFileName());
}


/////////////////////////////////
// CLASS
//      HTMLButcherNewViewIntroWizardPage
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherNewViewIntroWizardPage, wxWizardPageSimple)
END_EVENT_TABLE()

HTMLButcherNewViewIntroWizardPage::HTMLButcherNewViewIntroWizardPage(wxWizard *parent) :
	wxWizardPageSimple(parent)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	// TITLE
    wxStaticText *titlectrl = new wxStaticText(this, wxID_ANY, _("Welcome"));
	wxFont tfont(titlectrl->GetFont());
	tfont.SetPointSize(14);
	titlectrl->SetFont(tfont);

	mainSizer->Add(
        titlectrl,
        0,
        wxALL,
        5
    );

    // divider line
    wxStaticLine *line2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    mainSizer->Add(line2, 0, wxGROW|wxALL, 3);

    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
                         wxString(_("Welcome to the New view wizard!"))+
						 wxT("\n\n")+
						 wxString(_("This wizard will help on the creation of a new VIEW (HTML page)."))+
						 wxT("\n\n")+
						 wxString(_("To continue, press Next."))+
						 wxT("\n")),
        0,
        wxALL,
        5
    );

    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

/////////////////////////////////
// CLASS
//      HTMLButcherNewViewFileWizardPage
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherNewViewFileWizardPage, wxWizardPageSimple)
END_EVENT_TABLE()

HTMLButcherNewViewFileWizardPage::HTMLButcherNewViewFileWizardPage(wxWizard *parent) :
	wxWizardPageSimple(parent)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	// TITLE
    wxStaticText *titlectrl = new wxStaticText(this, wxID_ANY, _("Layout Image File"));
	wxFont tfont(titlectrl->GetFont());
	tfont.SetPointSize(14);
	titlectrl->SetFont(tfont);

	mainSizer->Add(
        titlectrl,
        0,
        wxALL,
        5
    );

    // divider line
    wxStaticLine *line2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    mainSizer->Add(line2, 0, wxGROW|wxALL, 3);

    // FILE
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
			_("Select image file:")),
        0,
        wxALL,
        5
    );
	wxFilePickerCtrl *filectrl = new wxFilePickerCtrl(this, ID_FILE, wxEmptyString, _("Select a file"),
        _("All files (*.*)|*.*"), wxDefaultPosition, wxSize(200, -1), wxFLP_OPEN|wxFLP_USE_TEXTCTRL);
	mainSizer->Add(filectrl, 0, wxGROW|wxALL, 3);

    // DESCRIPTION
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
			wxString(_("The layout image FILE is the image made by the webdesigner"))+
			wxT("\n")+
			wxString(_("that will be sliced to generate the HTML page."))+
			wxT("\n")),
        0,
        wxALL,
        5
    );


    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

bool HTMLButcherNewViewFileWizardPage::TransferDataFromWindow()
{
    wxFilePickerCtrl *filectrl=(wxFilePickerCtrl *)FindWindow(ID_FILE);

	if (filectrl->GetPath().IsEmpty())
    {
        wxMessageBox(_("The layout image file is required."), _("Error"),
                     wxICON_ERROR | wxOK, this);

        return false;
    }

	wxFileName fn(filectrl->GetPath());

	if ( !fn.FileExists() || !fn.IsFileReadable() )
    {
        wxMessageBox(_("File not found."), _("Error"),
                     wxICON_ERROR | wxOK, this);

        return false;
    }

	std::unique_ptr<ButcherImage> limg(ButcherImageFactory::Load(fn.GetFullPath()));
	if (!limg.get())
    {
        wxMessageBox(_("Unknown image file format."), _("Error"),
                     wxICON_ERROR | wxOK, this);

        return false;
    }

	filename_=filectrl->GetPath();
	imagewidth_=limg->GetWidth();
	imageheight_=limg->GetHeight();

    return true;
}


/////////////////////////////////
// CLASS
//      HTMLButcherNewViewMaskWizardPage
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherNewViewMaskWizardPage, wxWizardPageSimple)
	EVT_RADIOBUTTON(wxID_ANY, HTMLButcherNewViewMaskWizardPage::OnRadio)
END_EVENT_TABLE()

HTMLButcherNewViewMaskWizardPage::HTMLButcherNewViewMaskWizardPage(wxWizard *parent) :
	wxWizardPageSimple(parent), autofilename_(wxEmptyString), autoimagewidth_(-1), autoimageheight_(-1)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	// TITLE
    wxStaticText *titlectrl = new wxStaticText(this, wxID_ANY, _("Layout Mask"));
	wxFont tfont(titlectrl->GetFont());
	tfont.SetPointSize(14);
	titlectrl->SetFont(tfont);

	mainSizer->Add(
        titlectrl,
        0,
        wxALL,
        5
    );

    // divider line
    wxStaticLine *line2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    mainSizer->Add(line2, 0, wxGROW|wxALL, 3);




	// NEW MASK
	wxRadioButton *masknewctrl = new wxRadioButton(this, ID_ISNEW, _("Create new mask"));
	mainSizer->Add(masknewctrl, 0, wxGROW|wxALL, 3);

    // NAME
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
			_("Mask name:")),
        0,
        wxALL,
        5
    );
	wxTextCtrl *namectrl = new wxTextCtrl(this, ID_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	mainSizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // SIZE
    wxBoxSizer *sizesizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(sizesizer, 0, wxGROW|wxALL, 3);


    // width
    wxStaticText *widthlabel = new wxStaticText(this, wxID_STATIC, _("Width:"), wxDefaultPosition, wxDefaultSize, 0);
    sizesizer->Add(widthlabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *widthctrl = new wxSpinCtrl(this, ID_WIDTH, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 1, 10000, 1);
    sizesizer->Add(widthctrl, wxALIGN_CENTER_VERTICAL|wxALL, 3);

	sizesizer->AddSpacer(40);

    // height
    wxStaticText *heightlabel = new wxStaticText(this, wxID_STATIC, _("Height:"), wxDefaultPosition, wxDefaultSize, 0);
    sizesizer->Add(heightlabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *heightctrl = new wxSpinCtrl(this, ID_HEIGHT, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 1, 10000, 1);
    sizesizer->Add(heightctrl, wxALIGN_CENTER_VERTICAL|wxALL, 3);



	// EXISTING MASK
	wxRadioButton *maskexistingctrl = new wxRadioButton(this, ID_ISEXISTING, _("Use existing mask"));
	mainSizer->Add(maskexistingctrl, 0, wxGROW|wxALL, 3);

    // mask
	wxComboBox *maskctrl = new wxComboBox(this, ID_MASK, wxEmptyString, wxDefaultPosition, wxSize(300, -1),
        wxArrayString(), wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT);
    mainSizer->Add(maskctrl, 0, wxGROW|wxALL, 3);

	maskctrl->Enable(false);


    // DESCRIPTION
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
			wxString(_("The MASK will define the areas of the HTML slicing."))+
			wxT("\n\n")+
			wxString(_("It could be defined as the real output page slices,"))+
			wxT("\n")+
			wxString(_("but to allow reuse, a MASK was separately created from"))+
			wxT("\n")+
			wxString(_("the output page (the VIEW)."))+
			wxT("\n\n")+
			wxString(_("Normally the MASK should have the same size as the layout IMAGE,"))+
			wxT("\n")+
			wxString(_("but if the layout images aren't all the same size, the mask"))+
			wxT("\n")+
			wxString(_("should be the size of the largest one."))+
			wxT("\n")),
        0,
        wxALL,
        5
    );

    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

void HTMLButcherNewViewMaskWizardPage::SetInfo(const wxString &filename, int imagewidth, int imageheight)
{
	wxTextCtrl *namectrl=(wxTextCtrl*)FindWindow(ID_NAME);
	wxSpinCtrl *widthctrl=(wxSpinCtrl*)FindWindow(ID_WIDTH);
	wxSpinCtrl *heightctrl=(wxSpinCtrl*)FindWindow(ID_HEIGHT);

	wxFileName lastfn(autofilename_);
	wxString lastname(lastfn.GetName().Upper());

	if (namectrl->GetValue().IsEmpty() || namectrl->GetValue()==lastname)
	{
		wxFileName fn(filename);
		namectrl->SetValue(fn.GetName().Upper());
	}
	if (widthctrl->GetValue()==1 || widthctrl->GetValue()==autoimagewidth_)
		widthctrl->SetValue(imagewidth);
	if (heightctrl->GetValue()==1 || heightctrl->GetValue()==autoimageheight_)
		heightctrl->SetValue(imageheight);

	autofilename_=filename;
	autoimagewidth_=imagewidth;
	autoimageheight_=imageheight;
}

bool HTMLButcherNewViewMaskWizardPage::TransferDataToWindow()
{
    wxComboBox *maskctrl = (wxComboBox*) FindWindow(ID_MASK);
	wxRadioButton *isexistingctrl = (wxRadioButton *) FindWindow(ID_ISEXISTING);

    // MASKS
	ButcherProject *project=dynamic_cast<HTMLButcherNewViewWizard*>(GetParent())->GetProject();
	
	maskctrl->Clear();
	if (project->Masks().Count()>0)
	{
		for (ButcherProjectMasks::iterator i=project->Masks().begin(); i!=project->Masks().end(); i++)
			wxccu_control_append(maskctrl, i->GetName(), i.first());
		
		if (maskctrl->GetCount()>0)
			maskctrl->SetSelection(0);
	} else {
		maskctrl->Enable(false);
		isexistingctrl->Enable(false);
	}

	return wxWizardPageSimple::TransferDataToWindow();
}

bool HTMLButcherNewViewMaskWizardPage::TransferDataFromWindow()
{
	wxTextCtrl *namectrl=(wxTextCtrl*)FindWindow(ID_NAME);
	wxSpinCtrl *widthctrl=(wxSpinCtrl*)FindWindow(ID_WIDTH);
	wxSpinCtrl *heightctrl=(wxSpinCtrl*)FindWindow(ID_HEIGHT);
    wxComboBox *maskctrl = (wxComboBox*) FindWindow(ID_MASK);

	wxRadioButton *isnewctrl = (wxRadioButton *) FindWindow(ID_ISNEW);

	if (isnewctrl->GetValue())
	{
		if (namectrl->GetValue().IsEmpty())
		{
			wxMessageBox(_("The mask name is required."), _("Error"),
						 wxICON_ERROR | wxOK, this);

			return false;
		}

		isnew_=true;
		maskname_=namectrl->GetValue();
		maskwidth_=widthctrl->GetValue();
		maskheight_=heightctrl->GetValue();
	} else {
		isnew_=false;
		maskid_=wxccu_control_getselectedid(maskctrl, 0);

		ButcherProject *project=dynamic_cast<HTMLButcherNewViewWizard*>(GetParent())->GetProject();

		maskname_=project->Masks()[maskid_]->GetName();
		maskwidth_=project->Masks()[maskid_]->GetWidth();
		maskheight_=project->Masks()[maskid_]->GetHeight();
	}

	return wxWizardPageSimple::TransferDataFromWindow();
}

void HTMLButcherNewViewMaskWizardPage::OnRadio(wxCommandEvent &event)
{
	wxTextCtrl *namectrl=(wxTextCtrl*)FindWindow(ID_NAME);
	wxSpinCtrl *widthctrl=(wxSpinCtrl*)FindWindow(ID_WIDTH);
	wxSpinCtrl *heightctrl=(wxSpinCtrl*)FindWindow(ID_HEIGHT);
    wxComboBox *maskctrl = (wxComboBox*) FindWindow(ID_MASK);

	namectrl->Enable(event.GetId()==ID_ISNEW);
	widthctrl->Enable(event.GetId()==ID_ISNEW);
	heightctrl->Enable(event.GetId()==ID_ISNEW);
	maskctrl->Enable(event.GetId()!=ID_ISNEW);
}

/////////////////////////////////
// CLASS
//      HTMLButcherNewViewViewWizardPage
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherNewViewViewWizardPage, wxWizardPageSimple)
END_EVENT_TABLE()

HTMLButcherNewViewViewWizardPage::HTMLButcherNewViewViewWizardPage(wxWizard *parent) :
	wxWizardPageSimple(parent), autofilename_(wxEmptyString)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	// TITLE
    wxStaticText *titlectrl = new wxStaticText(this, wxID_ANY, _("View (output page)"));
	wxFont tfont(titlectrl->GetFont());
	tfont.SetPointSize(14);
	titlectrl->SetFont(tfont);

	mainSizer->Add(
        titlectrl,
        0,
        wxALL,
        5
    );

    // divider line
    wxStaticLine *line2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    mainSizer->Add(line2, 0, wxGROW|wxALL, 3);

    // NAME
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
			_("View name:")),
        0,
        wxALL,
        5
    );
	wxTextCtrl *namectrl = new wxTextCtrl(this, ID_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	mainSizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // FILE NAME
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
			_("Output HTML file name:")),
        0,
        wxALL,
        5
    );
	wxTextCtrl *filenamectrl = new wxTextCtrl(this, ID_FILENAME, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	mainSizer->Add(filenamectrl, 0, wxGROW|wxALL, 3);

    // DESCRIPTION
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
			wxString(_("The VIEW is the output HTML page."))+
			wxT("\n\n")+
			wxString(_("It combines an image FILE with a MASK, generating a single"))+
			wxT("\n")+
			wxString(_("output HTML page together with the sliced images."))+
			wxT("\n")),
        0,
        wxALL,
        5
    );

    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

void HTMLButcherNewViewViewWizardPage::SetInfo(const wxString &filename)
{
	wxTextCtrl *namectrl=(wxTextCtrl*)FindWindow(ID_NAME);
	wxTextCtrl *filenamectrl=(wxTextCtrl*)FindWindow(ID_FILENAME);

	wxFileName lastfn(autofilename_);
	wxString lastname(lastfn.GetName().Upper());
	if (namectrl->GetValue().IsEmpty() || namectrl->GetValue()==lastname)
	{
		wxFileName fn(filename);
		namectrl->SetValue(fn.GetName().Upper());
	}
	wxString lastfilename(lastfn.GetName().Lower()+wxT(".html"));
	if (filenamectrl->GetValue().IsEmpty() || filenamectrl->GetValue()==lastfilename)
	{
		wxFileName fn(filename);
		filenamectrl->SetValue(fn.GetName().Lower()+wxT(".html"));
	}

	autofilename_=filename;
}

bool HTMLButcherNewViewViewWizardPage::TransferDataFromWindow()
{
	wxTextCtrl *namectrl=(wxTextCtrl*)FindWindow(ID_NAME);
	wxTextCtrl *filenamectrl=(wxTextCtrl*)FindWindow(ID_FILENAME);

	if (namectrl->GetValue().IsEmpty())
    {
        wxMessageBox(_("The view name is required."), _("Error"),
                     wxICON_ERROR | wxOK, this);

        return false;
    }

	if (filenamectrl->GetValue().IsEmpty())
    {
        wxMessageBox(_("The file name is required."), _("Error"),
                     wxICON_ERROR | wxOK, this);

        return false;
    }

	viewname_=namectrl->GetValue();
	filename_=filenamectrl->GetValue();

    return true;
}

/////////////////////////////////
// CLASS
//      HTMLButcherNewViewLastWizardPage
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherNewViewLastWizardPage, wxWizardPageSimple)
END_EVENT_TABLE()

HTMLButcherNewViewLastWizardPage::HTMLButcherNewViewLastWizardPage(wxWizard *parent) :
	wxWizardPageSimple(parent)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	// TITLE
    wxStaticText *titlectrl = new wxStaticText(this, wxID_ANY, _("Confirmation"));
	wxFont tfont(titlectrl->GetFont());
	tfont.SetPointSize(14);
	titlectrl->SetFont(tfont);

	mainSizer->Add(
        titlectrl,
        0,
        wxALL,
        5
    );

    // divider line
    wxStaticLine *line2 = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    mainSizer->Add(line2, 0, wxGROW|wxALL, 3);

    // NAME
    mainSizer->Add(
        new wxStaticText(this, ID_CONFIRMATION, _("Confirmation")),
        0,
        wxALL,
        5
    );

    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

void HTMLButcherNewViewLastWizardPage::SetInfo(const wxString &filename, 
	const wxString &maskname,
	int maskwidth, int maskheight, const wxString &viewname,
	const wxString &viewfilename)
{
	filename_=filename;
	maskname_=maskname;
	maskwidth_=maskwidth;
	maskheight_=maskheight;
	viewname_=viewname;
	viewfilename_=viewfilename;

	wxFileName fn(filename_);

	wxStaticText *confirmctrl=(wxStaticText *)FindWindow(ID_CONFIRMATION);
	confirmctrl->SetLabel(
			wxString::Format(
			wxString(_("Please review your selection, and click Finish if all is correct."))+
			wxT("\n\n")+
			wxString(_("Image file name: %s"))+
			wxT("\n\n")+
			wxString(_("Mask name: %s"))+
			wxT("\n")+
			wxString(_("Mask size: Width = %d   Height = %d"))+
			wxT("\n\n")+
			wxString(_("View name: %s"))+
			wxT("\n")+
			wxString(_("View file name: %s"))+
			wxT("\n"),
			fn.GetFullName().c_str(), maskname_.c_str(), maskwidth_, maskheight_,
			viewname_.c_str(), viewfilename_.c_str())
	);

}


