//=============================================================================
/**
 *  @file    BControls.cpp
 *
 *  $Id: BControls.cpp,v 1.10 2008/05/20 21:31:21 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-02
 */
//=============================================================================

#include <memory>
#include <wx/dcbuffer.h>
#include <wx/help.h>
#include <wx/cshelp.h>

#include "BControls.h"
#include "ButcherProjectMaskDrawSelection.h"

#include "DialogFileLink.h"
#include "wxpopupmenu.h"



BEGIN_EVENT_TABLE(ButcherControl_Dialog, wxDialog)
#ifdef BUTCHER_USE_HELP
	EVT_BUTTON(wxID_HELP, ButcherControl_Dialog::OnHelp)
	EVT_HELP(wxID_ANY, ButcherControl_Dialog::OnHelpEvent)
#endif
END_EVENT_TABLE()

/////////////////////////////////
// CLASS
//      ButcherControl_Dialog
/////////////////////////////////
ButcherControl_Dialog::ButcherControl_Dialog( wxWindow* parent,
    wxWindowID id, ButcherProject *project, const wxString& caption,
    const wxPoint& pos, const wxSize& size, long style ) :
    wxDialog(parent, id, caption, pos, size, style),
    project_(project)
{
    //Create(parent, id, caption, pos, size, style);
}



void ButcherControl_Dialog::ShowHelp()
{
#ifdef BUTCHER_USE_HELP
	if (!static_cast<wxHelpControllerHelpProvider*>(wxHelpProvider::Get())->GetHelpController()->DisplaySection(helpsection_))
	{
		wxMessageBox(_("Could not load help"));
	}
#endif
}

void ButcherControl_Dialog::OnHelp(wxCommandEvent& event)
{
	ShowHelp();
}

void ButcherControl_Dialog::OnHelpEvent(wxHelpEvent& event)
{
	ShowHelp();
}



/////////////////////////////////
// CLASS
//      ButcherControl_FileLink
/////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(ButcherControl_FileLink, wxControl)

BEGIN_EVENT_TABLE(ButcherControl_FileLink, wxControl)
    EVT_PAINT(ButcherControl_FileLink::OnPaint)
    EVT_LEFT_DOWN(ButcherControl_FileLink::OnLeftDown)
    EVT_LEFT_UP(ButcherControl_FileLink::OnLeftUp)
    EVT_MOTION(ButcherControl_FileLink::OnMotion)
    EVT_LEAVE_WINDOW(ButcherControl_FileLink::OnLeaveWindow)
    EVT_SIZE(ButcherControl_FileLink::OnSize)
END_EVENT_TABLE()



bool ButcherControl_FileLink::Create(wxWindow *parent,
    wxWindowID id, ButcherProject *project,
        const wxPoint& pos, const wxSize& size, long style,
        const wxString& name)
{
    if (!wxControl::Create(parent, id, pos, size, style, wxDefaultValidator, name))
        return false;

    m_rollover = false;
    m_clicking = false;

    SetLabel(_("Empty Link"));

    // colours
    m_normalColour = *wxBLUE;
    m_hoverColour = *wxRED;
    SetForegroundColour(m_normalColour);

    // by default the font of an hyperlink control is underlined
    wxFont f = GetFont();
    f.SetUnderlined(true);
    SetFont(f);

    SetInitialSize(size);

    return true;
}

void ButcherControl_FileLink::SetProject(ButcherProject *project)
{
    if (filelink_) delete filelink_;
    filelink_=new ButcherProjectFileLink(project);
}

void ButcherControl_FileLink::SetFileLink(const ButcherProjectFileLink *fl)
{
    if (filelink_)
    {
        filelink_->SetBURL(fl->GetBURL());
        //if (!filelink_->GetBURL().IsEmpty())
            SetLabel(filelink_->GetDescription());
            SetToolTip(filelink_->GetDescription());
/*
        else
            SetLabel(_("Empty Link"));
*/
        Refresh();
    }
}

