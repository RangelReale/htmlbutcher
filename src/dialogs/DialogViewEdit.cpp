//=============================================================================
/**
 *  @file    DialogViewEdit.cpp
 *
 *  $Id: DialogViewEdit.cpp,v 1.27 2008/02/16 20:07:37 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-03
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
#include <wx/notebook.h>
#include <wx/clrpicker.h>

#include "DialogViewEdit.h"
#include "PopupCSSEdit.h"
#include "BUtil.h"



BEGIN_EVENT_TABLE(HTMLButcherViewEditDialog, ButcherControl_Dialog)
    EVT_BUTTON(ID_BTN_CSS, HTMLButcherViewEditDialog::OnBtnCSS)
	EVT_RADIOBOX(ID_LAYOUTTYPE, HTMLButcherViewEditDialog::OnCtrlChanged)
END_EVENT_TABLE()

HTMLButcherViewEditDialog::HTMLButcherViewEditDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style),
    encodingimap_(), bgrepeatimap_(), layouttypeimap_(),
    assortedfilegroups_(),
    bgimage_(project)
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    Init();
    CreateControls();
}



void HTMLButcherViewEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-view.html"));
#endif

	viewname_=wxEmptyString;
	layouttype_=ButcherProjectConsts::LTYPE_DEFAULT;
    fileid_=0;
    maskid_=0;
    imagefilepathid_=GetProject()->FilePaths().DefaultId();
    cssfileid_=0;
    memset(filealternateid_, 0, sizeof(filealternateid_));
    items_=ButcherProjectView::ITEM_HTML|ButcherProjectView::ITEM_IMAGES|
        ButcherProjectView::ITEM_IMAGESGLOBAL;
    filename_=wxEmptyString;
    bodytagadd_=wxEmptyString;
    tabletagadd_=wxEmptyString;
    title_=wxEmptyString;
    head_=wxEmptyString;
    bodyprepend_=wxEmptyString;
    bodyappend_=wxEmptyString;
    encoding_=ButcherHTMLConsts::BE_ISO8859_1;
    bgrepeat_=ButcherProjectAreaConfig::BR_NONE;
    isbgcolor_=false;
    isstretch_=false;
    bgcolor_=wxColor(wxT("BLACK"));

    encodingimap_.Add(ButcherHTMLConsts::BE_UTF8, _("UTF8"));
    encodingimap_.Add(ButcherHTMLConsts::BE_ISO8859_1, _("ISO8859-1"));

    bgrepeatimap_.Add(ButcherProjectAreaConfig::BR_NONE, _("None"));
    bgrepeatimap_.Add(ButcherProjectAreaConfig::BR_HORIZONTAL, _("Horizontal"));
    bgrepeatimap_.Add(ButcherProjectAreaConfig::BR_VERTICAL, _("Vertical"));
    bgrepeatimap_.Add(ButcherProjectAreaConfig::BR_ALL, _("All"));

	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_DEFAULT, _("Default"));
	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_TABLE, _("Table"));
	layouttypeimap_.Add(ButcherProjectConsts::LTYPE_APTABLELESS, _("AP Tableless"));
}



void HTMLButcherViewEditDialog::Load(ButcherProjectView *view)
{
    viewname_=view->GetName();
	layouttype_=view->GetLayoutType();
    fileid_=view->GetFileId();
    maskid_=view->GetMaskId();
    maskconfigid_=view->GetBLId();
    items_=view->GetItems();
    imagefilepathid_=view->GetImageFilePathId();
    cssfileid_=view->GetCSSFileId();
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        filealternateid_[i]=view->GetFileAlternateId(i);
    filename_=view->GetFilename();
    bodytagadd_=view->GetBodyTagAdd();
    tabletagadd_=view->GetTableTagAdd();
    title_=view->GetTitle();
    head_=view->GetHead();
    bodyprepend_=view->GetBodyPrepend();
    bodyappend_=view->GetBodyAppend();
    encoding_=view->GetEncoding();
    assortedfilegroups_=view->AssortedFileGroups();
    bgimage_.SetBURL(view->BGImage().GetBURL());
    bgrepeat_=view->GetBGRepeat();
    isbgcolor_=view->GetIsBGColor();
    isstretch_=view->GetIsStretch();
	styleseparated_=view->GetStyleSeparated();
    bgcolor_=view->GetBGColor();
}



void HTMLButcherViewEditDialog::Save(ButcherProjectView *view)
{
    ButcherProjectBaseAutoUpdate upd(view);

    view->SetName(viewname_);
	view->SetLayoutType(layouttype_);
    view->SetFileId(fileid_);
    view->SetMaskId(maskid_);
    //view->SetMaskConfigId(maskconfigid_);
    view->SetItems(items_);
    view->SetImageFilePathId(imagefilepathid_);
    view->SetCSSFileId(cssfileid_);
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        view->SetFileAlternateId(i, filealternateid_[i]);
    view->SetFilename(filename_);
    view->SetBodyTagAdd(bodytagadd_);
    view->SetTableTagAdd(tabletagadd_);
    view->SetTitle(title_);
    view->SetHead(head_);
    view->SetBodyPrepend(bodyprepend_);
    view->SetBodyAppend(bodyappend_);
    view->SetEncoding(encoding_);
    view->AssortedFileGroups().Set(assortedfilegroups_.Get());
    view->SetBGRepeat(bgrepeat_);
    view->SetIsBGColor(isbgcolor_);
    view->SetIsStretch(isstretch_);
	view->SetStyleSeparated(styleseparated_);
    view->SetBGColor(bgcolor_);
    view->BGImage().SetBURL(bgimage_.GetBURL());
}




void HTMLButcherViewEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);


    // NOTEBOOK
    wxNotebook *notebook = new wxNotebook(this, wxID_ANY);
    topsizer->Add(notebook, 1, wxEXPAND|wxALL, 3);

    wxPanel *nbMain = new wxPanel(notebook, wxID_ANY);
    wxPanel *nbHTML = NULL;
    wxPanel *nbAlternate = new wxPanel(notebook, wxID_ANY);

    notebook->AddPage(nbMain, _("View"), true);
    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        nbHTML = new wxPanel(notebook, wxID_ANY);
        notebook->AddPage(nbHTML, _("HTML"), false);
    }
    notebook->AddPage(nbAlternate, _("Alternate"), false);

    /***
    ** Main
    ***/

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    nbMain->SetSizer(boxsizer);

    // Name
    wxStaticText *namelabel = new wxStaticText(nbMain, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(namelabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl = new wxTextCtrl(nbMain, ID_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // Layout type
	wxRadioBox *layouttypectrl = new wxRadioBox(nbMain, ID_LAYOUTTYPE, _("&Layout type"), wxDefaultPosition, wxSize(300, -1),
        layouttypeimap_.Load(), 0);
	boxsizer->Add(layouttypectrl, 0, wxGROW|wxALL, 3);

    // File
    wxStaticText *filelabel = new wxStaticText(nbMain, wxID_STATIC, _("&File:"), wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(filelabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *filectrl = new wxComboBox(nbMain, ID_FILE, wxEmptyString, wxDefaultPosition, wxSize(300, -1),
        wxArrayString(), wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT);
    boxsizer->Add(filectrl, 0, wxGROW|wxALL, 3);

    // Mask
    wxStaticText *masklabel = new wxStaticText(nbMain, wxID_STATIC, _("&Mask:"), wxDefaultPosition, wxDefaultSize, 0);
    boxsizer->Add(masklabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *maskctrl = new wxComboBox(nbMain, ID_MASK, wxEmptyString, wxDefaultPosition, wxSize(300, -1),
        wxArrayString(), wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT);
    boxsizer->Add(maskctrl, 0, wxGROW|wxALL, 3);

    // Image dir
    boxsizer->Add(new wxStaticText(nbMain, wxID_STATIC, _("&Image path:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *imagedirctrl = new wxComboBox(nbMain, ID_IMAGEDIR, wxEmptyString, wxDefaultPosition, wxSize(300, -1),
        wxArrayString(), wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT);
    boxsizer->Add(imagedirctrl, 0, wxGROW|wxALL, 3);


	// FILENAME-ENCODING sizer
	wxFlexGridSizer *boxsizer2 = new wxFlexGridSizer(2, 0, 0);
	boxsizer2->SetFlexibleDirection(wxVERTICAL);
	boxsizer2->AddGrowableCol(0, 0);
	boxsizer2->AddGrowableCol(1, 0);
	boxsizer->Add(boxsizer2, 0, wxGROW|wxALL, 0);
	
	// Filename
    boxsizer2->Add(
        new wxStaticText(nbMain, wxID_STATIC, _("&Filename:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALL|wxGROW, 3);

    // encoding
    boxsizer2->Add(new wxStaticText(nbMain, wxID_STATIC, _("&Encoding:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALL|wxGROW, 3);

	// Filename
    wxTextCtrl *filenamectrl = new wxTextCtrl(nbMain, ID_FILENAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    boxsizer2->Add(filenamectrl, 0, wxGROW|wxALL, 3);

    // encoding
    wxComboBox *encodingctrl=new wxComboBox(nbMain, ID_ENCODING, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);
    encodingimap_.Load(encodingctrl);

    boxsizer2->Add(encodingctrl, 0, wxGROW|wxALL, 3);


    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        // ITEMSGROUPSSIZER
        wxBoxSizer *itemsgroupssizer = new wxBoxSizer(wxHORIZONTAL);
        boxsizer->Add(itemsgroupssizer, 1, wxGROW|wxALL, 3);

        // ITEMS SIZER
        wxStaticBoxSizer *itemssizer = new wxStaticBoxSizer(wxVERTICAL, nbMain, _("Items"));

        // Check: HTML
        wxCheckBox *itemhtmlctrl = new wxCheckBox(nbMain, ID_ITEM_HTML, _("HTML"), wxDefaultPosition, wxDefaultSize, 0);
        itemssizer->Add(itemhtmlctrl, 1, wxGROW|wxALL);

        // Check: IMAGES
        wxCheckBox *itemimagesctrl = new wxCheckBox(nbMain, ID_ITEM_IMAGES, _("Images"), wxDefaultPosition, wxDefaultSize, 0);
        itemssizer->Add(itemimagesctrl, 1, wxGROW|wxALL);

        // Check: IMAGESGLOBAL
        wxCheckBox *itemimagesglobalctrl = new wxCheckBox(nbMain, ID_ITEM_IMAGESGLOBAL, _("Images Global"), wxDefaultPosition, wxDefaultSize, 0);
        itemssizer->Add(itemimagesglobalctrl, 1, wxGROW|wxALL);

        // ITEMSSIZER: ADD
        itemsgroupssizer->Add(itemssizer, 0, wxGROW|wxALL, 3);

        // GROUPSSIZER
        wxBoxSizer *groupssizer = new wxBoxSizer(wxVERTICAL);
        itemsgroupssizer->Add(groupssizer, 1, wxGROW|wxALL, 3);

        // Groups
        groupssizer->Add(new wxStaticText(nbMain, wxID_STATIC, _("&Assorted file groups:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALL, 3);

        wxCheckListBox *groupsctrl = new wxCheckListBox(nbMain, ID_ASSORTEDFILEGROUPS, wxDefaultPosition, wxDefaultSize);
        groupssizer->Add(groupsctrl, 1, wxGROW|wxALL, 3);
    }

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        /***
        ** HTML
        ***/

        //wxBoxSizer *htmlboxsizer = new wxBoxSizer(wxVERTICAL);
        wxFlexGridSizer *htmlboxsizer = new wxFlexGridSizer(2);
        nbHTML->SetSizer(htmlboxsizer);

        // FIELDS SIZER
        //wxBoxSizer *fieldssizer = new wxBoxSizer(wxHORIZONTAL);
        //htmlboxsizer->Add(fieldssizer, 0, wxGROW|wxALL, 3);


        // FIELDS 1 SIZER
        wxBoxSizer *fields1sizer = new wxBoxSizer(wxVERTICAL);
        htmlboxsizer->Add(fields1sizer, 0, wxGROW|wxALL, 3);


        // CSS file
        fields1sizer->Add(new wxStaticText(nbHTML, wxID_STATIC, _("C&SS file:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxComboBox *cssfilectrl = new wxComboBox(nbHTML, ID_CSSFILE, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxArrayString(), wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT);
        fields1sizer->Add(cssfilectrl, 0, wxGROW|wxALL, 3);

        // bgimage
        fields1sizer->Add(
            new wxStaticText(nbHTML, wxID_STATIC, _("Back&ground Image:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);
        ButcherControl_FileLink *imagelinkctrl = new ButcherControl_FileLink(nbHTML, ID_BGIMAGE, NULL,
            wxDefaultPosition, wxDefaultSize);
        imagelinkctrl->SetProject(GetProject());
        fields1sizer->Add(imagelinkctrl, 0, wxGROW|wxALL, 3);

        // bgrepeat
        fields1sizer->Add(
            new wxStaticText(nbHTML, wxID_STATIC, _("Background &Repeat:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxComboBox *bgrepeatctrl=new wxComboBox(nbHTML, ID_BGREPEAT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxArrayString(), wxCB_READONLY);
        bgrepeatimap_.Load(bgrepeatctrl);

        fields1sizer->Add(bgrepeatctrl, 0, wxGROW|wxALL, 3);


        // isbgcolor
        wxCheckBox *isbgcolorctrl = new wxCheckBox(nbHTML, ID_ISBGCOLOR, _("Background Co&lor:"), wxDefaultPosition, wxDefaultSize);
        fields1sizer->Add(isbgcolorctrl, 0, wxGROW|wxALL, 3);

        // bgcolor
        wxColourPickerCtrl *bgcolorctrl = new wxColourPickerCtrl(nbHTML, ID_BGCOLOR, *wxBLACK, wxDefaultPosition, wxDefaultSize,
            wxCLRP_SHOW_LABEL|wxCLRP_USE_TEXTCTRL);
        fields1sizer->Add(bgcolorctrl , 0, wxGROW|wxALL, 3);

        // is stretch
        wxCheckBox *isstretchctrl = new wxCheckBox(nbHTML, ID_ISSTRETCH, _("Stretchable La&yout"), wxDefaultPosition, wxDefaultSize);
        fields1sizer->Add(isstretchctrl, 0, wxGROW|wxALL, 3);


        // FIELDS 2 SIZER
        wxBoxSizer *fields2sizer = new wxBoxSizer(wxVERTICAL);
        htmlboxsizer->Add(fields2sizer, 1, wxGROW|wxALL, 3);

        // Title
        fields2sizer->Add(
            new wxStaticText(nbHTML, wxID_STATIC, _("&Title:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *titlectrl = new wxTextCtrl(nbHTML, ID_TITLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
        fields2sizer->Add(titlectrl, 0, wxGROW|wxALL, 3);



        // Head
        fields2sizer->Add(
            new wxStaticText(nbHTML, wxID_STATIC, _("&Head (HTML inside <HEAD>):"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        ButcherControl_FmtTextCtrl *headctrl = new ButcherControl_FmtTextCtrl(nbHTML, ID_HEAD, wxDefaultPosition, wxSize(200, 110));
        headctrl->SetEditFormat(ButcherControl_FmtTextCtrl::FMT_HTML);
        headctrl->SetAllowFileLink(GetProject(), true);
        fields2sizer->Add(headctrl, 1, wxGROW|wxALL, 3);

        // style separated
        wxCheckBox *styleseparatedctrl = new wxCheckBox(nbHTML, ID_STYLESEPARATED, _("AP tableless styles on top of pa&ge"), wxDefaultPosition, wxDefaultSize);
        fields2sizer->Add(styleseparatedctrl, 0, wxGROW|wxALL, 3);

        // FIELDS 3 SIZER

        // FIELDS 3 SIZER 1
        wxBoxSizer *fields3sizer1 = new wxBoxSizer(wxVERTICAL);
        htmlboxsizer->Add(fields3sizer1, 1, wxGROW|wxALL, 3);

        // FIELDS 3 SIZER 2
        wxBoxSizer *fields3sizer2 = new wxBoxSizer(wxVERTICAL);
        htmlboxsizer->Add(fields3sizer2, 1, wxGROW|wxALL, 3);


        // Body Tag Add
        fields3sizer1->Add(
            new wxStaticText(nbHTML, wxID_STATIC, _("&Body Tag Add: (<BODY> attributes)"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *bodytagaddctrl = new wxTextCtrl(nbHTML, ID_BODYTAGADD, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
        fields3sizer1->Add(bodytagaddctrl, 0, wxGROW|wxALL, 3);

        // Body Prepend
        fields3sizer1->Add(
            new wxStaticText(nbHTML, wxID_STATIC, _("Body &Prepend:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        ButcherControl_FmtTextCtrl *bodyprependctrl = new ButcherControl_FmtTextCtrl(nbHTML, ID_BODYPREPEND, wxDefaultPosition, wxSize(200, 110));
        bodyprependctrl->SetEditFormat(ButcherControl_FmtTextCtrl::FMT_HTML);
        fields3sizer1->Add(bodyprependctrl , 1, wxGROW|wxALL, 3);


        // Table Tag Add
        fields3sizer2->Add(
            new wxStaticText(nbHTML, wxID_STATIC, _("&Table Tag Add: (<TABLE> attributes)"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxTextCtrl *tabletagaddctrl = new wxTextCtrl(nbHTML, ID_TABLETAGADD, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
        fields3sizer2->Add(tabletagaddctrl, 0, wxGROW|wxALL, 3);

        // Body Append
        fields3sizer2->Add(
            new wxStaticText(nbHTML, wxID_STATIC, _("Body &Append:"), wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        ButcherControl_FmtTextCtrl *bodyappendctrl = new ButcherControl_FmtTextCtrl(nbHTML, ID_BODYAPPEND, wxDefaultPosition, wxSize(200, 110));
        bodyappendctrl->SetEditFormat(ButcherControl_FmtTextCtrl::FMT_HTML);
        fields3sizer2->Add(bodyappendctrl, 1, wxGROW|wxALL, 3);
    }


    /***
    ** ALTERNATE
    ***/

    wxBoxSizer *alternateboxsizer = new wxBoxSizer(wxVERTICAL);
    nbAlternate->SetSizer(alternateboxsizer);

    // File Alternate
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
    {
        alternateboxsizer->Add(new wxStaticText(nbAlternate, wxID_STATIC,
            wxString::Format(_("Alternate File &%d:"), i+1),
                wxDefaultPosition, wxDefaultSize, 0),
            0, wxALIGN_LEFT|wxALL, 3);

        wxComboBox *filealtctrl = new wxComboBox(nbAlternate, ID_FILEALTERNATE+i, wxEmptyString, wxDefaultPosition, wxSize(300, -1),
            wxArrayString(), wxCB_DROPDOWN|wxCB_READONLY|wxCB_SORT);
        alternateboxsizer->Add(filealtctrl, 0, wxGROW|wxALL, 3);
    }


    // BOXSIZER: ADD
    //topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);


    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    topsizer->Add(line, 0, wxEXPAND|wxALL, 3);

    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);

    // css button
    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        wxButton* cssbutton = new wxButton ( this, ID_BTN_CSS, _("CSS"), wxDefaultPosition, wxDefaultSize, 0 );
        buttonsizer->Add(cssbutton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    }

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

    // BUTTONSIZER: ADD
    topsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // validators

    FindWindow(ID_NAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &viewname_));
    //FindWindow(ID_WIDTH)->SetValidator(wxGenericValidator((int*)&maskwidth_));
    //FindWindow(ID_HEIGHT)->SetValidator(wxGenericValidator((int*)&maskheight_));

    FindWindow(ID_FILENAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &filename_));

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        FindWindow(ID_TITLE)->SetValidator(wxTextValidator(wxFILTER_NONE, &title_));
        FindWindow(ID_BODYTAGADD)->SetValidator(wxTextValidator(wxFILTER_NONE, &bodytagadd_));
        FindWindow(ID_TABLETAGADD)->SetValidator(wxTextValidator(wxFILTER_NONE, &tabletagadd_));
        FindWindow(ID_HEAD)->SetValidator(ButcherControl_GenericValidator(&head_));
        FindWindow(ID_BODYPREPEND)->SetValidator(ButcherControl_GenericValidator(&bodyprepend_));
        FindWindow(ID_BODYAPPEND)->SetValidator(ButcherControl_GenericValidator(&bodyappend_));
    }

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



bool HTMLButcherViewEditDialog::TransferDataToWindow()
{
	wxRadioBox *layouttypectrl = (wxRadioBox*) FindWindow(ID_LAYOUTTYPE);
    wxComboBox *filectrl = (wxComboBox*) FindWindow(ID_FILE);
    wxComboBox *filealtctrl = (wxComboBox*) FindWindow(ID_FILEALTERNATE);
    wxComboBox *maskctrl = (wxComboBox*) FindWindow(ID_MASK);
    wxComboBox *imagedirctrl = (wxComboBox*) FindWindow(ID_IMAGEDIR);
    wxComboBox *encodingctrl=(wxComboBox*)FindWindow(ID_ENCODING);

	// LAYOUT TYPE
	layouttypectrl->SetSelection(layouttypeimap_.GetIndex(layouttype_));

    wxString tmp;
    int newitem;
    // FILES
	for (ButcherProjectFiles::iterator i=GetProject()->Files().begin(); i!=GetProject()->Files().end(); i++)
	{
        tmp=wxString::Format(wxT("%s [%dx%d]"),
            i->GetDisplayName().c_str(),
            i->GetImageWidth(),
            i->GetImageHeight());

        newitem=filectrl->Append(tmp,
            static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
        if (i.first()==fileid_)
            filectrl->SetSelection(newitem);

        for (int alti=0; alti<BUTCHERCONST_VIEW_MAXALTERNATE; alti++)
        {
            filealtctrl = (wxComboBox*) FindWindow(ID_FILEALTERNATE+alti);

            newitem=filealtctrl->Append(tmp,
                static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
            if (i.first()==filealternateid_[alti])
                filealtctrl->SetSelection(newitem);
        }
	}

    // MASKS
	for (ButcherProjectMasks::iterator i=GetProject()->Masks().begin(); i!=GetProject()->Masks().end(); i++)
	{
        newitem=maskctrl->Append(wxString::Format(wxT("%s"),
                i->GetName().c_str()),
            static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
        if (i.first()==maskid_)
            maskctrl->SetSelection(newitem);
	}

    // FILE PATHS
	for (ButcherProjectFilePaths::iterator i=GetProject()->FilePaths().begin(); i!=GetProject()->FilePaths().end(); i++)
	{
        tmp=wxString::Format(wxT("%s"),
            i->GetName().c_str());

        newitem=imagedirctrl->Append(tmp,
            static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
        if (i.first()==imagefilepathid_)
            imagedirctrl->SetSelection(newitem);
	}

    encodingctrl->SetSelection(encodingimap_.GetIndex(encoding_));

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        wxCheckBox *itemhtmlctrl = (wxCheckBox*) FindWindow(ID_ITEM_HTML);
        wxCheckBox *itemimagesctrl = (wxCheckBox*) FindWindow(ID_ITEM_IMAGES);
        wxCheckBox *itemimagesglobalctrl = (wxCheckBox*) FindWindow(ID_ITEM_IMAGESGLOBAL);
        wxCheckListBox *groupsctrl=(wxCheckListBox*)FindWindow(ID_ASSORTEDFILEGROUPS);

        wxComboBox *cssfilectrl = (wxComboBox*) FindWindow(ID_CSSFILE);

        ButcherControl_FileLink *bgimagectrl=(ButcherControl_FileLink*)FindWindow(ID_BGIMAGE);
        wxComboBox *bgrepeatctrl=(wxComboBox*)FindWindow(ID_BGREPEAT);
        wxCheckBox *isbgcolorctrl=(wxCheckBox*)FindWindow(ID_ISBGCOLOR);
        wxCheckBox *isstretchctrl=(wxCheckBox*)FindWindow(ID_ISSTRETCH);
		wxCheckBox *styleseparatedctrl=(wxCheckBox*)FindWindow(ID_STYLESEPARATED);
        wxColourPickerCtrl *bgcolorctrl=(wxColourPickerCtrl*)FindWindow(ID_BGCOLOR);


        itemhtmlctrl->SetValue((items_&ButcherProjectView::ITEM_HTML)==ButcherProjectView::ITEM_HTML);
        itemimagesctrl->SetValue((items_&ButcherProjectView::ITEM_IMAGES)==ButcherProjectView::ITEM_IMAGES);
        itemimagesglobalctrl->SetValue((items_&ButcherProjectView::ITEM_IMAGESGLOBAL)==ButcherProjectView::ITEM_IMAGESGLOBAL);

        // ASSORTED FILE GROUPS
		for (ButcherProjectAssortedFileGroups::iterator i=GetProject()->AssortedFileGroups().begin(); 
			i!=GetProject()->AssortedFileGroups().end(); i++)
		{
            newitem=groupsctrl->Append(i->GetName());
            groupsctrl->Check(newitem, assortedfilegroups_.Exists(i.first()));
		}

        // CSS FILES
        wxccu_control_append_select(cssfilectrl, _("No CSS"), BLID_t(0), cssfileid_);

		for (ButcherProjectCSSFiles::iterator i=GetProject()->CSSFiles().begin(); i!=GetProject()->CSSFiles().end(); i++)
		{
            tmp=wxString::Format(wxT("%s"),
                i->GetName().c_str());

            wxccu_control_append_select(cssfilectrl, tmp, i.first(), cssfileid_);
		}

        bgimagectrl->SetFileLink(&bgimage_);
        bgrepeatctrl->SetSelection(bgrepeatimap_.GetIndex(bgrepeat_));
        isbgcolorctrl->SetValue(isbgcolor_);
        isstretchctrl->SetValue(isstretch_);
		styleseparatedctrl->SetValue(styleseparated_);
        bgcolorctrl->SetColour(bgcolor_);
    }


    return wxDialog::TransferDataToWindow();
}



bool HTMLButcherViewEditDialog::TransferDataFromWindow()
{
    // check
    wxTextCtrl *namectrl = (wxTextCtrl*)FindWindow(ID_NAME);
    if (namectrl->GetValue().IsEmpty())
    {
        butil_errordialog(_("Name cannot be blank"), this);
        return false;
    }

    // save
	wxRadioBox *layouttypectrl = (wxRadioBox*) FindWindow(ID_LAYOUTTYPE);
    wxComboBox *filectrl = (wxComboBox*) FindWindow(ID_FILE);
    wxComboBox *filealtctrl = (wxComboBox*) FindWindow(ID_FILEALTERNATE);
    wxComboBox *maskctrl = (wxComboBox*) FindWindow(ID_MASK);
    wxComboBox *imagedirctrl = (wxComboBox*) FindWindow(ID_IMAGEDIR);
    wxComboBox *encodingctrl=(wxComboBox*)FindWindow(ID_ENCODING);

	layouttype_=layouttypeimap_.GetId(layouttypectrl->GetSelection(), ButcherProjectConsts::LTYPE_DEFAULT);

    if (filectrl->GetSelection() == wxNOT_FOUND) {
        butil_errordialog(_("File is required"), this);
        return false;
    }
    if (maskctrl->GetSelection() == wxNOT_FOUND) {
        butil_errordialog(_("Mask is required"), this);
        return false;
    }


    fileid_=static_cast<ButcherListIdClientData*>(filectrl->GetClientObject(filectrl->GetSelection()))->GetId();
    for (int alti=0; alti<BUTCHERCONST_VIEW_MAXALTERNATE; alti++)
    {
        filealtctrl = (wxComboBox*) FindWindow(ID_FILEALTERNATE+alti);

        if (filealtctrl->GetSelection() == wxNOT_FOUND)
            filealternateid_[alti]=0;
        else
            filealternateid_[alti]=static_cast<ButcherListIdClientData*>(filealtctrl->GetClientObject(filealtctrl->GetSelection()))->GetId();
    }

    maskid_=static_cast<ButcherListId2ClientData*>(maskctrl->GetClientObject(maskctrl->GetSelection()))->GetId();
    maskconfigid_=static_cast<ButcherListId2ClientData*>(maskctrl->GetClientObject(maskctrl->GetSelection()))->GetId2();
    if (imagedirctrl->GetSelection()!=wxNOT_FOUND)
        imagefilepathid_=static_cast<ButcherListIdClientData*>(imagedirctrl->GetClientObject(imagedirctrl->GetSelection()))->GetId();
    else
        imagefilepathid_=0;

    encoding_=encodingimap_.GetId(encodingctrl->GetSelection(), ButcherHTMLConsts::BE_ISO8859_1);

    if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
    {
        wxCheckBox *itemhtmlctrl = (wxCheckBox*) FindWindow(ID_ITEM_HTML);
        wxCheckBox *itemimagesctrl = (wxCheckBox*) FindWindow(ID_ITEM_IMAGES);
        wxCheckBox *itemimagesglobalctrl = (wxCheckBox*) FindWindow(ID_ITEM_IMAGESGLOBAL);
        wxCheckListBox *groupsctrl=(wxCheckListBox*)FindWindow(ID_ASSORTEDFILEGROUPS);

        wxComboBox *cssfilectrl = (wxComboBox*) FindWindow(ID_CSSFILE);

        ButcherControl_FileLink *bgimagectrl=(ButcherControl_FileLink*)FindWindow(ID_BGIMAGE);
        wxComboBox *bgrepeatctrl=(wxComboBox*)FindWindow(ID_BGREPEAT);
        wxCheckBox *isbgcolorctrl=(wxCheckBox*)FindWindow(ID_ISBGCOLOR);
        wxCheckBox *isstretchctrl=(wxCheckBox*)FindWindow(ID_ISSTRETCH);
		wxCheckBox *styleseparatedctrl=(wxCheckBox*)FindWindow(ID_STYLESEPARATED);
        wxColourPickerCtrl *bgcolorctrl=(wxColourPickerCtrl*)FindWindow(ID_BGCOLOR);

        cssfileid_=wxccu_control_getselectedid(cssfilectrl, 0);

        items_=0;
        if (itemhtmlctrl->GetValue()) items_|=ButcherProjectView::ITEM_HTML;
        if (itemimagesctrl->GetValue()) items_|=ButcherProjectView::ITEM_IMAGES;
        if (itemimagesglobalctrl->GetValue()) items_|=ButcherProjectView::ITEM_IMAGESGLOBAL;
        if (items_==0)
        {
            wxMessageBox(_("At least one item must be selected."), _("Error"), wxOK|wxICON_EXCLAMATION);
            return false;
        }

        assortedfilegroups_.Clear();
        int ct=0;
		for (ButcherProjectAssortedFileGroups::iterator i=GetProject()->AssortedFileGroups().begin(); 
			i!=GetProject()->AssortedFileGroups().end(); i++)
		{
            if (groupsctrl->IsChecked(ct))
                assortedfilegroups_.Add(i.first());
            ct++;
		}

        bgimage_.SetBURL(bgimagectrl->GetFileLink()->GetBURL());
        bgrepeat_=bgrepeatimap_.GetId(bgrepeatctrl->GetSelection(), ButcherProjectAreaConfig::BR_NONE);
        isbgcolor_=isbgcolorctrl->GetValue();
        isstretch_=isstretchctrl->GetValue();
		styleseparated_=styleseparatedctrl->GetValue();
        bgcolor_=bgcolorctrl->GetColour();

    }

    return wxDialog::TransferDataFromWindow();
}



void HTMLButcherViewEditDialog::OnBtnCSS(wxCommandEvent &event)
{
    wxComboBox *cssfilectrl = (wxComboBox*) FindWindow(ID_CSSFILE);

    BLID_t cid=wxccu_control_getselectedid(cssfilectrl, 0);
    if (cid>0)
    {
        HTMLButcherPopupCSSEditDialog d(this, wxID_ANY, GetProject());
        d.SetCSSFileId(cid);
        d.ShowModal();
    }
}



void HTMLButcherViewEditDialog::OnCtrlChanged(wxCommandEvent &event)
{
	UpdateAppState();
}

void HTMLButcherViewEditDialog::UpdateAppState()
{
	wxRadioBox *layouttypectrl = (wxRadioBox*) FindWindow(ID_LAYOUTTYPE);
	ButcherProjectConsts::layouttype_t layouttype=layouttypeimap_.GetId(layouttypectrl->GetSelection(), ButcherProjectConsts::LTYPE_DEFAULT);

	FindWindow(ID_TABLETAGADD)->Enable(layouttype!=ButcherProjectConsts::LTYPE_APTABLELESS);
	//FindWindow(ID_STYLESEPARATED)->Enable(layouttype==ButcherProjectConsts::LTYPE_APTABLELESS);
}
