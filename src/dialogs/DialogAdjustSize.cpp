//=============================================================================
/**
 *  @file    DialogAdjustSize.cpp
 *
 *  $Id: DialogAdjustSize.cpp,v 1.13 2008/02/25 21:52:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-09
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

#include "DialogAdjustSize.h"



BEGIN_EVENT_TABLE(HTMLButcherAdjustSizeDialog, ButcherControl_Dialog)
    EVT_SPINCTRL(ID_LEFT, HTMLButcherAdjustSizeDialog::OnChangeLeft)
    EVT_SPINCTRL(ID_TOP, HTMLButcherAdjustSizeDialog::OnChangeTop)
    EVT_SPINCTRL(ID_RIGHT, HTMLButcherAdjustSizeDialog::OnChangeRight)
    EVT_SPINCTRL(ID_BOTTOM, HTMLButcherAdjustSizeDialog::OnChangeBottom)
    EVT_SPINCTRL(ID_MOVELEFT, HTMLButcherAdjustSizeDialog::OnChangeMoveLeft)
    EVT_SPINCTRL(ID_MOVETOP, HTMLButcherAdjustSizeDialog::OnChangeMoveTop)
    EVT_SPINCTRL(ID_WIDTH, HTMLButcherAdjustSizeDialog::OnChangeWidth)
    EVT_SPINCTRL(ID_HEIGHT, HTMLButcherAdjustSizeDialog::OnChangeHeight)
    EVT_BUTTON(ID_REVERT, HTMLButcherAdjustSizeDialog::OnRevert)
END_EVENT_TABLE()



HTMLButcherAdjustSizeDialog::HTMLButcherAdjustSizeDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style), file_(NULL),
    area_(NULL), line_(NULL), defaultvalues_(-1, -1, -1, -1), updating_(false)
{
    Init();
    CreateControls();
}




void HTMLButcherAdjustSizeDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-positionandsize.html"));
#endif
}




void HTMLButcherAdjustSizeDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);

    // PREVIEW
    wxBoxSizer *previewsizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(previewsizer, 1, wxEXPAND|wxALL, 3);

    // preview image
    //wxStaticText *previewlabel = new wxStaticText(this, wxID_STATIC, _("Area:"), wxDefaultPosition, wxDefaultSize, 0);
    //previewsizer->Add(previewlabel, 0, wxALIGN_LEFT|wxALL, 3);

    previewctrl_ = new ButcherImageView(this, wxID_ANY, wxDefaultPosition, wxSize(300, 300));
    previewctrl_->SetFullSize(true);
    previewctrl_->SetMargin(10);
    previewctrl_->SetBGColor(wxTheColourDatabase->Find(wxT("MEDIUM VIOLET RED")));

    previewsizer->Add(previewctrl_, 1, wxEXPAND|wxALL, 3);



    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(fieldssizer, 0, wxEXPAND|wxALL, 3);


    // POSITIONS
    wxStaticBoxSizer *positionbox = new wxStaticBoxSizer(wxVERTICAL, this, _("Positions"));
    fieldssizer->Add(positionbox, 0, wxGROW|wxALL, 3);

    wxFlexGridSizer *positionsizer = new wxFlexGridSizer(6);
    positionbox->Add(positionsizer, 0, wxGROW|wxALL, 3);

    //positionsizer->AddGrowableCol(1, 1);
    //positionsizer->AddGrowableCol(4, 1);


    // top
    positionsizer->AddSpacer(0);
    //positionsizer->AddSpacer(0);

    positionsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Top:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_RIGHT|wxALL, 3);

    wxSpinCtrl *mtopctrl = new wxSpinCtrl(this, ID_TOP, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 0, 10000, 0);
    positionsizer->Add(mtopctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticText *ltopctrl = new wxStaticText(this, ID_TOPGLOBAL, wxT("(00000)"));
    positionsizer->Add(ltopctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    positionsizer->AddSpacer(0);
    positionsizer->AddSpacer(0);

    // left
    positionsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Left:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mleftctrl = new wxSpinCtrl(this, ID_LEFT, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 0, 10000, 0);
    positionsizer->Add(mleftctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticText *lleftctrl = new wxStaticText(this, ID_LEFTGLOBAL, wxT("(00000)"));
    positionsizer->Add(lleftctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    //positionsizer->AddSpacer(0);

    // right
    positionsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Right:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mrightctrl = new wxSpinCtrl(this, ID_RIGHT, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 0, 10000, 0);
    positionsizer->Add(mrightctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticText *lrightctrl = new wxStaticText(this, ID_RIGHTGLOBAL, wxT("(00000)"));
    positionsizer->Add(lrightctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // bottom
    //positionsizer->AddSpacer(0);
    positionsizer->AddSpacer(0);

    positionsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Bottom:"), wxDefaultPosition, wxDefaultSize, 0),
        0, wxALIGN_RIGHT|wxALL, 3);

    wxSpinCtrl *mbottomctrl = new wxSpinCtrl(this, ID_BOTTOM, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 0, 10000, 0);
    positionsizer->Add(mbottomctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticText *lbottomctrl = new wxStaticText(this, ID_BOTTOMGLOBAL, wxT("(00000)"));
    positionsizer->Add(lbottomctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    positionsizer->AddSpacer(0);
    positionsizer->AddSpacer(0);

    // MOVEMENT
    wxStaticBoxSizer *movementbox = new wxStaticBoxSizer(wxVERTICAL, this, _("Movement"));
    fieldssizer->Add(movementbox, 0, wxGROW|wxALL, 3);

	wxBoxSizer *movementsizer = new wxBoxSizer(wxHORIZONTAL);
    movementbox->Add(movementsizer, 0, wxGROW|wxALL, 3);

    // move left
    movementsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("Le&ft:"), wxDefaultPosition, wxSize(-1, -1), 0),
        0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mmoveleftctrl = new wxSpinCtrl(this, ID_MOVELEFT, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 0, 10000, 0);
    movementsizer->Add(mmoveleftctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    movementsizer->AddSpacer(20);

    // move top
    movementsizer->Add(
        new wxStaticText(this, wxID_STATIC, _("To&p:"), wxDefaultPosition, wxSize(-1, -1), 0),
        0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mmovetopctrl = new wxSpinCtrl(this, ID_MOVETOP, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 0, 10000, 0);
    movementsizer->Add(mmovetopctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    //movementsizer->AddSpacer(0);


    // SIZE
    wxStaticBoxSizer *sizebox = new wxStaticBoxSizer(wxVERTICAL, this, _("Size"));
    fieldssizer->Add(sizebox, 0, wxGROW|wxALL, 3);

	wxBoxSizer *sizesizer = new wxBoxSizer(wxHORIZONTAL);
    sizebox->Add(sizesizer, 0, wxGROW|wxALL, 3);

    // width
    sizesizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Width:"), wxDefaultPosition, wxSize(-1, -1), 0),
        0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mwidthctrl = new wxSpinCtrl(this, ID_WIDTH, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 0, 10000, 0);
    sizesizer->Add(mwidthctrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    sizesizer->AddSpacer(20);

    // height
    sizesizer->Add(
        new wxStaticText(this, wxID_STATIC, _("&Height:"), wxDefaultPosition, wxSize(-1, -1), 0),
        0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxSpinCtrl *mheightctrl = new wxSpinCtrl(this, ID_HEIGHT, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
        wxSP_ARROW_KEYS, 0, 10000, 0);
	sizesizer->Add(mheightctrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);



    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    // BUTTONS
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // revert button
    wxButton* revert = new wxButton ( this, ID_REVERT, _("Re&vert"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(revert, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // cancel button
    wxButton* cancel = new wxButton ( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // validators
    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



bool HTMLButcherAdjustSizeDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    defaultvalues_=area_->GetRect();

    UpdateDisplay(true);
    return true;
}




bool HTMLButcherAdjustSizeDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;
    return true;
}




void HTMLButcherAdjustSizeDialog::UpdateDisplay(bool isfirst)
{
    if (updating_) return;

    wxSpinCtrl *mleftctrl=(wxSpinCtrl*)FindWindow(ID_LEFT);
    wxSpinCtrl *mtopctrl=(wxSpinCtrl*)FindWindow(ID_TOP);
    wxSpinCtrl *mrightctrl=(wxSpinCtrl*)FindWindow(ID_RIGHT);
    wxSpinCtrl *mbottomctrl=(wxSpinCtrl*)FindWindow(ID_BOTTOM);
    wxSpinCtrl *mwidthctrl=(wxSpinCtrl*)FindWindow(ID_WIDTH);
    wxSpinCtrl *mheightctrl=(wxSpinCtrl*)FindWindow(ID_HEIGHT);
    wxStaticText *lleftctrl=(wxStaticText*)FindWindow(ID_LEFTGLOBAL);
    wxStaticText *ltopctrl=(wxStaticText*)FindWindow(ID_TOPGLOBAL);
    wxStaticText *lrightctrl=(wxStaticText*)FindWindow(ID_RIGHTGLOBAL);
    wxStaticText *lbottomctrl=(wxStaticText*)FindWindow(ID_BOTTOMGLOBAL);
    wxSpinCtrl *mmoveleftctrl=(wxSpinCtrl*)FindWindow(ID_MOVELEFT);
    wxSpinCtrl *mmovetopctrl=(wxSpinCtrl*)FindWindow(ID_MOVETOP);

    ButcherImage *preview=file_->GetImage()->SubImage(
        area_->GetGlobalRect()
    );
    previewctrl_->SetImage(preview);

    updating_=true;

    // free range, reset it later
    mleftctrl->SetRange(0, 10000);
    mtopctrl->SetRange(0, 10000);
    mrightctrl->SetRange(0, 10000);
    mbottomctrl->SetRange(0, 10000);
    mmoveleftctrl->SetRange(0, 10000);
    mmovetopctrl->SetRange(0, 10000);

    if (mleftctrl->GetValue() != area_->GetLeft()->GetPosition())
        mleftctrl->SetValue(area_->GetLeft()->GetPosition());
    if (mtopctrl->GetValue() != area_->GetTop()->GetPosition())
        mtopctrl->SetValue(area_->GetTop()->GetPosition());
    if (mrightctrl->GetValue() != area_->GetRight()->GetPosition()-1)
        mrightctrl->SetValue(area_->GetRight()->GetPosition()-1);
    if (mbottomctrl->GetValue() != area_->GetBottom()->GetPosition()-1)
        mbottomctrl->SetValue(area_->GetBottom()->GetPosition()-1);

    if (mmoveleftctrl->GetValue() != area_->GetLeft()->GetPosition())
        mmoveleftctrl->SetValue(area_->GetLeft()->GetPosition());
    if (mmovetopctrl->GetValue() != area_->GetTop()->GetPosition())
        mmovetopctrl->SetValue(area_->GetTop()->GetPosition());

    if (mwidthctrl->GetValue() != area_->GetWidth())
        mwidthctrl->SetValue(area_->GetWidth());
    if (mheightctrl->GetValue() != area_->GetHeight())
        mheightctrl->SetValue(area_->GetHeight());

    lleftctrl->SetLabel(wxString::Format(wxT("(%d)"), area_->GetLeft()->GetGlobalPosition()));
    ltopctrl->SetLabel(wxString::Format(wxT("(%d)"), area_->GetTop()->GetGlobalPosition()));
    lrightctrl->SetLabel(wxString::Format(wxT("(%d)"), area_->GetRight()->GetGlobalPosition()-1));
    lbottomctrl->SetLabel(wxString::Format(wxT("(%d)"), area_->GetBottom()->GetGlobalPosition()-1));

    SetSpinRange(area_->GetLeft(), mleftctrl, false);
    SetSpinRange(area_->GetTop(), mtopctrl, false);
    SetSpinRange(area_->GetRight(), mrightctrl, true);
    SetSpinRange(area_->GetBottom(), mbottomctrl, true);
    SetSpinRange(area_->GetLeft(), area_->GetRight(), mmoveleftctrl);
    SetSpinRange(area_->GetTop(), area_->GetBottom(), mmovetopctrl);
    mwidthctrl->Enable(area_->GetLeft()->IsMoveable() || area_->GetRight()->IsMoveable());
    mheightctrl->Enable(area_->GetTop()->IsMoveable() || area_->GetTop()->IsMoveable());

    updating_=false;
}



void HTMLButcherAdjustSizeDialog::OnChangeLeft(wxSpinEvent &event)
{
    if (updating_) return;

    if (area_->GetLeft()->CanMove(event.GetPosition()))
    {
        area_->GetLeft()->Move(event.GetPosition());
    }
    UpdateDisplay();
    event.Skip();
}




void HTMLButcherAdjustSizeDialog::OnChangeTop(wxSpinEvent &event)
{
    if (updating_) return;

    if (area_->GetTop()->CanMove(event.GetPosition()))
    {
        area_->GetTop()->Move(event.GetPosition());
    }
    UpdateDisplay();
    event.Skip();
}




void HTMLButcherAdjustSizeDialog::OnChangeRight(wxSpinEvent &event)
{
    if (updating_) return;

    if (area_->GetRight()->CanMove(event.GetPosition()+1))
    {
        area_->GetRight()->Move(event.GetPosition()+1);
    }
    UpdateDisplay();
    event.Skip();
}




void HTMLButcherAdjustSizeDialog::OnChangeBottom(wxSpinEvent &event)
{
    if (updating_) return;

    if (area_->GetBottom()->CanMove(event.GetPosition()+1))
    {
        area_->GetBottom()->Move(event.GetPosition()+1);
    }
    UpdateDisplay();
    event.Skip();
}




void HTMLButcherAdjustSizeDialog::OnChangeWidth(wxSpinEvent &event)
{
    if (updating_) return;

    area_->Resize(event.GetPosition());

    UpdateDisplay();
    event.Skip();
}




void HTMLButcherAdjustSizeDialog::OnChangeHeight(wxSpinEvent &event)
{
    if (updating_) return;

    area_->Resize(-1, event.GetPosition());

    UpdateDisplay();
    event.Skip();
}




void HTMLButcherAdjustSizeDialog::OnChangeMoveLeft(wxSpinEvent &event)
{
    if (updating_) return;

    area_->Move(event.GetPosition());

    UpdateDisplay();
    event.Skip();
}




void HTMLButcherAdjustSizeDialog::OnChangeMoveTop(wxSpinEvent &event)
{
    if (updating_) return;

    area_->Move(-1, event.GetPosition());

    UpdateDisplay();
    event.Skip();
}




void HTMLButcherAdjustSizeDialog::SetSpinRange(ButcherProjectLine *line, wxSpinCtrl *spinctrl, bool isend)
{
    ButcherProjectLineMinMax mm;

    if (line->IsMoveable())
    {
        mm=line->GetMoveMinMax();
        if (isend)
        {
            mm.min--;
            mm.max--;
        }
        spinctrl->SetRange(mm.min, mm.max);
    }
    else
        spinctrl->Enable(false);
}



void HTMLButcherAdjustSizeDialog::SetSpinRange(ButcherProjectLine *start,
    ButcherProjectLine *end, wxSpinCtrl *spinctrl)
{
    ButcherProjectLineMinMax mm;

    if (start->IsMoveable() && end->IsMoveable())
    {
        mm=MovementMinMax(start, end);

        if (spinctrl->GetValue()<mm.min || spinctrl->GetValue()>mm.max)
            wxMessageBox(wxT("Uh oh"));

        spinctrl->SetRange(mm.min, mm.max);
    }
    else
        spinctrl->Enable(false);
}




void HTMLButcherAdjustSizeDialog::RevertToDefault()
{
    if (defaultvalues_.GetLeft()>=0)
    {
        if (area_->GetLeft()->GetPosition()!=defaultvalues_.GetLeft() &&
            area_->GetLeft()->CanMove(defaultvalues_.GetLeft()))
            area_->GetLeft()->Move(defaultvalues_.GetLeft());
        if (area_->GetTop()->GetPosition()!=defaultvalues_.GetTop() &&
            area_->GetTop()->CanMove(defaultvalues_.GetTop()))
            area_->GetTop()->Move(defaultvalues_.GetTop());
        if (area_->GetRight()->GetPosition()!=defaultvalues_.GetRight()+1 &&
            area_->GetRight()->CanMove(defaultvalues_.GetRight()+1))
            area_->GetRight()->Move(defaultvalues_.GetRight()+1);
        if (area_->GetBottom()->GetPosition()!=defaultvalues_.GetBottom()+1 &&
            area_->GetBottom()->CanMove(defaultvalues_.GetBottom()+1))
            area_->GetBottom()->Move(defaultvalues_.GetBottom()+1);
    }
}




void HTMLButcherAdjustSizeDialog::OnRevert(wxCommandEvent &event)
{
    RevertToDefault();
    UpdateDisplay();
}




ButcherProjectLineMinMax HTMLButcherAdjustSizeDialog::MovementMinMax(ButcherProjectLine *start, ButcherProjectLine *end)
{
    ButcherProjectLineMinMax
        mms=start->GetMoveMinMax(),
        mme=end->GetMoveMinMax();
    mms.max=mme.max;
    return mms;
}



