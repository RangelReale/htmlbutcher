//=============================================================================
/**
 *  @file    DialogImageFormatEdit.cpp
 *
 *  $Id: DialogImageFormatEdit.cpp,v 1.10 2008/02/08 13:24:46 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-19
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
#include <wx/filename.h>

#include "DialogImageFormatEdit.h"
#include "BList.h"
#include <cppcomp/ccustring.h>
#include "BUtil.h"



using namespace cppcomp;






/////////////////////////////////
// CLASS
//      HTMLButcherImageFormatListEditDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherImageFormatListEditDialog, HTMLButcherListEditDialog)
    EVT_CHECKBOX(ID_AUTOUPDATE, HTMLButcherImageFormatListEditDialog::OnAutoUpdate)
END_EVENT_TABLE()

HTMLButcherImageFormatListEditDialog::~HTMLButcherImageFormatListEditDialog()
{
    if (preview_) delete preview_;
    if (previewformat_) delete previewformat_;
}



void HTMLButcherImageFormatListEditDialog::do_createcontrols_preview()
{
    // PREVIEW
    previewsizer_ = new wxBoxSizer(wxVERTICAL);

    // preview image
    previewlabel_ = new wxStaticText(this, wxID_STATIC, _("Preview (Original):"), wxDefaultPosition, wxDefaultSize, 0);
    previewsizer_->Add(previewlabel_, 0, wxALIGN_LEFT|wxALL, 3);

    previewctrl_ = new ButcherImageView(this, wxID_ANY, wxPoint(0, 0), wxSize(300, 300));
    previewctrl_->SetFullSize(true);

    previewsizer_->Add(previewctrl_, 1, wxEXPAND|wxALL, 3);

    // AUTO UPDATE
    wxCheckBox *autoupdatectrl=new wxCheckBox(this, ID_AUTOUPDATE, _("Auto-update"));
    //autoupdatectrl->SetValue(true);
    previewsizer_->Add(autoupdatectrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);


    GetBodySizer()->Prepend(previewsizer_, 2, wxEXPAND|wxALL|wxFIXED_MINSIZE, 3);

    ControlsChanged();
}




void HTMLButcherImageFormatListEditDialog::do_load()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-imageformats.html"));
#endif

	for (ButcherProjectImageFormats::iterator i=GetProject()->ImageFormats().begin(); 
		i!=GetProject()->ImageFormats().end(); i++)
	{
        item_add(i->GetName(), i.first());
	}
}




void HTMLButcherImageFormatListEditDialog::do_add()
{
    HTMLButcherImageFormatEditDialog d(this, wxID_ANY, GetProject());
    if (preview_)
        d.SetPreview(preview_->Clone());
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        BLID_t newid=GetProject()->ImageFormats().Add(d.GetFormatName(),
            d.GetFormat(), d.GetFlags());
        item_add(GetProject()->ImageFormats().Get(newid)->GetName(), newid);
    }
}




void HTMLButcherImageFormatListEditDialog::do_edit(unsigned long id)
{
    HTMLButcherImageFormatEditDialog d(this, wxID_ANY, GetProject());
    d.SetFormatName(GetProject()->ImageFormats().Get(id)->GetName());
    d.SetFormat(GetProject()->ImageFormats().Get(id)->GetFormat());
    d.SetFlags(GetProject()->ImageFormats().Get(id)->GetFlags());

    if (preview_)
        d.SetPreview(preview_->Clone());

    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(GetProject());
        GetProject()->ImageFormats().Edit(id, d.GetFormatName(), d.GetFormat(),
            d.GetFlags());

        item_edit(id, GetProject()->ImageFormats().Get(id)->GetName());
    }
}



void HTMLButcherImageFormatListEditDialog::do_remove(unsigned long id)
{
    // get number of views using this mask
    ButcherProjectEventNotify en(GetProject(), ButcherProjectEvent::BPE_IMAGEFORMATDELETED, id, 0, false);
    GetProject()->ExecuteEventNotify(en);

    wxString remmsg=_("Are you sure you want to remove this image format?");
    if (en.GetRemoveCount()>0||en.GetChangeCount()>0)
        remmsg=wxString::Format(_("Removing this image format will affect %d item(s). Are you sure?"), en.GetRemoveCount()+en.GetChangeCount());

    wxMessageDialog d(this, remmsg, _("Remove image format"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (d.ShowModal() != wxID_YES) return;

    ButcherProjectBaseAutoUpdate upd(GetProject());
    if (!GetProject()->ImageFormats().Delete(id))
    {
        butil_errordialog(_("This image format cannot be deleted"), this);
        return;
    }

    item_remove(id);
}




void HTMLButcherImageFormatListEditDialog::do_select(BLID_t id)
{
    if (!preview_) return;

    wxCheckBox *cb=(wxCheckBox*)FindWindow(ID_AUTOUPDATE);
    if (cb->GetValue())
        UpdatePreview(id);
}




void HTMLButcherImageFormatListEditDialog::SetPreview(ButcherImage *preview)
{
    if (preview_) delete preview_;
    preview_=preview;
    if (preview_) {
        do_createcontrols_preview();
        previewctrl_->SetImage(preview_, false);
    }
}




void HTMLButcherImageFormatListEditDialog::UpdatePreview()
{
    if (item_current() > 0)
        UpdatePreview(item_current());
}




void HTMLButcherImageFormatListEditDialog::UpdatePreview(BLID_t id)
{
    SetCursor(*wxHOURGLASS_CURSOR);
    try
    {
        if (preview_) {
            if (previewformat_) {
                delete previewformat_;
                previewformat_=NULL;
            }
            previewformat_=preview_->Save(GetProject()->ImageFormats().Get(id)->GetFormat(),
                GetProject()->ImageFormats().Get(id)->GetFlags());
            previewlabel_->SetLabel(wxString::Format(_("Preview (%s - size %s [%s bytes]):"),
                GetProject()->ImageFormats().Get(id)->GetName().c_str(),
                    wxFileName::GetHumanReadableSize(previewformat_->GetFileSize(), wxT("0")).c_str(),
                    wxString(ccustring_numberformat(previewformat_->GetFileSize(), NF_THOUSANDS).c_str(), wxConvUTF8).c_str()));

            previewctrl_->SetImage(previewformat_, false);
        }
    } catch(...) {
        SetCursor(*wxSTANDARD_CURSOR);
        throw;
    }
    SetCursor(*wxSTANDARD_CURSOR);
}



void HTMLButcherImageFormatListEditDialog::OnAutoUpdate(wxCommandEvent &event)
{
    if (event.IsChecked())
        UpdatePreview();
    event.Skip();
}




/////////////////////////////////
// CLASS
//      HTMLButcherImageFormatEditDialog
/////////////////////////////////

BEGIN_EVENT_TABLE(HTMLButcherImageFormatEditDialog, ButcherControl_Dialog)
    EVT_CHECKBOX(ID_AUTOUPDATE, HTMLButcherImageFormatEditDialog::OnAutoUpdate)
    EVT_COMBOBOX(ID_FORMAT, HTMLButcherImageFormatEditDialog::OnFormatChange)

    EVT_COMBOBOX(ID_FORMAT, HTMLButcherImageFormatEditDialog::OnCtrlChanged)
END_EVENT_TABLE()

HTMLButcherImageFormatEditDialog::HTMLButcherImageFormatEditDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style),
        //formatmap_(), flagsmap_(),
        imageformatimap_(), jpegcompressionimap_(), quantizationimap_(), forcebppimap_(),
        preview_(NULL), previewformat_()
{
    Init();
    CreateControls();
}




HTMLButcherImageFormatEditDialog::~HTMLButcherImageFormatEditDialog()
{
    if (previewformat_) delete previewformat_;
    if (preview_) delete preview_;
}



void HTMLButcherImageFormatEditDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-imageformats.html"));
#endif

	format_=ButcherImage::FMT_JPG;
    flags_=ButcherImage::JPG_QUAL_75;

    imageformatimap_.Add(ButcherImage::FMT_JPG, wxT("JPEG"));
    imageformatimap_.Add(ButcherImage::FMT_GIF, wxT("GIF"));
    imageformatimap_.Add(ButcherImage::FMT_PNG, wxT("PNG"));
	imageformatimap_.Add(ButcherImage::FMT_BMP, wxT("BMP"));
	imageformatimap_.Add(ButcherImage::FMT_TIFF, wxT("TIFF"));
	imageformatimap_.Add(ButcherImage::FMT_XPM, wxT("XPM"));

    jpegcompressionimap_.Add(ButcherImage::GEN_NOFLAG, _("Use default compression"));
    jpegcompressionimap_.Add(ButcherImage::JPG_QUAL_100, _("Maximum (100%)"));
    jpegcompressionimap_.Add(ButcherImage::JPG_QUAL_75, _("Normal (75%)"));
    jpegcompressionimap_.Add(ButcherImage::JPG_QUAL_50, _("Medium (50%)"));
    jpegcompressionimap_.Add(ButcherImage::JPG_QUAL_25, _("Low (25%)"));
    jpegcompressionimap_.Add(ButcherImage::JPG_QUAL_10, _("Minimum (10%)"));

    quantizationimap_.Add(ButcherImage::GEN_NOFLAG, _("Use default quantization"));
    quantizationimap_.Add(ButcherImage::GEN_QUANT_WU, _("Xialon Wu"));
    quantizationimap_.Add(ButcherImage::GEN_QUANT_NN, _("NeuQuant"));

    forcebppimap_.Add(ButcherImage::GEN_NOFLAG, _("Use default bit depth"));
    forcebppimap_.Add(ButcherImage::GEN_FORCE8BIT, _("8 bit"));
    forcebppimap_.Add(ButcherImage::GEN_FORCE16BIT, _("16 bit"));
    forcebppimap_.Add(ButcherImage::GEN_FORCE24BIT, _("24 bit"));
    forcebppimap_.Add(ButcherImage::GEN_FORCE32BIT, _("32 bit"));
}



void HTMLButcherImageFormatEditDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // BODY
    bodysizer_ = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer_, 1, wxEXPAND|wxALL, 3);




    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
    bodysizer_->Add(fieldssizer, 0, /*wxEXPAND|*/wxALL, 3);

    // Name
    wxStaticText *namelabel = new wxStaticText(this, wxID_STATIC, _("&Name:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(namelabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxTextCtrl *namectrl = new wxTextCtrl(this, ID_NAME, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0);
    fieldssizer->Add(namectrl, 0, wxGROW|wxALL, 3);

    // format
    wxStaticText *formatlabel = new wxStaticText(this, wxID_STATIC, _("&Format:"), wxDefaultPosition, wxDefaultSize, 0);
    fieldssizer->Add(formatlabel, 0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *formatctrl=new wxComboBox(this, ID_FORMAT, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);
    imageformatimap_.Load(formatctrl);

    fieldssizer->Add(formatctrl, 0, wxGROW|wxALL, 3);

    // JPEG compression
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, _("&JPEG compression:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *jpegcompressionctrl=new wxComboBox(this, ID_JPEGCOMPRESSION, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);
    jpegcompressionimap_.Load(jpegcompressionctrl);

    fieldssizer->Add(jpegcompressionctrl, 0, wxGROW|wxALL, 3);

    // JPEG progressive
    wxCheckBox *jpegprogressivectrl = new wxCheckBox(this, ID_JPEGPROGRESSIVE, _("JPEG pro&gressive"));
    fieldssizer->Add(jpegprogressivectrl, 0, wxGROW|wxALL, 3);

    // Quantization
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, _("GIF/PNG8 &quantization: (reduction to 8 bit)")),
        0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *quantizationctrl=new wxComboBox(this, ID_QUANTIZATON, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);
    quantizationimap_.Load(quantizationctrl);

    fieldssizer->Add(quantizationctrl, 0, wxGROW|wxALL, 3);

    // Force bit depth
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, _("Force bit &depth if possible:")),
        0, wxALIGN_LEFT|wxALL, 3);

    wxComboBox *forcebppctrl=new wxComboBox(this, ID_FORCEBPP, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxArrayString(), wxCB_READONLY);
    forcebppimap_.Load(forcebppctrl);

    fieldssizer->Add(forcebppctrl, 0, wxGROW|wxALL, 3);


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
    FindWindow(ID_NAME)->SetValidator(wxTextValidator(wxFILTER_NONE, &formatname_));
    //FindWindow(ID_WIDTH)->SetValidator(wxGenericValidator((int*)&maskwidth_));
    //FindWindow(ID_HEIGHT)->SetValidator(wxGenericValidator((int*)&maskheight_));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



void HTMLButcherImageFormatEditDialog::do_createcontrols_preview()
{
    // PREVIEW
    previewsizer_ = new wxBoxSizer(wxVERTICAL);

    // preview image
    previewlabel_ = new wxStaticText(this, wxID_STATIC, _("Preview (Original):"), wxDefaultPosition, wxDefaultSize, 0);
    previewsizer_->Add(previewlabel_, 0, wxALIGN_LEFT|wxALL, 3);

    previewctrl_ = new ButcherImageView(this, wxID_ANY, wxPoint(0, 0), wxSize(400, 300));
    previewctrl_->SetFullSize(true);

    previewsizer_->Add(previewctrl_, 1, wxEXPAND|wxALL, 3);


    // AUTO UPDATE
    wxCheckBox *autoupdatectrl=new wxCheckBox(this, ID_AUTOUPDATE, _("Auto-update"));
    previewsizer_->Add(autoupdatectrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    bodysizer_->Prepend(previewsizer_, 1, wxEXPAND|wxALL, 3);

    GetSizer()->SetSizeHints(this);

    CentreOnScreen();
}




void HTMLButcherImageFormatEditDialog::SetPreview(ButcherImage *preview)
{
    if (preview_) delete preview_;

    preview_=preview;
    if (preview_) {
        do_createcontrols_preview();

        previewctrl_->SetImage(preview_, false);
    }
}





ButcherImage::format_t HTMLButcherImageFormatEditDialog::DialogFormat()
{
    wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_FORMAT);

    return imageformatimap_.GetId(formatctrl->GetSelection(), ButcherImage::FMT_JPG);
}




int HTMLButcherImageFormatEditDialog::DialogFlags()
{
    wxComboBox *jpegcompressionctrl=(wxComboBox*)FindWindow(ID_JPEGCOMPRESSION);
    wxCheckBox *jpegprogressivectrl=(wxCheckBox*)FindWindow(ID_JPEGPROGRESSIVE);
    wxComboBox *quantizationctrl=(wxComboBox*)FindWindow(ID_QUANTIZATON);
    wxComboBox *forcebppctrl=(wxComboBox*)FindWindow(ID_FORCEBPP);

    int ret=0;

    ret|=jpegcompressionimap_.GetId(jpegcompressionctrl->GetSelection(), ButcherImage::GEN_NOFLAG);
    if (jpegprogressivectrl->GetValue()) ret|=ButcherImage::JPG_PROGRESSIVE;
    ret|=quantizationimap_.GetId(quantizationctrl->GetSelection(), ButcherImage::GEN_NOFLAG);
    ret|=forcebppimap_.GetId(forcebppctrl->GetSelection(), ButcherImage::GEN_NOFLAG);
    return ret;
}




bool HTMLButcherImageFormatEditDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxComboBox *formatctrl=(wxComboBox*)FindWindow(ID_FORMAT);
    wxComboBox *jpegcompressionctrl=(wxComboBox*)FindWindow(ID_JPEGCOMPRESSION);
    wxCheckBox *jpegprogressivectrl=(wxCheckBox*)FindWindow(ID_JPEGPROGRESSIVE);
    wxComboBox *quantizationctrl=(wxComboBox*)FindWindow(ID_QUANTIZATON);
    wxComboBox *forcebppctrl=(wxComboBox*)FindWindow(ID_FORCEBPP);

    //formatctrl->SetSelection(formatmap_[format_]);
    formatctrl->SetSelection(imageformatimap_.GetIndex(format_));

    ButcherImage::flags_t loadflags;

    loadflags=ButcherImage::GEN_NOFLAG;
    if ((flags_&ButcherImage::JPG_QUAL_100)==ButcherImage::JPG_QUAL_100) loadflags=ButcherImage::JPG_QUAL_100;
    else if ((flags_&ButcherImage::JPG_QUAL_75)==ButcherImage::JPG_QUAL_75) loadflags=ButcherImage::JPG_QUAL_75;
    else if ((flags_&ButcherImage::JPG_QUAL_50)==ButcherImage::JPG_QUAL_50) loadflags=ButcherImage::JPG_QUAL_50;
    else if ((flags_&ButcherImage::JPG_QUAL_25)==ButcherImage::JPG_QUAL_25) loadflags=ButcherImage::JPG_QUAL_25;
    else if ((flags_&ButcherImage::JPG_QUAL_10)==ButcherImage::JPG_QUAL_10) loadflags=ButcherImage::JPG_QUAL_10;
    jpegcompressionctrl->SetSelection(jpegcompressionimap_.GetIndex(loadflags));
    jpegprogressivectrl->SetValue((flags_&ButcherImage::JPG_PROGRESSIVE)==ButcherImage::JPG_PROGRESSIVE);

    loadflags=ButcherImage::GEN_NOFLAG;
    if ((flags_&ButcherImage::GEN_QUANT_WU)==ButcherImage::GEN_QUANT_WU) loadflags=ButcherImage::GEN_QUANT_WU;
    else if ((flags_&ButcherImage::GEN_QUANT_NN)==ButcherImage::GEN_QUANT_NN) loadflags=ButcherImage::GEN_QUANT_NN;
    quantizationctrl->SetSelection(quantizationimap_.GetIndex(loadflags));

    loadflags=ButcherImage::GEN_NOFLAG;
    if ((flags_&ButcherImage::GEN_FORCE8BIT)==ButcherImage::GEN_FORCE8BIT) loadflags=ButcherImage::GEN_FORCE8BIT;
    else if ((flags_&ButcherImage::GEN_FORCE16BIT)==ButcherImage::GEN_FORCE16BIT) loadflags=ButcherImage::GEN_FORCE16BIT;
    else if ((flags_&ButcherImage::GEN_FORCE24BIT)==ButcherImage::JPG_QUAL_50) loadflags=ButcherImage::GEN_FORCE24BIT;
    else if ((flags_&ButcherImage::GEN_FORCE32BIT)==ButcherImage::GEN_FORCE32BIT) loadflags=ButcherImage::GEN_FORCE32BIT;
    forcebppctrl->SetSelection(forcebppimap_.GetIndex(loadflags));

    UpdatePreview();

    UpdateAppState();

    return true;
}



