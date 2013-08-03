//=============================================================================
/**
 *  @file    DialogMaskAreaSlice.cpp
 *
 *  $Id: DialogMaskAreaSlice.cpp,v 1.2 2008/02/09 12:27:41 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-08
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

#include "DialogMaskAreaSlice.h"
#include "BList.h"






BEGIN_EVENT_TABLE(HTMLButcherMaskAreaSliceDialog, ButcherControl_Dialog)
    EVT_BUTCHERIMAGEVIEW_AFTERPAINT(ID_PREVIEW, HTMLButcherMaskAreaSliceDialog::OnAfterDraw)
    EVT_SPINCTRL(ID_ROWS, HTMLButcherMaskAreaSliceDialog::OnChangeSize)
    EVT_SPINCTRL(ID_COLS, HTMLButcherMaskAreaSliceDialog::OnChangeSize)
END_EVENT_TABLE()

HTMLButcherMaskAreaSliceDialog::HTMLButcherMaskAreaSliceDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style),
    previewctrl_(NULL), view_(NULL), area_(NULL), config_(NULL)
{
    Init();
    CreateControls();
}



void HTMLButcherMaskAreaSliceDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-gridslice.html"));
#endif
}




void HTMLButcherMaskAreaSliceDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxVERTICAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);

    // PREVIEW
    wxBoxSizer *previewsizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(previewsizer, 2, wxEXPAND, 3);

    // preview image
    previewctrl_ = new ButcherImageView(this, ID_PREVIEW, wxDefaultPosition, wxSize(400, 400));
    previewsizer->Add(previewctrl_, 1, wxEXPAND|wxALL, 3);


    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxHORIZONTAL);
    bodysizer->Add(fieldssizer, 0, wxALIGN_CENTER_HORIZONTAL, 3);

    // ROWS
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, _("&Rows:")),
        0, wxGROW|wxALL, 3);
    wxSpinCtrl *rowsctrl = new wxSpinCtrl(this, ID_ROWS, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxSP_ARROW_KEYS, 1, 30, 2);
    fieldssizer->Add(rowsctrl, 0, wxALL, 3);

    // COLUMNS
    fieldssizer->Add(new wxStaticText(this, wxID_STATIC, _("&Columns:")),
        0, wxGROW|wxALL, 3);
    wxSpinCtrl *colsctrl = new wxSpinCtrl(this, ID_COLS, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxSP_ARROW_KEYS, 1, 30, 2);
    fieldssizer->Add(colsctrl, 0, wxALL, 3);


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

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}




void HTMLButcherMaskAreaSliceDialog::Load(ButcherProjectView *view, ButcherProjectArea *loadarea,
    ButcherProjectAreaConfig *loadconfig)
{
    if (!loadconfig->CanSlice())
        throw ButcherException(_("This area cannot be sliced"));

    view_=view;
    area_=loadarea;
    config_=loadconfig;
}




void HTMLButcherMaskAreaSliceDialog::Save(ButcherProjectView *view,
    ButcherProjectArea *loadarea, ButcherProjectAreaConfig *loadconfig)
{
    ButcherProjectBaseAutoUpdate upd(loadarea);
}




bool HTMLButcherMaskAreaSliceDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    previewctrl_->SetImage(view_->CreateAreaImageDefault(area_));
    return true;
}




bool HTMLButcherMaskAreaSliceDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    if (config_->WillSliceClearAreas())
        if (wxMessageBox(_("This area already contains slices, they will be removed for this slicing operations, area you sure?"),
            _("Warning"), wxYES_NO|wxICON_WARNING)!=wxYES)
            return false;

    wxSpinCtrl *rowsctrl=(wxSpinCtrl*)FindWindow(ID_ROWS);
    wxSpinCtrl *colsctrl=(wxSpinCtrl*)FindWindow(ID_COLS);

    try
    {
        config_->Slice(rowsctrl->GetValue(), colsctrl->GetValue());
    } catch (ButcherException &e) {
        wxMessageBox(e.what(), _("Error"), wxOK|wxICON_ERROR);
        return false;
    }

    return true;
}




void HTMLButcherMaskAreaSliceDialog::OnAfterDraw(ButcherImageViewDrawEvent &event)
{
    wxSpinCtrl *rowsctrl=(wxSpinCtrl*)FindWindow(ID_ROWS);
    wxSpinCtrl *colsctrl=(wxSpinCtrl*)FindWindow(ID_COLS);

    event.GetDC()->SetPen(*wxGREEN_PEN);

    wxRect dr(previewctrl_->GetView()->GetDrawRect());

    float sz=(float)dr.GetHeight()/(float)rowsctrl->GetValue();
    for (int rows=1; rows<rowsctrl->GetValue(); rows++)
    {
        event.GetDC()->DrawLine(dr.GetLeft(), dr.GetTop()+(int)(sz*(float)rows),
            dr.GetRight(), dr.GetTop()+(int)(sz*(float)rows));
    }

    sz=(float)dr.GetWidth()/(float)colsctrl->GetValue();
    for (int cols=1; cols<colsctrl->GetValue(); cols++)
    {
        event.GetDC()->DrawLine(dr.GetLeft()+(int)(sz*(float)cols), dr.GetTop(),
            dr.GetLeft()+(int)(sz*(float)cols), dr.GetBottom());
    }

}



void HTMLButcherMaskAreaSliceDialog::OnChangeSize(wxSpinEvent &event)
{
    previewctrl_->Refresh();
}


