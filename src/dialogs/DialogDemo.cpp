//=============================================================================
/**
 *  @file    DialogDemo.cpp
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-09-30
 */
//=============================================================================
#ifdef HTMLBUTCHER_DEMO

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/statline.h>
#include <wx/statbox.h>

#include "DialogDemo.h"

BEGIN_EVENT_TABLE(HTMLButcherDEMODialog, wxDialog)
	EVT_BUTTON(wxID_OK, HTMLButcherDEMODialog::OnOk)
	EVT_TIMER(ID_TIMER, HTMLButcherDEMODialog::OnTimer)
	EVT_HTML_LINK_CLICKED(ID_MSG, HTMLButcherDEMODialog::OnLink)
END_EVENT_TABLE()

HTMLButcherDEMODialog::HTMLButcherDEMODialog( wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    wxDialog(parent, id, caption, pos, size, style),
		timer_(this, ID_TIMER), tmrct_(0)
{
    Init();
    CreateControls();
}

void HTMLButcherDEMODialog::Init()
{
}

void HTMLButcherDEMODialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxGROW, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);

	wxHtmlWindow *msgctrl = new wxHtmlWindow(this, ID_MSG, wxDefaultPosition, wxSize(350, 230));
	msgctrl->SetBackgroundColour(GetBackgroundColour());
	msgctrl->SetPage(_("<html><body> \
		This is a DEMO version of HTMLButcher. This demo have the following limitations:<br>\
		<br>\
		<ul>\
		<li>No project loading and saving.</li>\
		<li>Limit of 2 files, 1 mask, 1 view, and 1 alternate file.</li>\
		<li>Line patterns are added to the generated images.</li>\
		</ul>\
		<br><br>\
		Other than that, the application is exactly the same. Try it out, and buy it from <a href=\"http://www.htmlbutcher.com\">http://www.htmlbutcher.com</a> if you like it.<br>\
		</body></html>"));
    bodysizer->Add(msgctrl, 1, wxEXPAND|wxALL, 3);

    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    // BUTTONS
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("Wait"), wxDefaultPosition, wxDefaultSize, 0 );
	ok->Enable(false);
    buttonsizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();

	// timer
	timer_.Start(1000, false);
}

void HTMLButcherDEMODialog::OnOk(wxCommandEvent &event)
{
	Close();
}

void HTMLButcherDEMODialog::OnLink(wxHtmlLinkEvent &event)
{
	::wxLaunchDefaultBrowser(event.GetLinkInfo().GetHref());
}

void HTMLButcherDEMODialog::OnTimer(wxTimerEvent &event)
{
	tmrct_++;
	if (tmrct_>5)
	{
		timer_.Stop();
		((wxButton*)(FindWindow(wxID_OK)))->SetLabel(_("&Ok"));
		((wxButton*)(FindWindow(wxID_OK)))->Enable(true);
	} else {
		((wxButton*)(FindWindow(wxID_OK)))->SetLabel(wxString::Format(_("Wait %ds"), 5-tmrct_+1));
		((wxButton*)(FindWindow(wxID_OK)))->Enable(false);
	}

	event.Skip();

}

#endif // HTMLBUTCHER_DEMO
