//=============================================================================
/**
 *  @file    ButcherViewBase.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherViewBase.h"
#include <math.h>



/////////////////////////////////
// CLASS
//      ButcherViewBase
/////////////////////////////////



wxCoord ButcherViewBase::PosToClient(wxCoord p)
{
#ifndef _MSC_VER
	return lround(p*GetPixelSize());
#else
	return floor((p*GetPixelSize())+0.5);
#endif
}



wxCoord ButcherViewBase::ClientToPos(wxCoord p)
{
#ifndef _MSC_VER
    return lround(p / GetPixelSize());
#else
	return floor((p / GetPixelSize())+0.5);
#endif
}



wxPoint ButcherViewBase::PosToClient(wxPoint p, wxPoint add)
{
    return wxPoint(PosToClient(p.x+add.x), PosToClient(p.y+add.y));
}



wxPoint ButcherViewBase::ClientToPos(wxPoint p, wxPoint add)
{
    return wxPoint(ClientToPos(p.x+add.x), ClientToPos(p.y+add.y));
}



wxRect ButcherViewBase::PosToClient(wxRect r, wxPoint add, wxSize addsize)
{
    return wxRect(
        PosToClient(r.GetLeft()+add.x),
        PosToClient(r.GetTop()+add.y),
        PosToClient(r.GetWidth()+addsize.GetWidth()),
        PosToClient(r.GetHeight()+addsize.GetHeight())
    );
}



wxRect ButcherViewBase::ClientToPos(wxRect r, wxPoint add, wxSize addsize)
{
    return wxRect(
        ClientToPos(r.GetLeft()+add.x),
        ClientToPos(r.GetTop()+add.y),
        ClientToPos(r.GetWidth()+addsize.GetWidth()),
        ClientToPos(r.GetHeight()+addsize.GetHeight())
    );
}