wxSize ButcherControl_FileLink::DoGetBestSize() const
{
    int w, h;

    wxClientDC dc((wxWindow *)this);
    dc.SetFont(GetFont());
    dc.GetTextExtent(GetLabel(), &w, &h);

    wxSize best(w, h);
    CacheBestSize(best);
    return best;
}


void ButcherControl_FileLink::SetNormalColour(const wxColour &colour)
{
    m_normalColour = colour;
    SetForegroundColour(m_normalColour);
    Refresh();
}



wxRect ButcherControl_FileLink::GetLabelRect() const
{
    // our best size is always the size of the label without borders
    wxSize c(GetClientSize()), b(GetBestSize());
    wxPoint offset;

    // the label is always centered vertically
    offset.y = (c.GetHeight()-b.GetHeight())/2;
    // the label is always left aligned
    offset.x = 0;
    return wxRect(offset, b);
}

void ButcherControl_FileLink::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    dc.SetFont(GetFont());
    if (IsEnabled())
        dc.SetTextForeground(GetForegroundColour());
    else
        dc.SetTextForeground(wxSYS_COLOUR_INACTIVECAPTION);
    dc.SetTextBackground(GetBackgroundColour());

    dc.DrawText(GetLabel(), GetLabelRect().GetTopLeft());
}

void ButcherControl_FileLink::OnLeftDown(wxMouseEvent& event)
{
    // the left click must start from the hyperlink rect
    m_clicking = GetLabelRect().Contains(event.GetPosition());
}



void ButcherControl_FileLink::OnLeftUp(wxMouseEvent& event)
{
    // the click must be started and ended in the hyperlink rect
    if (!m_clicking || !GetLabelRect().Contains(event.GetPosition()))
        return;

    SetForegroundColour(m_normalColour);
    m_clicking = false;

    LinkClicked();
}

void ButcherControl_FileLink::LinkClicked()
{
    if (filelink_)
    {
        ButcherProjectFileLink tmplink(*filelink_);

        HTMLButcherFileLinkDialog d(this, wxID_ANY, filelink_->GetProject());
        d.SetSelTypes(HTMLButcherFileLinkDialog::ST_IMAGEONLY|HTMLButcherFileLinkDialog::ST_ALLOWNONE);
        d.SetLink(&tmplink);
        if (d.ShowModal()==wxID_OK)
            SetFileLink(&d.GetFileLink());
    };
}

void ButcherControl_FileLink::OnMotion(wxMouseEvent& event)
{
    wxRect textrc = GetLabelRect();

    if (textrc.Contains(event.GetPosition()))
    {
        SetCursor(wxCursor(wxCURSOR_HAND));
        SetForegroundColour(m_hoverColour);
        m_rollover = true;
        Refresh();
    }
    else if (m_rollover)
    {
        SetCursor(*wxSTANDARD_CURSOR);
        SetForegroundColour(m_normalColour);
        m_rollover = false;
        Refresh();
    }
}



void ButcherControl_FileLink::OnLeaveWindow(wxMouseEvent& WXUNUSED(event) )
{
    // NB: when the label rect and the client size rect have the same
    //     height this function is indispensable to remove the "rollover"
    //     effect as the OnMotion() event handler could not be called
    //     in that case moving the mouse out of the label vertically...

    if (m_rollover)
    {
        SetCursor(*wxSTANDARD_CURSOR);
        SetForegroundColour(m_normalColour);
        m_rollover = false;
        Refresh();
    }
}

void ButcherControl_FileLink::OnSize(wxSizeEvent& WXUNUSED(event))
{
    // update the position of the label in the screen respecting
    // the selected align flag
    Refresh();
}

/////////////////////////////////
// CLASS
//      ButcherControl_FmtTextCtrl
/////////////////////////////////

