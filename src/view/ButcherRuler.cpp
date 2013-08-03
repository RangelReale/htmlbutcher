//=============================================================================
/**
 *  @file    ButcherRuler.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherRuler.h"
#include "ButcherView.h"

#include <wx/dcbuffer.h>



/////////////////////////////////
// CLASS
//      ButcherRuler
/////////////////////////////////
BEGIN_EVENT_TABLE(ButcherRuler, wxControl)
    EVT_PAINT(ButcherRuler::OnPaint)
    EVT_ERASE_BACKGROUND(ButcherRuler::OnEraseBackground)
END_EVENT_TABLE()



ButcherRuler::ButcherRuler(ButcherView* parent, rulerkind_t kind, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style,
    const wxString& name) :
    wxControl(parent, id, pos, size, style, wxDefaultValidator, name),
    view_(parent), kind_(kind), linesmallpct_(25), linebigpct_(50),
    start_(0), position_(-1)
{

}



void ButcherRuler::OnEraseBackground(wxEraseEvent &event)
{

}




void ButcherRuler::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(this);
    //dc.SetPen(*wxBLACK_PEN);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE)));

    dc.DrawRectangle(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());

    dc.SetPen(*wxBLACK_PEN);

    int max, lsmall, lbig;
    switch (kind_) {
    case BRK_LEFT:
    case BRK_RIGHT:
        max=view_->ClientToPos(GetClientSize().GetHeight());
        lsmall=(int)(GetClientSize().GetWidth()*(linesmallpct_/100.0));
        lbig=(int)(GetClientSize().GetWidth()*(linebigpct_/100.0));

        if (kind_==BRK_LEFT)
            dc.DrawLine(GetClientSize().GetWidth()-1, 0, GetClientSize().GetWidth()-1, GetClientSize().GetHeight());
        else
            dc.DrawLine(0, 0, 0, GetClientSize().GetHeight());

        break;
    case BRK_TOP:
    case BRK_BOTTOM:
        max=view_->ClientToPos(GetClientSize().GetWidth());
        lsmall=(int)(GetClientSize().GetHeight()*(linesmallpct_/100.0));
        lbig=(int)(GetClientSize().GetHeight()*(linebigpct_/100.0));

        if (kind_==BRK_TOP)
            dc.DrawLine(0, GetClientSize().GetHeight()-1, GetClientSize().GetWidth(), GetClientSize().GetHeight()-1);
        else
            dc.DrawLine(0, 0, GetClientSize().GetWidth(), 0);

        break;
    default:
        max=lsmall=lbig=0;
        break;
    }

    wxFont font(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    font.SetPointSize(7);
    //wxFont font(12, wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL, false);
    dc.SetFont(font);
    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.SetTextForeground(*wxBLACK);

    wxString postext;
    wxCoord postextw, postexth;

    // calculate pixel spacing based on maximum text width
    int d_start=0, d_skip=5, d_mult=10;

    // adjust start position
    max+=start_; d_start+=start_;

    postext=wxString::Format(wxT("%d"), max);
    dc.GetTextExtent(postext, &postextw, &postexth);

    if (postextw > view_->PosToClient(d_mult)-2) {
        /*d_start=0;*/ d_skip=10; d_mult=50;
    }

    // adjust start to mult
    int ddiff=d_start % d_mult;
    if (ddiff != 0) {
        d_start+=d_mult-ddiff;
    }

    wxPoint textstart(0, 0);
    bool isbig;
    int lsize, clientpos, postextor;
    for (int p=d_start; p<=max; p+=d_skip)
    {
        isbig=(p%d_mult)==0;
        lsize=(isbig?lbig:lsmall);

        if (isbig) {
            postext=wxString::Format(wxT("%d"), p);
            dc.GetTextExtent(postext, &postextw, &postexth);
        }

        clientpos=view_->PosToClient(p-start_);
        //if (clientpos!=0) clientpos--;

        switch (kind_) {
        case BRK_LEFT:
            dc.DrawLine(GetClientSize().GetWidth()-lsize, clientpos, GetClientSize().GetWidth(), clientpos);
            if (isbig) {
                textstart.x=0;
                textstart.y=clientpos+(postextw/2);
                postextor=90;
            }
            break;
        case BRK_TOP:
            dc.DrawLine(clientpos, GetClientSize().GetHeight()-lsize, clientpos, GetClientSize().GetHeight());
            if (isbig) {
                textstart.x=clientpos-(postextw/2);
                textstart.y=0;
                postextor=0;
            }
            break;
        case BRK_RIGHT:
            dc.DrawLine(0, clientpos, lsize, clientpos);
            if (isbig) {
                textstart.x=GetClientSize().GetWidth();
                textstart.y=clientpos-(postextw/2);
                postextor=270;
            }
            break;
        case BRK_BOTTOM:
            dc.DrawLine(clientpos, 0, clientpos, lsize);
            if (isbig) {
                textstart.x=clientpos-(postextw/2);
                textstart.y=GetClientSize().GetHeight()-postexth;
                postextor=0;
            }
            break;
        default:
            postextor=0;
            break;
        }

        if (isbig && p>0 && p<max) {
            // draw position text
            dc.DrawRotatedText(postext, textstart, postextor);
        }

    }

    if (position_>=0 && position_>=start_)
    {
        clientpos=view_->PosToClient(position_-start_);

        switch (kind_) {
        case BRK_LEFT:
        case BRK_RIGHT:
            dc.DrawLine(0, clientpos, GetClientSize().GetWidth(), clientpos);
            break;

        case BRK_TOP:
        case BRK_BOTTOM:
            dc.DrawLine(clientpos, 0, clientpos, GetClientSize().GetWidth());
            break;
        }
    }

    //dc.DrawLine(10, 0, 10, 30);

    dc.SetFont(wxNullFont);
    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}



void ButcherRuler::SetSelection(long position)
{
    if (position!=position_)
    {
        long oldpos=position_;
        position_=position;
        // clean old rectangle
        RefreshRect(PosRect(oldpos, 1), false);

        if (position_>=0 && position_>start_)
        {
            // draw new rectangle
            RefreshRect(PosRect(position_, 1), false);
        }
    }
}




wxRect ButcherRuler::PosRect(long position, long spacing)
{
    long clientpos=view_->PosToClient(position-start_);

    switch (kind_) {
    case BRK_LEFT:
    case BRK_RIGHT:
        return wxRect(wxPoint(0, clientpos-spacing), wxPoint(GetClientSize().GetWidth(), clientpos+spacing));
        break;

    case BRK_TOP:
    case BRK_BOTTOM:
        return wxRect(wxPoint(clientpos-spacing, 0), wxPoint(clientpos+spacing, GetClientSize().GetWidth()));
        break;
    }
    return wxRect(0, 0, 0, 0);
}


