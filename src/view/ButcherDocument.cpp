//=============================================================================
/**
 *  @file    ButcherDocument.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherDocument.h"
#include "ButcherView.h"
#include "ButcherDocumentKeyEvent.h"
#include "ButcherDocumentDrawEvent.h"
#include "ButcherDocumentMouseEvent.h"

#include <wx/dcbuffer.h>



/////////////////////////////////
// CLASS
//      ButcherDocument
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTKEYBOARD_ACTION )


BEGIN_EVENT_TABLE(ButcherDocument, wxControl)
    EVT_PAINT(ButcherDocument::OnPaint)
    EVT_ERASE_BACKGROUND(ButcherDocument::OnEraseBackground)
    EVT_MOUSE_EVENTS(ButcherDocument::OnMouse)
    EVT_KEY_DOWN(ButcherDocument::OnKeyboard)
    EVT_KEY_UP(ButcherDocument::OnKeyboard)
	EVT_CHILD_FOCUS(ButcherDocument::OnChildFocus) 
    //EVT_CHAR(ButcherDocument::OnKeyboard)
END_EVENT_TABLE()

ButcherDocument::ButcherDocument(ButcherView* view, wxWindow *parent,
    wxWindowID id,
    const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) :
    wxControl(parent, id, pos, size, style | wxWANTS_CHARS, wxDefaultValidator, name),
    view_(view)
{

}




void ButcherDocument::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(this);

    dc.SetPen(*wxTRANSPARENT_PEN);
    //dc.SetBrush(*wxWHITE_BRUSH);
	dc.SetBrush(*wxBLACK_BRUSH);

    dc.DrawRectangle(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());

    ButcherDocumentDrawEvent evt(this, &dc, GetUpdateRegion());
    evt.SetEventObject(this);
    ProcessEvent(evt);

    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}




void ButcherDocument::OnEraseBackground(wxEraseEvent &event)
{

}




void ButcherDocument::OnMouse(wxMouseEvent &event)
{
    ButcherDocumentMouseEvent newe(event);

    newe.m_x=view_->ClientToPos(event.m_x);
    newe.m_y=view_->ClientToPos(event.m_y);

/*
    if (event.GetEventType() == wxEVT_MOTION)
        wxLogDebug(wxT("Mouse = x: %d - y: %d - nx: %d - ny: %d - pixel: %f"),
            event.m_x, event.m_y, newe.m_x, newe.m_y, view_->GetPixelSize());
*/

    view_->GetEventHandler()->ProcessEvent(newe);
    if (newe.GetSkipped())
        event.Skip();
}



void ButcherDocument::OnKeyboard(wxKeyEvent &event)
{
    ButcherDocumentKeyEvent newe(event);

    newe.m_x=view_->ClientToPos(event.m_x);
    newe.m_y=view_->ClientToPos(event.m_y);

/*
    if (event.GetEventType() == wxEVT_MOTION)
        wxLogDebug(wxT("Key = x: %d - y: %d - nx: %d - ny: %d - pixel: %f"),
            event.m_x, event.m_y, newe.m_x, newe.m_y, view_->GetPixelSize());
*/
    view_->GetEventHandler()->ProcessEvent(newe);
    if (newe.GetSkipped())
        event.Skip();
}




void ButcherDocument::DrawGrid(wxDC *dc)
{
	static wxDash gdash[2]={1,2};
	//wxPen *ypen=wxThePenList->FindOrCreatePen(wxT("LIGHT GREY"), 1, wxUSER_DASH);
	wxPen *ypen=wxThePenList->FindOrCreatePen(wxColour(0xA0, 0xA0, 0xA0), 1, wxUSER_DASH);
	ypen->SetDashes(2, gdash);


    dc->SetPen(*ypen);

    unsigned int dpos=view_->GetGridSize();
    while (dpos < view_->GetDesignWidth()) {
        dc->DrawLine(view_->PosToClient(dpos), 0, view_->PosToClient(dpos), GetSize().GetHeight()-1);
        dpos+=view_->GetGridSize();
    }
    dpos=view_->GetGridSize();
    while (dpos < view_->GetDesignHeight()) {
        dc->DrawLine(0, view_->PosToClient(dpos), GetSize().GetWidth()-1, view_->PosToClient(dpos));
        dpos+=view_->GetGridSize();
    }

    dc->SetPen(wxNullPen);
}




void ButcherDocument::OnChildFocus(wxChildFocusEvent &event)
{
	// Prevents the scrolling window from jumping on ACTIVATE events
}