#ifdef BUTCHER_USE_STEDIT
BEGIN_EVENT_TABLE(ButcherControl_FmtTextCtrl, wxSTEditor)
#else
BEGIN_EVENT_TABLE(ButcherControl_FmtTextCtrl, wxTextCtrl)
#endif
    EVT_RIGHT_DOWN(ButcherControl_FmtTextCtrl::OnRMouseDown)
END_EVENT_TABLE()



void ButcherControl_FmtTextCtrl::Init()
{
#ifdef BUTCHER_USE_STEDIT
    RegisterPrefs(wxSTEditorPrefs::GetGlobalEditorPrefs());
    RegisterStyles(wxSTEditorStyles::GetGlobalEditorStyles());
    RegisterLangs(wxSTEditorLangs::GetGlobalEditorLangs());
#ifdef __WXGTK__
    GetEditorStyles().SetSize(STE_STYLE_DEFAULT, 10);
#endif
#endif
}

ButcherControl_FmtTextCtrl::ButcherControl_FmtTextCtrl(wxWindow* parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator,
    const wxString& name)
#ifdef BUTCHER_USE_STEDIT
        : wxSTEditor(parent, id, pos, size, style, name)
#else
        : wxTextCtrl(parent, id, wxEmptyString, pos, size, style, validator, name)
#endif
    , project_(NULL), allowfilelink_(false), filelinkseltypes_(0)
{
    Init();
    Create(parent, id, pos, size, style, validator, name);
}

bool ButcherControl_FmtTextCtrl::Create(wxWindow* parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator,
    const wxString& name)
{
    return true;
}



void ButcherControl_FmtTextCtrl::SetEditFormat(editformat_t fmt)
{
#ifdef BUTCHER_USE_STEDIT
    switch (fmt)
    {
    case FMT_CSS:
        SetLanguage(STE_LANG_CSS);
        break;
    case FMT_HTML:
        SetLanguage(STE_LANG_HTML);
        break;
    default:
        break;
    }
#endif
}

void ButcherControl_FmtTextCtrl::OnRMouseDown(wxMouseEvent &event)
{
    if (allowfilelink_)
    {
        wxPopupMenu pm;

        pm.Append(1, _("&Insert file link"));
        PopupMenu(&pm);
        if (pm.GetSelectedId()==1)
        {
            DoInsertFileLink();
        }
    }
    event.Skip();
}



void ButcherControl_FmtTextCtrl::DoInsertFileLink()
{
    if (!project_) return;

    HTMLButcherFileLinkDialog d(this, wxID_ANY, project_);
    if (d.ShowModal()==wxID_OK)
        WriteText(wxString::Format(wxT("$$burl %s$$"), d.GetFileLink().GetBURL().c_str()));
}

/////////////////////////////////
// CLASS
//      ButcherControl_FileLink
/////////////////////////////////
bool ButcherControl_GenericValidator::TransferToWindow()
{
    if ( !m_validatorWindow )
        return false;

#ifdef BUTCHER_USE_STEDIT
    // wxSTEdit
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSTEditor)) )
    {
        wxSTEditor* pControl = (wxSTEditor*) m_validatorWindow;
        if (m_pString)
        {
            pControl->SetText(*m_pString) ;
            return true;
        }
        else if (m_pInt)
        {
            wxString str;
            str.Printf(wxT("%d"), *m_pInt);
            pControl->SetText(str);
            return true;
        }
    }
#endif
    return wxGenericValidator::TransferToWindow();
}



bool ButcherControl_GenericValidator::TransferFromWindow()
{
    if ( !m_validatorWindow )
        return false;

#ifdef BUTCHER_USE_STEDIT
    if (m_validatorWindow->IsKindOf(CLASSINFO(wxSTEditor)) )
    {
        wxSTEditor* pControl = (wxSTEditor*) m_validatorWindow;
        if (m_pString)
        {
            *m_pString = pControl->GetValue() ;
            return true;
        }
        else if (m_pInt)
        {
            *m_pInt = wxAtoi(pControl->GetValue());
            return true;
        }
    }
#endif
    return wxGenericValidator::TransferFromWindow();
}

