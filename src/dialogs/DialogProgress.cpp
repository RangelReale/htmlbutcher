//=============================================================================
/**
 *  @file    DialogProgress.cpp
 *
 *  $Id: DialogProgress.cpp,v 1.4 2008/02/06 12:09:51 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-22
 */
//=============================================================================

#include "DialogProgress.h"



/////////////////////////////////
// CLASS
//      HTMLButcherProgressDialog
/////////////////////////////////
BEGIN_EVENT_TABLE(HTMLButcherProgressDialog, wxFrame)
END_EVENT_TABLE()

IMPLEMENT_CLASS(HTMLButcherProgressDialog, wxFrame)



HTMLButcherProgressDialog::HTMLButcherProgressDialog() :
    wxFrame(), windisable_(this)
{
    SetExtraStyle(GetExtraStyle() | wxWS_EX_TRANSIENT);
    Init();
    CreateControls();
}




HTMLButcherProgressDialog::HTMLButcherProgressDialog( wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    wxFrame(parent, id, caption, pos, size, style), windisable_(this)
{
    SetExtraStyle(GetExtraStyle() | wxWS_EX_TRANSIENT);
    Init();
    CreateControls();
}




void HTMLButcherProgressDialog::Init()
{
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
}




bool HTMLButcherProgressDialog::Create( wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style )
{
    return wxFrame::Create(parent, id, caption, pos, size, style);
}




void HTMLButcherProgressDialog::CreateControls()
{
    // SIZER: TOP
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    // description
    ldesc_ = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(320, -1), wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
    topsizer->Add(ldesc_, 0, wxEXPAND|wxALL, 5);

    // step
/*
    lstep_ = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250, -1), wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
    topsizer->Add(lstep_, 0, wxEXPAND|wxALL, 5);
*/
    gauge_ = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition,
#ifdef __WXMSW__
        wxSize(-1, 20));
#else
        wxDefaultSize);
#endif
    topsizer->Add(gauge_, 1, wxEXPAND|wxALL, 5);


    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



void HTMLButcherProgressDialog::UpdateProgress(ButcherProject *project)
{
    if (project->ProgressCount()>0)
    {
        if ((project->ProgressGet(project->ProgressCount()-1).flags&ButcherProject::UF_GROUP)!=ButcherProject::UF_GROUP)
            ldesc_->SetLabel(project->ProgressGet(project->ProgressCount()-1).description);

        if (project->ProgressGet(project->ProgressCount()-1).progress>=0)
        {
            if (gauge_->GetRange()==0)
                gauge_->SetRange(100);
            gauge_->SetValue(project->ProgressGet(project->ProgressCount()-1).progress);
        }
        else
        {
            if (gauge_->GetRange()!=0)
                gauge_->SetRange(0);
            gauge_->Pulse();
        }

        //lstep_->SetLabel(wxString::Format(_("%d steps remaining..."), project->ProgressCount()));
        Update();

        wxYieldIfNeeded();
    }
}




