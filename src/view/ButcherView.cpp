//=============================================================================
/**
 *  @file    ButcherView.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherView.h"
#include "ButcherProject.h"
#include "ButcherProjectView.h"
#include "ButcherProjectFile.h"
#include "ButcherProjectMaskDrawSelection.h"
#include "ButcherRuler.h"
#include "ButcherDocument.h"



/////////////////////////////////
// CLASS
//      ButcherView
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_BUTCHEROPERATION_ACTION )
DEFINE_EVENT_TYPE( wxEVT_BUTCHERSCROLL_ACTION )

BEGIN_EVENT_TABLE(ButcherView, wxControl)
    EVT_SIZE(ButcherView::OnSize)
    EVT_SET_FOCUS(ButcherView::OnSetFocus)

    EVT_BUTCHERDOCUMENTDRAW(wxID_ANY, ButcherView::OnDrawDocument)
    EVT_BUTCHERSCROLL(wxID_ANY, ButcherView::OnDWindowRealWinScroll)
END_EVENT_TABLE()



ButcherProjectArea::areaselect_t ButcherView_AreaSelect(bool showareas, bool showareasglobal)
{
    ButcherProjectArea::areaselect_t ret=ButcherProjectArea::AS_NONE;
    if (showareas) ret=ret|ButcherProjectArea::AS_AREA|ButcherProjectArea::AS_AREAMAP;
    if (showareasglobal) ret=ret|ButcherProjectArea::AS_AREAGLOBAL;
    return ret;
    //if (!showareas&&!showareasglobal) return ButcherProjectArea::AS_NONE;
    //return (showareasglobal?(showareas?ButcherProjectArea::AS_ALL:ButcherProjectArea::AS_AREAGLOBAL):ButcherProjectArea::AS_AREA);
}




ButcherProjectArea::areaselect_t ButcherView_AreaSelect(ButcherView::areaview_t areaview)
{
    ButcherProjectArea::areaselect_t ret=ButcherProjectArea::AS_NONE;
    if ((areaview&ButcherView::AV_AREA)==ButcherView::AV_AREA) ret|=ButcherProjectArea::AS_AREA;
    if ((areaview&ButcherView::AV_AREAGLOBAL)==ButcherView::AV_AREAGLOBAL) ret|=ButcherProjectArea::AS_AREAGLOBAL;
    if ((areaview&ButcherView::AV_AREAMAP)==ButcherView::AV_AREAMAP) ret|=ButcherProjectArea::AS_AREAMAP;
    return ret;
}




ButcherView::ButcherView(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style,
    const wxString& name) :
    wxControl(parent, id, pos, size, style, wxDefaultValidator, name),
    selection_(NULL), filealternate_(false), filealternateid_(-1)
{
    dwindow_=new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    dwindow_->SetScrollRate( 10, 10 );

    wxASSERT_MSG(wxEVT_SCROLLWIN_TOP<=wxEVT_SCROLLWIN_THUMBRELEASE, _("Logic error in scrollwin events"));
    for (int i=wxEVT_SCROLLWIN_TOP; i<=wxEVT_SCROLLWIN_THUMBRELEASE; i++) {
        dwindow_->Connect(wxID_ANY, i,
            wxScrollWinEventHandler(ButcherView::OnDWindowWinScroll), NULL, this);
    }
    //dwindow_->Connect(wxID_ANY, wxEVT_LEAVE_WINDOW, wxMouseEventHandler(ButcherViewEditor::OnDWindowLeave), NULL, this);

    ruler_left_=new ButcherRuler(this, ButcherRuler::BRK_LEFT);//, wxID_ANY, wxDefaultPosition, wxSize(rulersize_, -1));
    ruler_top_=new ButcherRuler(this, ButcherRuler::BRK_TOP);//, wxID_ANY, wxDefaultPosition, wxSize(-1, rulersize_));
    ruler_right_=new ButcherRuler(this, ButcherRuler::BRK_RIGHT);//, wxID_ANY, wxDefaultPosition, wxSize(rulersize_, -1));
    ruler_bottom_=new ButcherRuler(this, ButcherRuler::BRK_BOTTOM);//, wxID_ANY, wxDefaultPosition, wxSize(-1, rulersize_));

    designer_ = new ButcherDocument(this, dwindow_/*, wxID_ANY, wxPoint(rulersize_, rulersize_), wxSize(designwidth_, designheight_)*/);
    //designer_->SetBackgroundColour(*wxGREEN);

    RepositionView();

    SetCursor(*wxCROSS_CURSOR);
}