/////////////////////////////////
// CLASS
//      ButcherControl_ButcherViewSimulated
/////////////////////////////////

float ButcherControl_ButcherViewSimulated::GetPixelSize()
{
	//if (width_==0 || height_==0) return 1;
	return min( (float)width_/(float)maskroot_->GetWidth(), (float)height_/(float)maskroot_->GetHeight() );
}



/////////////////////////////////
// CLASS
//      ButcherControl_SelectionDisplay
/////////////////////////////////
BEGIN_EVENT_TABLE(ButcherControl_SelectionDisplay, wxControl)
    EVT_PAINT(ButcherControl_SelectionDisplay::OnPaint)
    EVT_ERASE_BACKGROUND(ButcherControl_SelectionDisplay::OnErase)
END_EVENT_TABLE()

ButcherControl_SelectionDisplay::ButcherControl_SelectionDisplay(wxWindow* parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxControl(parent, id, pos, size, style, wxDefaultValidator, name), selectevent_(NULL)
{
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
}

ButcherControl_SelectionDisplay::~ButcherControl_SelectionDisplay()
{
	if (selectevent_) delete selectevent_;
}

void ButcherControl_SelectionDisplay::SetSelectEvent(const ButcherViewSelectEvent &event)
{
	if (!selectevent_ || !selectevent_->IsSameArea(event))
	{
		if (selectevent_) delete selectevent_;
		selectevent_=new ButcherViewSelectEvent(event);
		Refresh();
		Update();
	}
}



void ButcherControl_SelectionDisplay::ClearSelectEvent()
{
	if (selectevent_) delete selectevent_;
	selectevent_=NULL;
	Refresh();
	Update();
}

/*
class BSDV : public ButcherViewBase
{
public:
	BSDV() : ButcherViewBase(), left_(0), top_(0) {}

    virtual unsigned int GetDesignLeft() { return left_; }
    virtual unsigned int GetDesignTop() { return top_; }
	virtual unsigned int GetDesignWidth() { return 50; }
	virtual unsigned int GetDesignHeight() { return 50; }
	virtual float GetPixelSize() { return 0.2f; }

	void SetDesignLeft(unsigned int l) { left_=l; }
	void SetDesignTop(unsigned int t) { top_=t; }
private:
	unsigned int left_, top_;
};
*/



void ButcherControl_SelectionDisplay::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	dc.SetPen(*wxTRANSPARENT_PEN);
    wxBrush *b=wxTheBrushList->FindOrCreateBrush(GetBackgroundColour());
	//wxBrush *b=wxTheBrushList->FindOrCreateBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    dc.SetBrush(*b);

	dc.DrawRectangle(GetSize());

	if (selectevent_)
	{
		if (selectevent_->GetSelect() == ButcherViewSelectEvent::SEL_LINEHOVER) {
			wxString msg;
			for (unsigned int i=0; i<selectevent_->GetLineSelect()->GetCount(); i++)
			{
				msg+=wxString::Format(wxT("[%s]"), selectevent_->GetLineSelect()->GetItem(i)->GetFullDescription().c_str());
			}
			dc.DrawText(msg, 5, 5);

			ButcherControl_ButcherViewSimulated n(selectevent_->GetProjectView()->GetMask()->GetMaskRoot());
			n.SetSize(0, 10, 50, 50);

			ButcherProjectMaskDrawSelection sel(selectevent_->GetProjectView()->GetMask());
			sel.Set(ButcherProjectMask::DM_HOVER, selectevent_->GetLineSelect());
            selectevent_->GetProjectView()->GetMask()->Draw(&n, &dc,
                ButcherProjectArea::AS_ALL,
                selectevent_->GetProjectView(), ButcherProjectMask::DM_DEFAULT,
                &sel);
		} else if (selectevent_->GetSelect() == ButcherViewSelectEvent::SEL_AREAHOVER) {
			wxString msg;
			ButcherControl_ButcherViewSimulated n(selectevent_->GetProjectView()->GetMask()->GetMaskRoot());
			n.SetSize(10, 10, 100, GetSize().GetHeight());
			//n.SetDesignTop(10);
			for (unsigned int i=0; i<selectevent_->GetAreaSelect()->GetCount(); i++)
			{
				ButcherProjectMaskDrawSelection sel(selectevent_->GetProjectView()->GetMask());
				//sel.Set(ButcherProjectMask::DM_HOVER, selectevent_->GetAreaSelect());
				sel.Add(ButcherProjectMask::DM_HOVER, selectevent_->GetAreaSelect()->GetItem(i));

				selectevent_->GetProjectView()->GetMask()->Draw(&n, &dc,
					ButcherProjectArea::AS_ALL,
					selectevent_->GetProjectView(), ButcherProjectMask::DM_DEFAULT,
					NULL, ButcherProjectMask::DO_DONTDRAWTITLES);
				sel.Draw(&n, &dc, selectevent_->GetProjectView(), ButcherProjectMask::DO_DONTDRAWTITLES);

				msg=wxString::Format(wxT("[%s(%s)]"), selectevent_->GetAreaSelect()->GetItem(i)->GetFullDescription().c_str(),
					selectevent_->GetAreaSelect()->GetItem(i)->Configs().Get(selectevent_->GetProjectView()->GetBLId())->GetFlagsDescription().c_str());

				dc.DrawText(msg, n.GetDesignLeft(), n.GetDesignTop()-10);

				//n.SetDesignLeft(n.GetDesignLeft()+100);
				n.SetDesignTop(n.GetDesignTop()+100);
			}
			//dc.DrawText(msg, 5, 5);


		} else if (selectevent_->GetSelect() == ButcherViewSelectEvent::SEL_NONE) {
			// draw nothing
		}
	}

    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);


    event.Skip();
}

