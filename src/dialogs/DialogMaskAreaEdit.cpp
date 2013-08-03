//=============================================================================
/**
 *  @file    DialogMaskAreaEdit.cpp
 *
 *  $Id: DialogMaskAreaEdit.cpp,v 1.36 2008/02/27 18:53:05 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-12
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

#include "DialogMaskAreaEdit.h"
#include "BList.h"
#include "DialogImageFormatEdit.h"



BEGIN_EVENT_TABLE(HTMLButcherMaskAreaEditDialog, ButcherControl_Dialog)
    EVT_BUTTON(ID_IMAGEFORMATDIALOG, HTMLButcherMaskAreaEditDialog::OnImageFormat)
    EVT_COMBOBOX(ID_KIND, HTMLButcherMaskAreaEditDialog::OnCtrlChanged)
	EVT_RADIOBOX(ID_LAYOUTTYPE, HTMLButcherMaskAreaEditDialog::OnCtrlChanged)
    EVT_CHECKBOX(ID_BACKGROUND, HTMLButcherMaskAreaEditDialog::OnCtrlChanged)
    EVT_CHECKBOX(ID_HAVEIMAGEMAP, HTMLButcherMaskAreaEditDialog::OnCtrlChanged)
    EVT_CHECKBOX(ID_ISBGCOLOR, HTMLButcherMaskAreaEditDialog::OnCtrlChanged)
    EVT_COMBOBOX(ID_FILEALTERNATE, HTMLButcherMaskAreaEditDialog::OnAlternateFileChanged)
END_EVENT_TABLE()



HTMLButcherMaskAreaEditDialog::HTMLButcherMaskAreaEditDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style),
    areaimap_(), alignimap_(), valignimap_(), bgrepeatimap_(), imagesourceimap_(),
    imagelink_(project), loadview_(NULL), loadarea_(NULL), margins_()
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    Init();
    CreateControls();
}

void HTMLButcherMaskAreaEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-area.html"));
#endif
	
	areaname_=wxEmptyString;
    areakind_=ButcherProjectAreaConfig::AK_NONE;
    imageformat_=1;
    background_=false;
	layouttype_=ButcherProjectConsts::LTYPE_DEFAULT;
    haveimagemap_=false;
    imagemapname_=wxEmptyString;
    filename_=wxEmptyString;
    filename_prepend_=true;

    content_=wxEmptyString;
    celltagappend_=wxEmptyString;
    imageurl_=wxEmptyString;
    imageurltagappend_=wxEmptyString;
    imagetagappend_=wxEmptyString;

    align_=ButcherProjectAreaConfig::AA_NONE;
    valign_=ButcherProjectAreaConfig::AV_TOP;
    bgrepeat_=ButcherProjectAreaConfig::BR_ALL;
    isbgcolor_=false;
    bgcolor_=wxColor(wxT("BLACK"));
    tabletagappend_=wxEmptyString;
    innerscrollable_=false;
    variablesize_=false;
    alternatefile_=-1;
    imagesource_=ButcherProjectAreaConfig::IS_AREA;
	margins_.Set(0, 0, 0, 0);

    areaimap_.Add(ButcherProjectAreaConfig::AK_NONE, _("None"));
    areaimap_.Add(ButcherProjectAreaConfig::AK_IMAGE, _("Image"));
    areaimap_.Add(ButcherProjectAreaConfig::AK_MASK, _("Mask"));

    alignimap_.Add(ButcherProjectAreaConfig::AA_NONE, _("None"));
    alignimap_.Add(ButcherProjectAreaConfig::AA_LEFT, _("Left"));
    alignimap_.Add(ButcherProjectAreaConfig::AA_CENTER, _("Center"));
    alignimap_.Add(ButcherProjectAreaConfig::AA_RIGHT, _("Right"));

    valignimap_.Add(ButcherProjectAreaConfig::AV_NONE, _("None"));
    valignimap_.Add(ButcherProjectAreaConfig::AV_TOP, _("Top"));
    valignimap_.Add(ButcherProjectAreaConfig::AV_MIDDLE, _("Middle"));
    valignimap_.Add(ButcherProjectAreaConfig::AV_BOTTOM, _("Bottom"));

    bgrepeatimap_.Add(ButcherProjectAreaConfig::BR_NONE, _("None"));
    bgrepeatimap_.Add(ButcherProjectAreaConfig::BR_HORIZONTAL, _("Horizontal"));
    bgrepeatimap_.Add(ButcherProjectAreaConfig::BR_VERTICAL, _("Vertical"));
    bgrepeatimap_.Add(ButcherProjectAreaConfig::BR_ALL, _("All"));

    imagesourceimap_.Add(ButcherProjectAreaConfig::IS_AREA, _("Default"));
    imagesourceimap_.Add(ButcherProjectAreaConfig::IS_LINK, _("Image Link"));

	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_DEFAULT, _("Default"));
	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_TABLE, _("Table"));
	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_APTABLELESS, _("AP Tableless"));
}



void HTMLButcherMaskAreaEditDialog::CreateControls()
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


    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        previewctrl_->SetSelectColor(true);
        previewctrl_->Connect(wxID_ANY, wxEVT_BUTCHERIMAGEVIEWCOLOR_ACTION,
            ButcherImageViewColorEventHandler(HTMLButcherMaskAreaEditDialog::OnSelectColor), NULL, this);

        // PREVIEW COLOR
        wxBoxSizer *previewcolorsizer = new wxBoxSizer(wxHORIZONTAL);


        // hover color
        previewcolorsizer->Add(
            new wxStaticText(this, wxID_STATIC, _("Hover:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *pchoverctrl = new wxTextCtrl(this, ID_PROC_HOVERCOLOR, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
        previewcolorsizer->Add(pchoverctrl, 0, wxGROW|wxALL, 3);

        // selected color
        previewcolorsizer->Add(
            new wxStaticText(this, wxID_STATIC, _("Selected:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *pcselectedctrl = new wxTextCtrl(this, ID_PROC_SELCOLOR, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
        previewcolorsizer->Add(pcselectedctrl, 0, wxGROW|wxALL, 3);


        previewsizer->Add(previewcolorsizer, 0, wxEXPAND|wxALL, 3);
    }


    //
    // NOTEBOOK
    //

    wxNotebook *notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_NOPAGETHEME);
    bodysizer->Add(notebook, 0, wxEXPAND|wxALL, 3);

    wxPanel *nbfields = new wxPanel(notebook, wxID_ANY);
    wxPanel *nbhtml = NULL;
    wxPanel *nbimage = NULL;

    notebook->AddPage(nbfields, _("Properties"), true);
    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        nbhtml = new wxPanel(notebook, wxID_ANY);
        nbimage = new wxPanel(notebook, wxID_ANY);

        notebook->AddPage(nbhtml, _("HTML"), false);
        notebook->AddPage(nbimage, _("Image"), false);
    }

    // FIELDS BASE
    //wxBoxSizer *fieldsbasesizer = new wxBoxSizer(wxHORIZONTAL);
    //bodysizer->Add(fieldsbasesizer, 0, wxEXPAND|wxALL, 3);

    //*****
    //* PROPERTIES
    //*****


    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
    nbfields->SetSizer(fieldssizer);
    //fieldsbasesizer->Add(fieldssizer, 0, wxEXPAND|wxALL, 3);

    // Name
    wxStaticText *namelabel = new wxStaticText(nbfields, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(namelabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl = new wxTextCtrl(nbfields, ID_NAME, wxEmptyString, wxDefaultPosition, wxSize(120, -1), 0);
    fieldssizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // kind
    wxStaticText *kindlabel = new wxStaticText(nbfields, wxID_STATIC, _("&Kind:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(kindlabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *kindctrl=new wxComboBox(nbfields, ID_KIND, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);
    areaimap_.Load(kindctrl);

    //kindctrl->SetSelection(2);

    fieldssizer->Add(kindctrl, 0, wxGROW|wxALL, 3);

    // background
    wxCheckBox *backgroundctrl = new wxCheckBox(nbfields, ID_BACKGROUND, _("&Background Image"), wxDefaultPosition, wxDefaultSize);
    fieldssizer->Add(backgroundctrl, 0, wxGROW|wxALL, 3);

    // Layout type
	wxRadioBox *layouttypectrl = new wxRadioBox(nbfields, ID_LAYOUTTYPE, _("&Layout type (inner mask)"), wxDefaultPosition, wxSize(300, -1),
        layouttypeimap_.Load(), 0);
	fieldssizer->Add(layouttypectrl, 0, wxGROW|wxALL, 3);

/*
    // inner scrollable
    wxCheckBox *innerscrollablectrl = new wxCheckBox(nbfields, ID_INNERSCROLLABLE, _("Inner &Scrollable (DIV)"), wxDefaultPosition, wxDefaultSize);
    fieldssizer->Add(innerscrollablectrl, 0, wxGROW|wxALL, 3);

    // variable size
    wxCheckBox *variablesizectrl = new wxCheckBox(nbfields, ID_VARIABLESIZE, _("Variable Si&ze"), wxDefaultPosition, wxDefaultSize);
    fieldssizer->Add(variablesizectrl, 0, wxGROW|wxALL, 3);
*/

    // image format
    wxStaticText *formatlabel = new wxStaticText(nbfields, wxID_STATIC, _("&Image Format:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(formatlabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *formatctrl=new wxComboBox(nbfields, ID_IMAGEFORMAT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);

    fieldssizer->Add(formatctrl, 0, wxGROW|wxALL, 3);

    // have image map
    wxCheckBox *imagemapctrl = new wxCheckBox(nbfields, ID_HAVEIMAGEMAP, _("Image &Map"), wxDefaultPosition, wxDefaultSize);
    fieldssizer->Add(imagemapctrl, 0, wxGROW|wxALL, 3);

    // Image map name
    wxStaticText *imagemapnamelabel = new wxStaticText(nbfields, wxID_STATIC, _("Image Ma&p Name:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(imagemapnamelabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *imagemapnamectrl = new wxTextCtrl(nbfields, ID_IMAGEMAPNAME, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        0);
    fieldssizer->Add(imagemapnamectrl , 0, wxGROW|wxALL, 3);

    // Filename
    fieldssizer->Add(
        new wxStaticText(nbfields, wxID_STATIC, _("Base &Filename: (without extension)"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *filenamectrl = new wxTextCtrl(nbfields, ID_FILENAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(filenamectrl, 0, wxGROW|wxALL, 3);

    // filename prepend
    wxCheckBox *filenameprependctrl = new wxCheckBox(nbfields, ID_FILENAMEPREPEND, _("Prepen&d parent area filename"), wxDefaultPosition, wxDefaultSize);
    fieldssizer->Add(filenameprependctrl, 0, wxGROW|wxALL, 3);

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        // alternate file
        fieldssizer->Add(
            new wxStaticText(nbfields, wxID_STATIC, _("Use &Alternate file:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxArrayString altfchoices;
        altfchoices.Add(_("No"));
        for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        {
            altfchoices.Add(wxString::Format(_("Alternate %d"), i+1));
        }

        wxComboBox *altfilectrl = new wxComboBox(nbfields, ID_FILEALTERNATE, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            altfchoices, wxCB_READONLY|wxCB_DROPDOWN);
        fieldssizer->Add(altfilectrl, 0, wxGROW|wxALL, 3);
    }

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        //*****
        //* HTML
        //*****

        wxBoxSizer *fieldshtmlrootsizer = new wxBoxSizer(wxVERTICAL);
        nbhtml->SetSizer(fieldshtmlrootsizer);


        wxBoxSizer *fieldshtmlbasesizer = new wxBoxSizer(wxHORIZONTAL);
        fieldshtmlrootsizer->Add(fieldshtmlbasesizer, 0, wxEXPAND|wxALL);
        //nbhtml->SetSizer(fieldshtmlbasesizer);
        //fieldsbasesizer->Add(fieldssizer, 0, wxEXPAND|wxALL, 3);

        // FIELDS
        wxBoxSizer *fieldshtmlsizer = new wxBoxSizer(wxVERTICAL);
        fieldshtmlbasesizer->Add(fieldshtmlsizer, 0, wxEXPAND|wxALL, 3);

        // align
        fieldshtmlsizer->Add(
            new wxStaticText(nbhtml, wxID_STATIC, _("&Align:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxComboBox *alignctrl=new wxComboBox(nbhtml, ID_ALIGN, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxArrayString(), wxCB_READONLY);
        alignimap_.Load(alignctrl);

        fieldshtmlsizer->Add(alignctrl, 0, wxGROW|wxALL, 3);

        // valign
        fieldshtmlsizer->Add(
            new wxStaticText(nbhtml, wxID_STATIC, _("&Vertical Align:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxComboBox *valignctrl=new wxComboBox(nbhtml, ID_VALIGN, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxArrayString(), wxCB_READONLY);
        valignimap_.Load(valignctrl);

        fieldshtmlsizer->Add(valignctrl, 0, wxGROW|wxALL, 3);

        // bgrepeat
        fieldshtmlsizer->Add(
            new wxStaticText(nbhtml, wxID_STATIC, _("Background &Repeat:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxComboBox *bgrepeatctrl=new wxComboBox(nbhtml, ID_BGREPEAT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxArrayString(), wxCB_READONLY);
        bgrepeatimap_.Load(bgrepeatctrl);

        fieldshtmlsizer->Add(bgrepeatctrl, 0, wxGROW|wxALL, 3);


        // inner scrollable
        wxCheckBox *innerscrollablectrl = new wxCheckBox(nbhtml, ID_INNERSCROLLABLE, _("Inner &Scrollable (DIV)"), wxDefaultPosition, wxDefaultSize);
        fieldshtmlsizer->Add(innerscrollablectrl, 0, wxGROW|wxALL, 3);


        // FIELDS 2
        wxBoxSizer *fields2sizer = new wxBoxSizer(wxVERTICAL);
        fieldshtmlbasesizer->Add(fields2sizer, 0, wxEXPAND|wxALL, 3);

        // Cell Tag Append
        wxStaticText *celltagappendlabel = new wxStaticText(nbhtml, wxID_STATIC, _("Cell &Tag Append: (<TD>/<DIV> attributes)"), wxDefaultPosition, wxDefaultSize, 0);
        fields2sizer->Add(celltagappendlabel, 0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *celltagappendctrl = new wxTextCtrl(nbhtml, ID_CELLTAGAPPEND, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            0);
        fields2sizer->Add(celltagappendctrl , 0, wxGROW|wxALL, 3);

        // Table Tag Append
        wxStaticText *tabletagappendlabel = new wxStaticText(nbhtml, wxID_STATIC, _("Table &Tag Append: (inner mask)"), wxDefaultPosition, wxDefaultSize, 0);
        fields2sizer->Add(tabletagappendlabel, 0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *tabletagappendctrl = new wxTextCtrl(nbhtml, ID_TABLETAGAPPEND, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            0);
        fields2sizer->Add(tabletagappendctrl , 0, wxGROW|wxALL, 3);

        // isbgcolor
        wxCheckBox *isbgcolorctrl = new wxCheckBox(nbhtml, ID_ISBGCOLOR, _("Background Co&lor:"), wxDefaultPosition, wxDefaultSize);
        fields2sizer->Add(isbgcolorctrl, 0, wxGROW|wxALL, 3);

        // bgcolor
        wxColourPickerCtrl *bgcolorctrl = new wxColourPickerCtrl(nbhtml, ID_BGCOLOR, *wxBLACK, wxDefaultPosition, wxDefaultSize,
            wxCLRP_SHOW_LABEL|wxCLRP_USE_TEXTCTRL);
        fields2sizer->Add(bgcolorctrl , 0, wxGROW|wxALL, 3);


        // variable size
        wxCheckBox *variablesizectrl = new wxCheckBox(nbhtml, ID_VARIABLESIZE, _("Variable Si&ze"), wxDefaultPosition, wxDefaultSize);
        fields2sizer->Add(variablesizectrl, 0, wxGROW|wxALL, 3);


        // Content
        wxStaticText *contentlabel = new wxStaticText(nbhtml, wxID_STATIC, _("&Content (HTML inside <TD>/<DIV>):"), wxDefaultPosition, wxDefaultSize, 0);
        fieldshtmlrootsizer->Add(contentlabel, 0, wxALIGN_LEFT|wxALL, 3);

        ButcherControl_FmtTextCtrl *contentctrl = new ButcherControl_FmtTextCtrl(nbhtml, ID_CONTENT, wxDefaultPosition, wxSize(120, 110));
        contentctrl->SetEditFormat(ButcherControl_FmtTextCtrl::FMT_HTML);
        fieldshtmlrootsizer->Add(contentctrl , 1, wxGROW|wxALL, 3);
    }


    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        //*****
        //* IMAGE
        //*****
        wxBoxSizer *imagesizer = new wxBoxSizer(wxVERTICAL);
        nbimage->SetSizer(imagesizer);

        // Image URL
        wxStaticText *imageurllabel = new wxStaticText(nbimage, wxID_STATIC, _("Image &URL: (http://www.site.com)"), wxDefaultPosition, wxDefaultSize, 0);
        imagesizer->Add(imageurllabel, 0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *imageurlctrl = new wxTextCtrl(nbimage, ID_IMAGEURL, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            0);
        imagesizer->Add(imageurlctrl , 0, wxGROW|wxALL, 3);

        // Image URL Tag Append
        wxStaticText *imageurltagappendlabel = new wxStaticText(nbimage, wxID_STATIC, _("Image &URL Tag Append: (<A> attributes)"), wxDefaultPosition, wxDefaultSize, 0);
        imagesizer->Add(imageurltagappendlabel, 0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *imageurltagappendctrl = new wxTextCtrl(nbimage, ID_IMAGEURLTAGAPPEND, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            0);
        imagesizer->Add(imageurltagappendctrl , 0, wxGROW|wxALL, 3);

        // Image tag append
        wxStaticText *imagetagappendlabel = new wxStaticText(nbimage, wxID_STATIC, _("Ima&ge Tag Append: (<IMG> attributes)"), wxDefaultPosition, wxDefaultSize, 0);
        imagesizer->Add(imagetagappendlabel, 0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *imagetagappendctrl = new wxTextCtrl(nbimage, ID_IMAGETAGAPPEND, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            0);
        imagesizer->Add(imagetagappendctrl , 0, wxGROW|wxALL, 3);

        if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_ADVANCED)
        {
            // image source
            wxRadioBox *imagesourcectrl=new wxRadioBox(nbimage, ID_IMAGESOURCE, _("Image &Source"), wxDefaultPosition, wxDefaultSize,
                imagesourceimap_.Load());
            //imagesourceimap_.Load(imagesourcectrl);
            imagesizer->Add(imagesourcectrl, 0, wxGROW|wxALL, 3);

            // image link
            imagesizer->Add(
                new wxStaticText(nbimage, wxID_STATIC, _("Image &Link:"), wxDefaultPosition, wxDefaultSize, 0),
                0, wxALIGN_LEFT|wxALL, 3);
            ButcherControl_FileLink *imagelinkctrl = new ButcherControl_FileLink(nbimage, ID_IMAGELINK, NULL,
                wxDefaultPosition, wxDefaultSize);
            imagelinkctrl->SetProject(GetProject());
            imagesizer->Add(imagelinkctrl, 0, wxGROW|wxALL, 3);

			// POSITIONS
			wxStaticBoxSizer *positionbox = new wxStaticBoxSizer(wxVERTICAL, nbimage, _("Output Image Margins"));
			imagesizer->Add(positionbox, 0, wxGROW|wxALL, 3);

			wxFlexGridSizer *positionsizer = new wxFlexGridSizer(6);
			positionbox->Add(positionsizer, 0, wxGROW|wxALL, 3);

			//positionsizer->AddGrowableCol(1, 1);
			//positionsizer->AddGrowableCol(4, 1);


			// top
			positionsizer->AddSpacer(0);
			//positionsizer->AddSpacer(0);

			positionsizer->Add(
				new wxStaticText(nbimage, wxID_STATIC, _("&Top:"), wxDefaultPosition, wxDefaultSize, 0),
				0, wxALIGN_RIGHT|wxALL, 3);

			wxSpinCtrl *mtopctrl = new wxSpinCtrl(nbimage, ID_MARGINTOP, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
				wxSP_ARROW_KEYS, 0, 10000, 0);
			positionsizer->Add(mtopctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

			positionsizer->AddSpacer(0);
			positionsizer->AddSpacer(0);
			positionsizer->AddSpacer(0);

			// left
			positionsizer->Add(
				new wxStaticText(nbimage, wxID_STATIC, _("&Left:"), wxDefaultPosition, wxDefaultSize, 0),
				0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

			wxSpinCtrl *mleftctrl = new wxSpinCtrl(nbimage, ID_MARGINLEFT, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
				wxSP_ARROW_KEYS, 0, 10000, 0);
			positionsizer->Add(mleftctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

			positionsizer->AddSpacer(0);

			// right
			positionsizer->Add(
				new wxStaticText(nbimage, wxID_STATIC, _("&Right:"), wxDefaultPosition, wxDefaultSize, 0),
				0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

			wxSpinCtrl *mrightctrl = new wxSpinCtrl(nbimage, ID_MARGINRIGHT, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
				wxSP_ARROW_KEYS, 0, 10000, 0);
			positionsizer->Add(mrightctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

			positionsizer->AddSpacer(0);

			// bottom
			//positionsizer->AddSpacer(0);
			positionsizer->AddSpacer(0);

			positionsizer->Add(
				new wxStaticText(nbimage, wxID_STATIC, _("&Bottom:"), wxDefaultPosition, wxDefaultSize, 0),
				0, wxALIGN_RIGHT|wxALL, 3);

			wxSpinCtrl *mbottomctrl = new wxSpinCtrl(nbimage, ID_MARGINBOTTOM, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
				wxSP_ARROW_KEYS, 0, 10000, 0);
			positionsizer->Add(mbottomctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

			positionsizer->AddSpacer(0);
			positionsizer->AddSpacer(0);
			positionsizer->AddSpacer(0);

        }
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
    FindWindow(ID_IMAGEMAPNAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &imagemapname_));

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        FindWindow(ID_CONTENT)->SetValidator(ButcherControl_GenericValidator(&content_));
        FindWindow(ID_CELLTAGAPPEND)->SetValidator(wxTextValidator(wxFILTER_NONE, &celltagappend_));
        FindWindow(ID_IMAGEURL)->SetValidator(wxTextValidator(wxFILTER_NONE, &imageurl_));
        FindWindow(ID_IMAGEURLTAGAPPEND)->SetValidator(wxTextValidator(wxFILTER_NONE, &imageurltagappend_));
        FindWindow(ID_IMAGETAGAPPEND)->SetValidator(wxTextValidator(wxFILTER_NONE, &imagetagappend_));
        FindWindow(ID_TABLETAGAPPEND)->SetValidator(wxTextValidator(wxFILTER_NONE, &tabletagappend_));
    }


    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}

void HTMLButcherMaskAreaEditDialog::Load(ButcherProjectView *view, ButcherProjectArea *area)
{
    ButcherProjectAreaConfig *config=area->Configs().Get(view->GetBLId());

    areaname_=area->GetName();
    areakind_=config->GetAreaKind();
    imageformat_=config->ImageInfo().GetImageFormatId();
    background_=config->GetBackground();
	layouttype_=config->GetLayoutType();
    filename_=config->ImageInfo().GetFilename();
    filename_prepend_=config->ImageInfo().GetFilenamePrepend();
    haveimagemap_=config->GetHaveMap();
    imagemapname_=config->GetImageMapName();

    content_=config->GetContent();
    celltagappend_=config->GetCellTagAppend();
    imageurl_=config->GetImageURL();
    imageurltagappend_=config->GetImageURLTagAppend();
    imagetagappend_=config->GetImageTagAppend();

    align_=config->GetAlign();
    valign_=config->GetVAlign();
    bgrepeat_=config->GetBGRepeat();
    isbgcolor_=config->GetIsBGColor();
    bgcolor_=config->GetBGColor();
    innerscrollable_=config->GetInnerScrollable();
    variablesize_=config->GetVariableSize();

    alternatefile_=config->GetAlternateFile();
    imagesource_=config->GetImageSource();
    imagelink_.SetBURL(config->ImageLink().GetBURL());
	margins_=config->GetMargins();

    tabletagappend_=config->GetInnerMask()->GetTableTagAdd();

	loadview_=view;
	loadarea_=area;
/*
	previewctrl_->SetImage(view->GetFileCurrent(alternatefile_)->GetSubImage(
        area->GetGlobalRect()));
*/

/*
	previewctrl_->SetImage(view->GetFile()->GetSubImage(
        area->GetGlobalRect()));
*/
}

void HTMLButcherMaskAreaEditDialog::Save(ButcherProjectView *view, ButcherProjectArea *area)
{
    ButcherProjectAreaConfig *config=area->Configs().Get(view->GetBLId());

    ButcherProjectBaseAutoUpdate upd(area);

    area->SetName(areaname_);
    config->SetAreaKind(areakind_);
    config->ImageInfo().SetImageFormatId(imageformat_);
    config->SetBackground(background_);
	config->SetLayoutType(layouttype_);
    config->ImageInfo().SetFilename(filename_);
    config->ImageInfo().SetFilenamePrepend(filename_prepend_);
    config->SetHaveMap(haveimagemap_);
    config->SetImageMapName(imagemapname_);

    config->SetContent(content_);
    config->SetCellTagAppend(celltagappend_);
    config->SetImageURL(imageurl_);
    config->SetImageURLTagAppend(imageurltagappend_);
    config->SetImageTagAppend(imagetagappend_);

    config->SetAlign(align_);
    config->SetVAlign(valign_);
    config->SetBGRepeat(bgrepeat_);
    config->SetIsBGColor(isbgcolor_);
    config->SetBGColor(bgcolor_);
    config->SetInnerScrollable(innerscrollable_);
    config->SetVariableSize(variablesize_);
	config->GetMarginsModify()=margins_;

    config->SetAlternateFile(alternatefile_);
    config->SetImageSource(imagesource_);
    config->ImageLink().SetBURL(imagelink_.GetBURL());

    config->GetInnerMask()->SetTableTagAdd(tabletagappend_);
}



bool HTMLButcherMaskAreaEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxComboBox *kindctrl=(wxComboBox*)FindWindow(ID_KIND);
	wxRadioBox *layouttypectrl = (wxRadioBox*) FindWindow(ID_LAYOUTTYPE);
    //wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);
    wxCheckBox *backgroundctrl=(wxCheckBox*)FindWindow(ID_BACKGROUND);
    wxCheckBox *filenameprependctrl=(wxCheckBox*)FindWindow(ID_FILENAMEPREPEND);
    wxCheckBox *imagemapctrl=(wxCheckBox*)FindWindow(ID_HAVEIMAGEMAP);

    kindctrl->SetSelection(areaimap_.GetIndex(areakind_));
	layouttypectrl->SetSelection(layouttypeimap_.GetIndex(layouttype_));
    imageformats_load(imageformat_);
    backgroundctrl->SetValue(background_);
    filenameprependctrl->SetValue(filename_prepend_);
    imagemapctrl->SetValue(haveimagemap_);

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        wxCheckBox *innerscrollablectrl=(wxCheckBox*)FindWindow(ID_INNERSCROLLABLE);
        wxCheckBox *variablesizectrl=(wxCheckBox*)FindWindow(ID_VARIABLESIZE);
        wxComboBox *altfilectrl=(wxComboBox*)FindWindow(ID_FILEALTERNATE);

        wxComboBox *alignctrl=(wxComboBox*)FindWindow(ID_ALIGN);
        wxComboBox *valignctrl=(wxComboBox*)FindWindow(ID_VALIGN);
        wxComboBox *bgrepeatctrl=(wxComboBox*)FindWindow(ID_BGREPEAT);
        wxCheckBox *isbgcolorctrl=(wxCheckBox*)FindWindow(ID_ISBGCOLOR);
        wxColourPickerCtrl *bgcolorctrl=(wxColourPickerCtrl*)FindWindow(ID_BGCOLOR);

		wxSpinCtrl *mleftctrl=(wxSpinCtrl*)FindWindow(ID_MARGINLEFT);
		wxSpinCtrl *mtopctrl=(wxSpinCtrl*)FindWindow(ID_MARGINTOP);
		wxSpinCtrl *mrightctrl=(wxSpinCtrl*)FindWindow(ID_MARGINRIGHT);
		wxSpinCtrl *mbottomctrl=(wxSpinCtrl*)FindWindow(ID_MARGINBOTTOM);

        alignctrl->SetSelection(alignimap_.GetIndex(align_));
        valignctrl->SetSelection(valignimap_.GetIndex(valign_));
        bgrepeatctrl->SetSelection(bgrepeatimap_.GetIndex(bgrepeat_));
        isbgcolorctrl->SetValue(isbgcolor_);
        bgcolorctrl->SetColour(bgcolor_);

        innerscrollablectrl->SetValue(innerscrollable_);
        variablesizectrl->SetValue(variablesize_);
        altfilectrl->SetSelection(alternatefile_+1);

		mleftctrl->SetValue(margins_.GetLeft());
		mtopctrl->SetValue(margins_.GetTop());
		mrightctrl->SetValue(margins_.GetRight());
		mbottomctrl->SetValue(margins_.GetBottom());
    }

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_ADVANCED)
    {
        wxRadioBox *imagesourcectrl=(wxRadioBox*)FindWindow(ID_IMAGESOURCE);
        ButcherControl_FileLink *imagelinkctrl=(ButcherControl_FileLink*)FindWindow(ID_IMAGELINK);

        imagesourcectrl->SetSelection(imagesourceimap_.GetIndex(imagesource_));
        imagelinkctrl->SetFileLink(&imagelink_);
    }


	preview_load();
    UpdateAppState();

    return true;
}

bool HTMLButcherMaskAreaEditDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxComboBox *kindctrl=(wxComboBox*)FindWindow(ID_KIND);
	wxRadioBox *layouttypectrl = (wxRadioBox*) FindWindow(ID_LAYOUTTYPE);
    wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);
    wxCheckBox *backgroundctrl=(wxCheckBox*)FindWindow(ID_BACKGROUND);
    wxCheckBox *filenameprependctrl=(wxCheckBox*)FindWindow(ID_FILENAMEPREPEND);
    wxCheckBox *imagemapctrl=(wxCheckBox*)FindWindow(ID_HAVEIMAGEMAP);

    areakind_=areaimap_.GetId(kindctrl->GetSelection(), ButcherProjectAreaConfig::AK_NONE);
	layouttype_=layouttypeimap_.GetId(layouttypectrl->GetSelection(), ButcherProjectConsts::LTYPE_DEFAULT);
    imageformat_=wxccu_control_getselectedid(formatctrl, 0);
    background_=backgroundctrl->GetValue();
    haveimagemap_=imagemapctrl->GetValue();
    filename_prepend_=filenameprependctrl->GetValue();

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        wxCheckBox *innerscrollablectrl=(wxCheckBox*)FindWindow(ID_INNERSCROLLABLE);
        wxCheckBox *variablesizectrl=(wxCheckBox*)FindWindow(ID_VARIABLESIZE);

        wxComboBox *alignctrl=(wxComboBox*)FindWindow(ID_ALIGN);
        wxComboBox *valignctrl=(wxComboBox*)FindWindow(ID_VALIGN);
        wxComboBox *bgrepeatctrl=(wxComboBox*)FindWindow(ID_BGREPEAT);
        wxCheckBox *isbgcolorctrl=(wxCheckBox*)FindWindow(ID_ISBGCOLOR);
        wxColourPickerCtrl *bgcolorctrl=(wxColourPickerCtrl*)FindWindow(ID_BGCOLOR);
        wxComboBox *altfilectrl=(wxComboBox*)FindWindow(ID_FILEALTERNATE);

		wxSpinCtrl *mleftctrl=(wxSpinCtrl*)FindWindow(ID_MARGINLEFT);
		wxSpinCtrl *mtopctrl=(wxSpinCtrl*)FindWindow(ID_MARGINTOP);
		wxSpinCtrl *mrightctrl=(wxSpinCtrl*)FindWindow(ID_MARGINRIGHT);
		wxSpinCtrl *mbottomctrl=(wxSpinCtrl*)FindWindow(ID_MARGINBOTTOM);

        align_=alignimap_.GetId(alignctrl->GetSelection(), ButcherProjectAreaConfig::AA_NONE);
        valign_=valignimap_.GetId(valignctrl->GetSelection(), ButcherProjectAreaConfig::AV_NONE);
        bgrepeat_=bgrepeatimap_.GetId(bgrepeatctrl->GetSelection(), ButcherProjectAreaConfig::BR_NONE);
        isbgcolor_=isbgcolorctrl->GetValue();
        bgcolor_=bgcolorctrl->GetColour();
        innerscrollable_=innerscrollablectrl->GetValue();
        variablesize_=variablesizectrl->GetValue();
        alternatefile_=altfilectrl->GetSelection()-1;
		margins_.SetLeft(mleftctrl->GetValue());
		margins_.SetTop(mtopctrl->GetValue());
		margins_.SetRight(mrightctrl->GetValue());
		margins_.SetBottom(mbottomctrl->GetValue());
    }

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_ADVANCED)
    {
        wxRadioBox *imagesourcectrl=(wxRadioBox*)FindWindow(ID_IMAGESOURCE);
        ButcherControl_FileLink *imagelinkctrl=(ButcherControl_FileLink*)FindWindow(ID_IMAGELINK);

        imagesource_=imagesourceimap_.GetId(imagesourcectrl->GetSelection(), ButcherProjectAreaConfig::IS_AREA);
        imagelink_.SetBURL(imagelinkctrl->GetFileLink()->GetBURL());
    }

    return true;
}

void HTMLButcherMaskAreaEditDialog::OnImageFormat(wxCommandEvent &event)
{
    HTMLButcherImageFormatListEditDialog d(this, wxID_ANY, GetProject());
    if (previewctrl_->GetImage())
        d.SetPreview(previewctrl_->GetImage()->Clone());
    d.ShowModal();

    imageformats_load(imageformat_);
}


void HTMLButcherMaskAreaEditDialog::imageformats_load(BLID_t selid)
{
    wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);
	formatctrl->Clear();

	for (ButcherProjectImageFormats::iterator i=GetProject()->ImageFormats().begin(); 
		i!=GetProject()->ImageFormats().end(); i++)
	{
        wxccu_control_append_select(formatctrl, i->GetName(),
            i.first(), selid);
	}
}



void HTMLButcherMaskAreaEditDialog::preview_load()
{
    wxComboBox *altfilectrl=(wxComboBox*)FindWindow(ID_FILEALTERNATE);

	previewctrl_->SetImage(loadview_->GetFileCurrent(altfilectrl->GetSelection()-1)->GetSubImage(
        loadarea_->GetGlobalRect()));
}

void HTMLButcherMaskAreaEditDialog::OnSelectColor(ButcherImageViewColorEvent &event)
{
    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {

        int cid=ID_PROC_HOVERCOLOR;
        if (event.GetSelect()==ButcherImageViewColorEvent::CS_SELECT ||
            event.GetSelect()==ButcherImageViewColorEvent::CS_CONFIRM)
        {
            cid=ID_PROC_SELCOLOR;

            if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
            {
                if (event.GetSelect()==ButcherImageViewColorEvent::CS_CONFIRM)
                {
                    wxColourPickerCtrl *bgcolorctrl=(wxColourPickerCtrl*)FindWindow(ID_BGCOLOR);
                    bgcolorctrl->SetColour(event.GetColor());
                }
            }
        }
        wxTextCtrl *ct=(wxTextCtrl*)FindWindow(cid);

        // some combinations of the fg/bg colours may be unreadable, so we invert
        // the colour to make sure fg colour is different enough from m_colour
        wxColour colFg(~event.GetColor().Red(), ~event.GetColor().Green(), ~event.GetColor().Blue());

        ct->SetForegroundColour(colFg);
        ct->SetBackgroundColour(event.GetColor());
        ct->SetValue(event.GetColor().GetAsString(wxC2S_HTML_SYNTAX));

        //SetTitle(event.GetColor().GetAsString(wxC2S_HTML_SYNTAX));
    }
    event.Skip();
}

void HTMLButcherMaskAreaEditDialog::OnCtrlChanged(wxCommandEvent &event)
{
    UpdateAppState();
}

void HTMLButcherMaskAreaEditDialog::UpdateAppState()
{
	wxRadioBox *layouttypectrl = (wxRadioBox*) FindWindow(ID_LAYOUTTYPE);
	ButcherProjectConsts::layouttype_t layouttype=layouttypeimap_.GetId(layouttypectrl->GetSelection(), ButcherProjectConsts::LTYPE_DEFAULT);

	bool imagemap=static_cast<wxCheckBox*>(FindWindow(ID_HAVEIMAGEMAP))->GetValue();
    ButcherProjectAreaConfigBase::areakind_t areakind=
        areaimap_.GetId(static_cast<wxComboBox*>(FindWindow(ID_KIND))->GetSelection(), ButcherProjectAreaConfig::AK_NONE);

	FindWindow(ID_IMAGEMAPNAME)->Enable(imagemap);
	FindWindow(ID_LAYOUTTYPE)->Enable(areakind==ButcherProjectAreaConfigBase::AK_MASK);

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        bool background=static_cast<wxCheckBox*>(FindWindow(ID_BACKGROUND))->GetValue();

        bool isbgcolor=static_cast<wxCheckBox*>(FindWindow(ID_ISBGCOLOR))->GetValue();

        FindWindow(ID_BGREPEAT)->Enable(background);
		FindWindow(ID_TABLETAGAPPEND)->Enable(layouttype!=ButcherProjectConsts::LTYPE_APTABLELESS && areakind==ButcherProjectAreaConfigBase::AK_MASK);
		//FindWindow(ID_CELLTAGAPPEND)->Enable(layouttype!=ButcherProjectConsts::LTYPE_APTABLELESS);
		FindWindow(ID_IMAGEURL)->Enable(areakind==ButcherProjectAreaConfigBase::AK_IMAGE && !background);
        FindWindow(ID_IMAGEURLTAGAPPEND)->Enable(areakind==ButcherProjectAreaConfigBase::AK_IMAGE && !background);
        FindWindow(ID_IMAGETAGAPPEND)->Enable(areakind==ButcherProjectAreaConfigBase::AK_IMAGE && !background);
        FindWindow(ID_BGCOLOR)->Enable(isbgcolor);

        if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_ADVANCED)
        {
            FindWindow(ID_IMAGESOURCE)->Enable(areakind==ButcherProjectAreaConfigBase::AK_IMAGE || background);
            FindWindow(ID_IMAGELINK)->Enable(areakind==ButcherProjectAreaConfigBase::AK_IMAGE || background);
        }
    }
}



void HTMLButcherMaskAreaEditDialog::OnAlternateFileChanged(wxCommandEvent &event)
{
	preview_load();
}