ButcherView::~ButcherView()
{
    DoChangedEvent(ButcherViewChangedEvent::VC_DESTROY);

    SelectionClear(false);
}



void ButcherView::RepositionView()
{
    if (GetShowRulers())
    {
        ruler_left_->SetSize(0, GetRulerSize(), GetRulerSize(), GetClientSize().GetHeight()-(2*GetRulerSize()));
        ruler_top_->SetSize(GetRulerSize(), 0, GetClientSize().GetWidth()-(2*GetRulerSize()), GetRulerSize());
        ruler_right_->SetSize(GetClientSize().GetWidth()-GetRulerSize(), GetRulerSize(), GetRulerSize(), GetClientSize().GetHeight()-(2*GetRulerSize()));
        ruler_bottom_->SetSize(GetRulerSize(), GetClientSize().GetHeight()-GetRulerSize(), GetClientSize().GetWidth()-(2*GetRulerSize()), GetRulerSize());

        dwindow_->SetSize(GetRulerSize(), GetRulerSize(), GetClientSize().GetWidth()-(2*GetRulerSize()), GetClientSize().GetHeight()-(2*GetRulerSize()));
    }
    else
    {
        dwindow_->SetSize(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());
    }
    ruler_left_->Show(GetShowRulers());
    ruler_top_->Show(GetShowRulers());
    ruler_right_->Show(GetShowRulers());
    ruler_bottom_->Show(GetShowRulers());

    dwindow_->SetVirtualSize( PosToClient(GetDesignWidth())+10, PosToClient(GetDesignHeight())+10 );

    int sw, sh;
    dwindow_->CalcUnscrolledPosition(0, 0, &sw, &sh);

    designer_->SetSize(0-sw, 0-sh, PosToClient(GetDesignWidth()), PosToClient(GetDesignHeight()));

    RepositionScroll();

    Refresh();

    DoChangedEvent(ButcherViewChangedEvent::VC_VIEW);
}




void ButcherView::RepositionScroll(int orientation)
{
    int sw, sh;
/*
    dwindow_->GetViewStart(&sw, &sh);

    wxLogDebug(wxT("RepositionScroll VS = sw: %d [%d] - sh: %d [%d]"),
        sw, ClientToPos(sw), sh, ClientToPos(sh));

*/
    dwindow_->CalcUnscrolledPosition(0, 0, &sw, &sh);

/*
    wxLogDebug(wxT("RepositionScroll = sw: %d [%d] - sh: %d [%d] - pos: %d"),
        sw, ClientToPos(sw), sh, ClientToPos(sh), orientation);
*/

    switch (orientation) {
    case wxVERTICAL:
    case wxBOTH:
        ruler_left_->SetStart(ClientToPos(sh));
        ruler_right_->SetStart(ClientToPos(sh));
        break;
    default:
        break;
    }

    switch (orientation) {
    case wxHORIZONTAL:
    case wxBOTH:
        ruler_top_->SetStart(ClientToPos(sw));
        ruler_bottom_->SetStart(ClientToPos(sw));
        break;
    default:
        break;
    }
}



void ButcherView::OnSize(wxSizeEvent &event)
{
    RepositionView();
    event.Skip();
}




void ButcherView::OnDWindowWinScroll(wxScrollWinEvent &event)
{
    DoBeforeScroll();

    // this event is fired BEFORE the scrolling, post another event to catch it AFTER scrolling took place
    wxScrollWinEvent evt(event);
    evt.SetEventType(wxEVT_BUTCHERSCROLL_ACTION);
    evt.SetEventObject(this);
    wxPostEvent(this, evt);

    event.Skip();
}




void ButcherView::OnDWindowRealWinScroll(wxScrollWinEvent &event)
{
    DoAfterScroll();

    RepositionScroll(event.GetOrientation());
    event.Skip();
}




void ButcherView::OnSetFocus(wxFocusEvent &event)
{
    //wxLogDebug(wxT("Focus"));

    //designer_->SetFocusIgnoringChildren();
    event.Skip();
}