void ButcherControl_SelectionDisplay::OnErase(wxEraseEvent &event)
{
    // do nothing
}



/////////////////////////////////
// CLASS
//      ButcherControl_TransparentStaticText
/////////////////////////////////
BEGIN_EVENT_TABLE (ButcherControl_TransparentStaticText, wxControl)
    EVT_PAINT(ButcherControl_TransparentStaticText::OnPaint)
    EVT_ERASE_BACKGROUND(ButcherControl_TransparentStaticText::OnEraseBackground)
END_EVENT_TABLE()

ButcherControl_TransparentStaticText::ButcherControl_TransparentStaticText(wxWindow* parent, wxWindowID id,
        const wxString &label , const wxPoint& pos ,
        const wxSize& size, long style ,
        const wxString& name )
        : wxControl(parent,id,pos,size,style|wxTRANSPARENT_WINDOW,wxDefaultValidator,name)
{
	SetLabel(label);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

ButcherControl_TransparentStaticText::~ButcherControl_TransparentStaticText()
{

}

bool ButcherControl_TransparentStaticText::HasTransparentBackground() const
{
    return true;
}

void ButcherControl_TransparentStaticText::OnEraseBackground( wxEraseEvent& event )
{
	// do nothing
	//event.Skip();
}
void ButcherControl_TransparentStaticText::OnPaint (wxPaintEvent & event)
{
    wxPaintDC dc(this);
    wxFont my_font = this->GetFont();
	dc.SetFont(my_font);
	dc.SetBackgroundMode(wxTRANSPARENT);
#ifndef __WXGTK__
	dc.SetTextForeground(*wxWHITE);
#else
	dc.SetTextForeground(*wxBLACK);
#endif
    dc.DrawText(this->GetLabel(),0,0);
	//event.Skip();
}



wxSize ButcherControl_TransparentStaticText::DoGetBestSize() const
{
    int x,y;
    GetTextExtent(this->GetLabel(),&x,&y);
    return wxSize(x,y);
}


