//=============================================================================
/**
 *  @file    ButcherViewDisplay.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherViewDisplay.h"
#include "ButcherDocument.h"



/////////////////////////////////
// CLASS
//      ButcherViewDisplay
/////////////////////////////////

#ifdef QT_HIDE_FROM

BEGIN_EVENT_TABLE(ButcherViewDisplay, ButcherView)
END_EVENT_TABLE()

#endif // QT_HIDE_FROM

ButcherViewDisplay::ButcherViewDisplay(QWidget* parent/*, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style,
    const wxString& name*/) :
    //ButcherView(parent, id, pos, size, style, name), 
	ButcherView(parent),
	parentview_(NULL),
    zoom_(100), drawtarget_(-1, -1)
{
    //dwindow_->SetScrollbars(0, 0, 0, 0);
    ///dwindow_->SetScrollRate( 1, 1 );
    //dwindow_->EnableScrolling(false, false);

    ///dwindow_->SetWindowStyleFlag( dwindow_->GetWindowStyle() & ~ wxHSCROLL );
    ///dwindow_->SetWindowStyleFlag( dwindow_->GetWindowStyle() & ~ wxVSCROLL );
}




ButcherViewDisplay::~ButcherViewDisplay()
{
    SetParentView(NULL);
}




void ButcherViewDisplay::SetParentView(ButcherView *pv)
{
    if (pv==parentview_) return;

#ifdef QT_HIDE_FROM
	if (parentview_)
        parentview_->Disconnect(wxID_ANY, wxEVT_BUTCHERVIEWCHANGED_ACTION,
            ButcherViewChangedEventHandler(ButcherViewDisplay::OnViewChanged),
            NULL, this);
    parentview_=pv;
    if (parentview_)
        parentview_->Connect(wxID_ANY, wxEVT_BUTCHERVIEWCHANGED_ACTION,
            ButcherViewChangedEventHandler(ButcherViewDisplay::OnViewChanged),
            NULL, this);
#endif // QT_HIDE_FROM

    parentview_=pv;
    ParentViewChanged();
}



ButcherProject *ButcherViewDisplay::GetProject()
{
    if (parentview_) return parentview_->GetProject();
    return NULL;
}




BLID_t ButcherViewDisplay::GetProjectViewId()
{
    if (parentview_) return parentview_->GetProjectViewId();
    return 0;
}




unsigned int ButcherViewDisplay::GetDesignWidth()
{
    if (parentview_) return parentview_->GetDesignWidth();
    return 0;
}




unsigned int ButcherViewDisplay::GetDesignHeight()
{
    if (parentview_) return parentview_->GetDesignHeight();
    return 0;
}




void ButcherViewDisplay::ParentViewChanged()
{
    RepositionView();
    //Refresh();
}




void ButcherViewDisplay::SetZoom(unsigned short z)
{
    zoom_=z;
    RepositionView();
    //Refresh();
}




void ButcherViewDisplay::OnViewChanged(ButcherViewChangedEvent &event)
{
    switch (event.GetChange())
    {
    case ButcherViewChangedEvent::VC_VIEW:
        RepositionView();
        break;
    case ButcherViewChangedEvent::VC_MOVE:
        ViewCenter(event.GetX(), event.GetY());
        DrawTarget(event.GetX(), event.GetY());
        break;
    case ButcherViewChangedEvent::VC_DESTROY:
        SetParentView(NULL);
        break;
    }
    ///event.Skip();
}



void ButcherViewDisplay::DrawTarget(long x, long y, wxDC *dc)
{
#ifdef QT_HIDE_FROM
	if (drawtarget_.x>-1)
    {
        int vx, vy;
        dwindow_->GetViewStart(&vx, &vy);

        //wxRect cr(PosToClient(drawtarget_), wxSize(10, 10));
        wxRect cr(vx, vy, GetClientSize().GetWidth(), GetClientSize().GetHeight());
        designer_->RefreshRect(cr, false);
        designer_->Update();
    }
    drawtarget_.x=x;
    drawtarget_.y=y;

    //Refresh();

    bool isdc=(dc!=NULL);
    if (!isdc)
        dc=new wxWindowDC(designer_);

    dc->SetPen(*wxRED_PEN);
    dc->SetBrush(*wxTRANSPARENT_BRUSH);

    wxRect dx(wxPoint(PosToClient(x)-5, PosToClient(y)-5), wxSize(10, 10));

    dc->DrawRectangle(dx);
    dc->DrawPoint(dx.GetLeft(), dx.GetTop());

    dc->SetPen(wxNullPen);
    dc->SetBrush(wxNullBrush);

    if (!isdc)
        delete dc;
#endif // QT_HIDE_FROM
}


