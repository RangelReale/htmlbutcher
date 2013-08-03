//=============================================================================
/**
 *  @file    DialogMaskAreaImageFormat.cpp
 *
 *  $Id: DialogMaskAreaImageFormat.cpp,v 1.11 2008/05/22 17:18:26 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-28
 */
//=============================================================================
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/statline.h>
#include <wx/valgen.h>
#include <wx/stattext.h>
#include <wx/htmllbox.h>
#include <wx/filename.h>
#include <memory>
#include <cppcomp/ccustring.h>

#include "DialogMaskAreaImageFormat.h"



using namespace std;
using namespace cppcomp;


/**
 * @class ButcherColorClientData
 *
 * @brief Color wxClientData
 */
class ButcherColorClientData : public wxClientData
{
public:
    ButcherColorClientData(const wxColour &color) :
        wxClientData(), color_(color) {}

    const wxColour &GetColor() { return color_; }
private:
    wxColour color_;
};



/**
 * @class ButcherPointClientData
 *
 * @brief Point wxClientData
 */
class ButcherPointClientData : public wxClientData
{
public:
    ButcherPointClientData(const wxPoint &point) :
        wxClientData(), point_(point) {}

    const wxPoint &GetPoint() { return point_; }
private:
    wxPoint point_;
};




BEGIN_EVENT_TABLE(HTMLButcherMaskAreaImageFormatDialog, ButcherControl_Dialog)
    EVT_COMBOBOX(ID_IMAGEFORMAT, HTMLButcherMaskAreaImageFormatDialog::OnImageFormatPreview)
    EVT_COMBOBOX(ID_PREVIEW2FORMAT, HTMLButcherMaskAreaImageFormatDialog::OnImageFormatPreview)
    EVT_LISTBOX_DCLICK(ID_TRANSPARENTCOLOR, HTMLButcherMaskAreaImageFormatDialog::OnTColorDClick)
    EVT_LISTBOX_DCLICK(ID_TRANSPARENTPIXEL, HTMLButcherMaskAreaImageFormatDialog::OnTPixelDClick)
END_EVENT_TABLE()



HTMLButcherMaskAreaImageFormatDialog::HTMLButcherMaskAreaImageFormatDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style), file_(NULL),
    area_(NULL), tcolors_(), tpixels_()
{
    Init();
    CreateControls();
}




void HTMLButcherMaskAreaImageFormatDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-imageformatprop.html"));
#endif

	imageformat_=0;
}




void HTMLButcherMaskAreaImageFormatDialog::Load(ButcherProjectAreaImageInfo *imageinfo)
{
    imageformat_=imageinfo->GetImageFormatId();
    tcolors_=imageinfo->TransparentColors();
    tpixels_=imageinfo->TransparentPixels();
}




void HTMLButcherMaskAreaImageFormatDialog::Save(ButcherProjectAreaImageInfo *imageinfo)
{
    ButcherProjectBaseAutoUpdate upd(imageinfo);

    imageinfo->SetImageFormatId(imageformat_);
    imageinfo->TransparentColorsModify()=tcolors_;
    imageinfo->TransparentPixelsModify()=tpixels_;
}



void HTMLButcherMaskAreaImageFormatDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 2);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 2);

    // PREVIEW
    wxStaticBoxSizer *previewsbsizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Preview"));
    bodysizer->Add(previewsbsizer, 1, wxEXPAND|wxALL, 2);


    wxBoxSizer *previewsizer = new wxBoxSizer(wxVERTICAL);
    previewsbsizer->Add(previewsizer, 1, wxEXPAND|wxALL, 2);

    // selection type
    wxArrayString seltype;
    seltype.Add(_("Transparent Color"));
    seltype.Add(_("Transparent Pixel"));

    wxRadioBox *selectionctrl = new wxRadioBox(this, ID_TRANSPARENTSELECTION, _("&Selection type: (double-click)"),
        wxDefaultPosition, wxDefaultSize, seltype, 2);
    selectionctrl->SetSelection(0);
    previewsizer->Add(selectionctrl, 0, wxGROW|wxALL, 2);


    // preview 1 image format
    previewsizer->Add(new wxStaticText(this, wxID_STATIC, _("Image &Format:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 2);

    wxComboBox *p1formatctrl=new wxComboBox(this, ID_IMAGEFORMAT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY|wxCB_SORT);
    previewsizer->Add(p1formatctrl, 0, wxGROW|wxALL, 2);


    // preview image 1
    //wxStaticText *previewlabel = new wxStaticText(this, wxID_STATIC, _("Area:"), wxDefaultPosition, wxDefaultSize, 0);
    //previewsizer->Add(previewlabel, 0, wxALIGN_LEFT|wxALL, 2);

    preview1ctrl_ = new ButcherImageView(this, ID_PREVIEW1, wxDefaultPosition, wxSize(400, 200));
    preview1ctrl_->SetSelectColor(true);
    preview1ctrl_->Connect(wxID_ANY, wxEVT_BUTCHERIMAGEVIEWCOLOR_ACTION,
        ButcherImageViewColorEventHandler(HTMLButcherMaskAreaImageFormatDialog::OnSelectColor), NULL, this);
    //preview1ctrl_->SetFullSize(true);
    preview1ctrl_->SetMargin(10);
    preview1ctrl_->SetBGColor(wxTheColourDatabase->Find(wxT("MEDIUM VIOLET RED")));

    previewsizer->Add(preview1ctrl_, 1, wxEXPAND|wxALL, 2);

    // preview 2 image format
    previewsizer->Add(new wxStaticText(this, wxID_STATIC, _("&Preview Image Format: (comparisson only)"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 2);

    wxComboBox *p2formatctrl=new wxComboBox(this, ID_PREVIEW2FORMAT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY|wxCB_SORT);
    previewsizer->Add(p2formatctrl, 0, wxGROW|wxALL, 2);


    // preview image 2
    preview2ctrl_ = new ButcherImageView(this, ID_PREVIEW2, wxDefaultPosition, wxSize(400, 200));
    preview2ctrl_->SetSelectColor(true);
    preview2ctrl_->Connect(wxID_ANY, wxEVT_BUTCHERIMAGEVIEWCOLOR_ACTION,
        ButcherImageViewColorEventHandler(HTMLButcherMaskAreaImageFormatDialog::OnSelectColor), NULL, this);
    //preview2ctrl_->SetFullSize(true);
    preview2ctrl_->SetMargin(10);
    preview2ctrl_->SetBGColor(wxTheColourDatabase->Find(wxT("MEDIUM VIOLET RED")));

    previewsizer->Add(preview2ctrl_, 1, wxEXPAND|wxALL, 2);

    // PREVIEW COLOR
    wxBoxSizer *previewcolorsizer = new wxBoxSizer(wxHORIZONTAL);


    // hover color
    previewcolorsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("Hover:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 2);

    wxTextCtrl *pchoverctrl = new wxTextCtrl(this, ID_PROC_HOVERCOLOR, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    previewcolorsizer->Add(pchoverctrl, 1, wxGROW|wxALL, 2);

    // selected color
    previewcolorsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("Selected:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 2);

    wxTextCtrl *pcselectedctrl = new wxTextCtrl(this, ID_PROC_SELCOLOR, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    previewcolorsizer->Add(pcselectedctrl, 1, wxGROW|wxALL, 2);


    previewsizer->Add(previewcolorsizer, 0, wxEXPAND|wxALL, 2);

    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(fieldssizer, 0, wxEXPAND|wxALL, 2);

    // transparent colors
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, _("&Transparent Colors:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 2);

    wxSimpleHtmlListBox *transparentcolorctrl = new wxSimpleHtmlListBox(this, ID_TRANSPARENTCOLOR);
    fieldssizer->Add(transparentcolorctrl, 1, wxGROW|wxALL, 2);

    // transparent pixels
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, _("Transparent &Pixels:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 2);

    wxListBox *transparentpixelctrl = new wxListBox(this, ID_TRANSPARENTPIXEL, wxDefaultPosition, wxDefaultSize,
        0, NULL, wxLB_SORT);
    fieldssizer->Add(transparentpixelctrl, 1, wxGROW|wxALL, 2);


    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 2);

    // BUTTONS
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    // cancel button
    wxButton* cancel = new wxButton ( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

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



bool HTMLButcherMaskAreaImageFormatDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxComboBox *p1formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);
    wxComboBox *p2formatctrl=(wxComboBox*)FindWindow(ID_PREVIEW2FORMAT);
    p1formatctrl->Clear();
    p2formatctrl->Clear();
    p1formatctrl->SetSelection(p1formatctrl->Append(_("- DEFAULT -"), static_cast<wxClientData *>(new ButcherListIdClientData(0))));
    p2formatctrl->SetSelection(p2formatctrl->Append(_("- DEFAULT -"), static_cast<wxClientData *>(new ButcherListIdClientData(0))));

    auto_ptr<ButcherImage> origimage(file_->GetImage()->SubImage(area_->GetGlobalRect())), fmtimage(NULL);

    ButcherProjectBaseAutoProgress prog(GetProject(), _("Calculating file sizes, please wait..."));

    int newi;
    wxString fmtdesc;
	for (ButcherProjectImageFormats::iterator i=file_->GetProject()->ImageFormats().begin(); 
		i!=file_->GetProject()->ImageFormats().end(); i++)
	{
        fmtimage.reset(origimage->Save(i->GetFormat(), i->GetFlags()));
        fmtdesc=wxString::Format(wxT("%s   [%s - %s bytes]"), i->GetName().c_str(),
            wxFileName::GetHumanReadableSize(fmtimage->GetFileSize(), wxT("0")).c_str(),
                    wxString(ccustring_numberformat(fmtimage->GetFileSize(), NF_THOUSANDS).c_str(), wxConvUTF8).c_str());

        newi=p1formatctrl->Append(fmtdesc,
            static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
        if (i.first()==imageformat_)
            p1formatctrl->SetSelection(newi);
        p2formatctrl->Append(fmtdesc,
            static_cast<wxClientData *>(new ButcherListIdClientData(i.first())));
	}

    LoadTColors();
    LoadTPixels();

    UpdateDisplay(true);
    return true;
}




bool HTMLButcherMaskAreaImageFormatDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);

    if (formatctrl->GetSelection()!=wxNOT_FOUND)
        imageformat_=static_cast<ButcherListIdClientData*>(formatctrl->GetClientObject(formatctrl->GetSelection()))->GetId();
    else
        imageformat_=0;

    SaveTColors();
    SaveTPixels();

    return true;
}




void HTMLButcherMaskAreaImageFormatDialog::UpdateDisplay(bool isfirst)
{
    wxComboBox *p1formatctrl=(wxComboBox*)FindWindow(ID_IMAGEFORMAT);
    wxComboBox *p2formatctrl=(wxComboBox*)FindWindow(ID_PREVIEW2FORMAT);

    BLID_t p1fmt=0, p2fmt=0;
    if (p1formatctrl->GetSelection()!=wxNOT_FOUND)
        p1fmt=static_cast<ButcherListIdClientData*>(p1formatctrl->GetClientObject(p1formatctrl->GetSelection()))->GetId();
    if (p2formatctrl->GetSelection()!=wxNOT_FOUND)
        p2fmt=static_cast<ButcherListIdClientData*>(p2formatctrl->GetClientObject(p2formatctrl->GetSelection()))->GetId();

    ButcherImage *preview=file_->GetImage()->SubImage(
        area_->GetGlobalRect()
    );

    SaveTColors();
    SaveTPixels();

/*
    ButcherImage::transparentcolors_t tcolors;
    tcolors.push_back(wxColour(wxT("#66282A")));
    tcolors.push_back(wxColour(wxT("#67282A")));
    tcolors.push_back(wxColour(0xFF, 0xFF, 0xFF));
*/

    if (p1fmt>0)
        preview1ctrl_->SetImage(preview->Save(file_->GetProject()->ImageFormats().Get(p1fmt)->GetFormat(),
            file_->GetProject()->ImageFormats().Get(p1fmt)->GetFlags(), &tcolors_, &tpixels_));
    else
        preview1ctrl_->SetImage(preview->Clone());
    if (p2fmt>0)
        preview2ctrl_->SetImage(preview->Save(file_->GetProject()->ImageFormats().Get(p2fmt)->GetFormat(),
            file_->GetProject()->ImageFormats().Get(p2fmt)->GetFlags(), &tcolors_, &tpixels_));
    else
        preview2ctrl_->SetImage(preview->Clone());

    delete preview;
}




void HTMLButcherMaskAreaImageFormatDialog::OnSelectColor(ButcherImageViewColorEvent &event)
{
    wxRadioBox *selectionctrl = (wxRadioBox*)FindWindow(ID_TRANSPARENTSELECTION);

    int cid=ID_PROC_HOVERCOLOR;
    if (event.GetSelect()==ButcherImageViewColorEvent::CS_SELECT ||
        event.GetSelect()==ButcherImageViewColorEvent::CS_CONFIRM)
    {
        cid=ID_PROC_SELCOLOR;
        if (event.GetSelect()==ButcherImageViewColorEvent::CS_CONFIRM)
        {
            if (selectionctrl->GetSelection()==0)
                AddTColor(event.GetColor());
            else
                AddTPixel(event.GetPixel());
            UpdateDisplay();
        }
    }
    wxTextCtrl *ct=(wxTextCtrl*)FindWindow(cid);

    // some combinations of the fg/bg colours may be unreadable, so we invert
    // the colour to make sure fg colour is different enough from m_colour
    wxColour colFg(~event.GetColor().Red(), ~event.GetColor().Green(), ~event.GetColor().Blue());

    ct->SetForegroundColour(colFg);
    ct->SetBackgroundColour(event.GetColor());
    ct->SetValue(wxString::Format(wxT("%s (%d,%d)"),
        event.GetColor().GetAsString(wxC2S_HTML_SYNTAX).c_str(),
        event.GetPixel().x, event.GetPixel().y));

    event.Skip();
}




void HTMLButcherMaskAreaImageFormatDialog::OnImageFormatPreview(wxCommandEvent &event)
{
    UpdateDisplay();
    event.Skip();
}



void HTMLButcherMaskAreaImageFormatDialog::AddTColor(const wxColour &color)
{
    wxSimpleHtmlListBox *tcolorsctrl=(wxSimpleHtmlListBox*)FindWindow(ID_TRANSPARENTCOLOR);

    // dont allow duplicates
    for (unsigned int i=0; i<tcolorsctrl->GetCount(); i++)
    {
        if (static_cast<ButcherColorClientData*>(tcolorsctrl->GetClientObject(i))->GetColor()==color)
            return;
    }


    // some combinations of the fg/bg colours may be unreadable, so we invert
    // the colour to make sure fg colour is different enough from m_colour
    wxColour colFg(~color.Red(), ~color.Green(), ~color.Blue());


    tcolorsctrl->Append(wxString::Format(wxT("<table bgcolor=\"%s\" width=\"100%%\"><tr><td><font color=\"%s\">%s</font></td></tr></table>"),
        color.GetAsString(wxC2S_HTML_SYNTAX).c_str(),
        colFg.GetAsString(wxC2S_HTML_SYNTAX).c_str(),
        color.GetAsString(wxC2S_HTML_SYNTAX).c_str()),
        new ButcherColorClientData(color));
}




void HTMLButcherMaskAreaImageFormatDialog::LoadTColors()
{
    wxSimpleHtmlListBox *tcolorsctrl=(wxSimpleHtmlListBox*)FindWindow(ID_TRANSPARENTCOLOR);
    tcolorsctrl->Clear();

    for (ButcherImage::transparentcolors_t::const_iterator i=tcolors_.begin(); i!=tcolors_.end(); i++)
    {
        AddTColor(*i);
    }
}




void HTMLButcherMaskAreaImageFormatDialog::SaveTColors()
{
    wxSimpleHtmlListBox *tcolorsctrl=(wxSimpleHtmlListBox*)FindWindow(ID_TRANSPARENTCOLOR);

    tcolors_.clear();
    for (unsigned int i=0; i<tcolorsctrl->GetCount(); i++)
    {
        tcolors_.push_back(static_cast<ButcherColorClientData*>(tcolorsctrl->GetClientObject(i))->GetColor());
    }

}




void HTMLButcherMaskAreaImageFormatDialog::OnTColorDClick(wxCommandEvent &event)
{
    wxSimpleHtmlListBox *tcolorsctrl=(wxSimpleHtmlListBox*)FindWindow(ID_TRANSPARENTCOLOR);
    if (tcolorsctrl->GetSelection()!=wxNOT_FOUND)
    {
        tcolorsctrl->Delete(tcolorsctrl->GetSelection());
        UpdateDisplay();
    }
    event.Skip();
}



void HTMLButcherMaskAreaImageFormatDialog::AddTPixel(const wxPoint &pixel)
{
    wxListBox *tpixelsctrl=(wxListBox*)FindWindow(ID_TRANSPARENTPIXEL);

    // don't allow duplicates
    for (unsigned int i=0; i<tpixelsctrl->GetCount(); i++)
    {
        if (static_cast<ButcherPointClientData*>(tpixelsctrl->GetClientObject(i))->GetPoint()==pixel)
            return;
    }

    tpixelsctrl->Append(wxString::Format(wxT("(%d, %d)"),
        pixel.x,
        pixel.y),
        new ButcherPointClientData(pixel));
}




void HTMLButcherMaskAreaImageFormatDialog::LoadTPixels()
{
    wxListBox *tpixelsctrl=(wxListBox*)FindWindow(ID_TRANSPARENTPIXEL);
    tpixelsctrl->Clear();

    for (ButcherImage::transparentpixels_t::const_iterator i=tpixels_.begin(); i!=tpixels_.end(); i++)
    {
        AddTPixel(*i);
    }
}




void HTMLButcherMaskAreaImageFormatDialog::SaveTPixels()
{
    wxListBox *tpixelsctrl=(wxListBox*)FindWindow(ID_TRANSPARENTPIXEL);

    tpixels_.clear();
    for (unsigned int i=0; i<tpixelsctrl->GetCount(); i++)
    {
        tpixels_.push_back(static_cast<ButcherPointClientData*>(tpixelsctrl->GetClientObject(i))->GetPoint());
    }

}




void HTMLButcherMaskAreaImageFormatDialog::OnTPixelDClick(wxCommandEvent &event)
{
    wxListBox *tpixelsctrl=(wxListBox*)FindWindow(ID_TRANSPARENTPIXEL);
    if (tpixelsctrl->GetSelection()!=wxNOT_FOUND)
    {
        tpixelsctrl->Delete(tpixelsctrl->GetSelection());
        UpdateDisplay();
    }
    event.Skip();
}


