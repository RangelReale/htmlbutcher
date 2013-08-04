#include <wx/xrc/xmlres.h>

#include <deque>
#include "DialogAbout.h"
#include "BControls.h"
#include "HTMLButcherVersion.h"





BEGIN_EVENT_TABLE(HTMLButcherAboutDialog, wxDialog)
    EVT_CHAR(HTMLButcherAboutDialog::OnChar)
    EVT_KEY_DOWN(HTMLButcherAboutDialog::OnChar)
    EVT_MOUSE_EVENTS(HTMLButcherAboutDialog::OnMouseEvent)
    EVT_WINDOW_CREATE(HTMLButcherAboutDialog::OnWindowCreate)
END_EVENT_TABLE()

HTMLButcherAboutDialog::HTMLButcherAboutDialog( wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style ) :
    wxDialog(parent, id, caption, pos, size, style), bitmap_()
{
    Init();
    CreateControls();
}



void HTMLButcherAboutDialog::Init()
{
}



void HTMLButcherAboutDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALL|wxGROW, 3);

	// logo
    {
        wxLogNull lnull;
        wxBitmap rbitmap(wxXmlResource::Get()->LoadBitmap(wxT("b_splash")));
        wxImage simage(rbitmap.ConvertToImage());
        simage.ConvertAlphaToMask();
        bitmap_=wxBitmap(simage);
        //wxBitmap bitmap(simage);
    }

	image_ = new wxStaticBitmap(this, wxID_ANY,
#ifndef __WXGTK__
        wxBitmap(bitmap_));
#else
        wxXmlResource::Get()->LoadBitmap(wxT("b_splash")));
#endif
	image_->Connect(wxID_ANY, wxEVT_LEFT_DOWN, wxMouseEventHandler(HTMLButcherAboutDialog::OnMouseEvent), NULL, this);
	image_->Connect(wxID_ANY, wxEVT_CHAR, wxKeyEventHandler(HTMLButcherAboutDialog::OnChar), NULL, this);
	image_->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(HTMLButcherAboutDialog::OnChar), NULL, this);

	boxsizer->Add(image_, 0);

	ButcherControl_TransparentStaticText *b=new ButcherControl_TransparentStaticText(
#ifndef __WXGTK__
        image_,
#else
        this,
#endif
		wxID_ANY, wxString::Format(_("Version %s"), wxString(wxT(HTMLBUTCHERVERSION_STRING)).c_str()), wxPoint(211, 217), wxSize(140, -1));

#if defined( __WXMSW__ )
    wxRegion treg(bitmap_);
    SetShape(treg);
    //SetTransparent(93);
#endif

	SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}



void HTMLButcherAboutDialog::OnMouseEvent(wxMouseEvent& event)
{
    if (event.LeftDown() || event.RightDown())
        Close(true);
}



void HTMLButcherAboutDialog::OnChar(wxKeyEvent& WXUNUSED(event))
{
    Close(true);
}



void HTMLButcherAboutDialog::OnWindowCreate(wxWindowCreateEvent & event)
{
#if !defined( __WXMSW__ )
    wxRegion treg(bitmap_);//image_->GetBitmap());
    SetShape(treg);
    //SetTransparent(93);
#endif
}