void ButcherView::OnDrawDocument(ButcherDocumentDrawEvent& event)
{
    if (GetProjectView()!=NULL) {
        // draw image

        if (!GetShowPreview()) // if preview, imagens will be drawn per area
        {
            ButcherProjectFile *fl=GetProjectFile();
            if (fl->GetImage())
            {
                wxRect rc(0, 0,
                    (int)(fl->GetImage()->GetWidth()*event.GetDocument()->GetView()->GetPixelSize()),
                    (int)(fl->GetImage()->GetHeight()*event.GetDocument()->GetView()->GetPixelSize())
                );

                fl->GetImage()->Draw(*event.GetDC(), rc);
            }
        }
        else
        {
            GetProjectView()->DrawPreview(this, event.GetDC());
            GetProjectView()->GetMask()->DrawPreview(this, event.GetDC(),
                ButcherView_AreaSelect(GetAreaViewCurrent()),
                GetProjectView());
        }

        // Draw grid
        if (GetShowGrid())
            event.GetDocument()->DrawGrid(event.GetDC());

        // draw mask
        if (GetShowBordersCurrent())
                GetProjectView()->GetMask()->Draw(this, event.GetDC(),
                    ButcherView_AreaSelect(GetAreaViewCurrent()),
                    GetProjectView(), ButcherProjectMask::DM_DEFAULT,
                    selection_);
    }
    DoAfterDraw(event);

    event.Skip();
}




void ButcherView::RulerPosition(long x, long y)
{
    ruler_left_->SetSelection(y);
    ruler_right_->SetSelection(y);
    ruler_top_->SetSelection(x);
    ruler_bottom_->SetSelection(x);
}




ButcherProjectView *ButcherView::GetProjectView()
{
    if (GetProject() && GetProjectViewId() > 0)
        return GetProject()->Views().Get(GetProjectViewId());
    return NULL;
}




ButcherProjectFile *ButcherView::GetProjectFile()
{
    if (!GetProjectView()) return NULL;

    if ((!filealternate_) ||
        (filealternateid_<0) ||
        (filealternateid_>BUTCHERCONST_VIEW_MAXALTERNATE-1) ||
        (GetProjectView()->GetFileAlternate(filealternateid_)) == NULL)
        return GetProjectView()->GetFile();
    else
        return GetProjectView()->GetFileAlternate(filealternateid_);
}





float ButcherView::GetPixelSize()
{
    return 1*(GetZoom()/100.0);
}



void ButcherView::DoChangedEvent(ButcherViewChangedEvent::change_t change, long movex, long movey)
{
    ButcherViewChangedEvent event(change);
    event.SetMove(movex, movey);
    event.SetEventObject(this);

    ProcessEvent(event);
}




void ButcherView::ViewCenter(long x, long y)
{
    int xu, yu;
    dwindow_->GetScrollPixelsPerUnit(&xu, &yu);

    dwindow_->Scroll(PosToClient(x/xu)-(GetClientSize().GetWidth()/2/xu),
        PosToClient(y/yu)-(GetClientSize().GetHeight()/2/yu));
}




void ButcherView::SetFocus()
{
    designer_->SetFocus();
    //SetFocusIgnoringChildren();
}




void ButcherView::SelectionClear(bool refresh, bool hoveronly)
{
    if (selection_)
    {
        if (!hoveronly)
        {
            delete selection_;
            selection_=NULL;
        }
        else
        {
            selection_->Set(ButcherProjectMask::DM_HOVER, static_cast<ButcherProjectMaskAreaSelect*>(NULL));
            selection_->Set(ButcherProjectMask::DM_HOVER, static_cast<ButcherProjectMaskLineSelect*>(NULL));
        }
        if (refresh)
            Refresh();
    }
}



void ButcherView::ProjectChanged()
{
    filealternateid_=-1;
}




void ButcherView::SetFileAlternate(bool s, bool refresh)
{
    filealternate_=s;
    if (filealternate_ && GetProjectView() && GetProjectView()->GetFileAlternate(filealternateid_)==NULL)
    {
        // find first alternate file
        filealternateid_=-1;
        for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        {
            if (GetProjectView()->GetFileAlternate(i))
            {
                filealternateid_=i;
                break;
            }
        }
    }
    if (refresh)
        Refresh();
}