bool HTMLButcherImageFormatEditDialog::TransferDataFromWindow()
{
    // check
    wxTextCtrl *namectrl = (wxTextCtrl*)FindWindow(ID_NAME);

    if (namectrl->GetValue().IsEmpty())
    {
        butil_errordialog(_("Name cannot be blank"), this);
        return false;
    }

    // save
    if (!wxDialog::TransferDataFromWindow()) return false;

    format_=DialogFormat();
    flags_=DialogFlags();

    return true;
}




void HTMLButcherImageFormatEditDialog::UpdatePreview()
{
    SetCursor(*wxHOURGLASS_CURSOR);
    try
    {
        if (preview_) {
            if (previewformat_) {
                delete previewformat_;
                previewformat_=NULL;
            }
            previewformat_=preview_->Save(DialogFormat(), DialogFlags());
            previewlabel_->SetLabel(wxString::Format(_("Preview (size %s [%s bytes]):"),
                wxFileName::GetHumanReadableSize(previewformat_->GetFileSize(), wxT("0")).c_str(),
                wxString(ccustring_numberformat(previewformat_->GetFileSize(), NF_THOUSANDS).c_str(), wxConvUTF8).c_str()));

            previewctrl_->SetImage(previewformat_, false);
        }
    } catch(...) {
        SetCursor(*wxSTANDARD_CURSOR);
        throw;
    }
    SetCursor(*wxSTANDARD_CURSOR);
}





void HTMLButcherImageFormatEditDialog::OnAutoUpdate(wxCommandEvent &event)
{
    if (event.IsChecked())
        UpdatePreview();
    event.Skip();
}




void HTMLButcherImageFormatEditDialog::OnFormatChange(wxCommandEvent &event)
{
    wxCheckBox *cb=(wxCheckBox*)FindWindow(ID_AUTOUPDATE);
    if (cb->GetValue())
        UpdatePreview();

    event.Skip();
}



void HTMLButcherImageFormatEditDialog::OnCtrlChanged(wxCommandEvent &event)
{
    UpdateAppState();
}




void HTMLButcherImageFormatEditDialog::UpdateAppState()
{
    ButcherImage::format_t format=DialogFormat();

    FindWindow(ID_JPEGCOMPRESSION)->Enable(format==ButcherImage::FMT_JPG);
    FindWindow(ID_JPEGPROGRESSIVE)->Enable(format==ButcherImage::FMT_JPG);
    FindWindow(ID_QUANTIZATON)->Enable(format!=ButcherImage::FMT_JPG);
}


