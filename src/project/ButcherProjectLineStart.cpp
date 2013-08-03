//=============================================================================
/**
 *  @file   ButcherProjectLineStart.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectLineStart.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectMaskContainer.h"



/////////////////////////////////
// CLASS
//      ButcherProjectLineStart
/////////////////////////////////
long ButcherProjectLineStart::GetPosition()
{
    return GetContainer()->GetMarginStart(GetOrientation());
}




ButcherProjectLineMinMax ButcherProjectLineStart::GetMoveMinMax()
{
    ButcherProjectLineMinMax ret=ButcherProjectLine::GetMoveMinMax();
    ret.min=0; // ignore margins
/*
    if (IsResizeable())
        ret.min-=GetMask()->GetDrawStart(GetOrientation());
*/
    return ret;
}



void ButcherProjectLineStart::SetPosition(long p)
{
    if (GetOrientation()==ButcherProjectLine::BPL_HORIZONTAL)
        GetContainer()->SetMarginTop(p);
    else
        GetContainer()->SetMarginLeft(p);
    ProjectModified();
}




wxString ButcherProjectLineStart::GetPosDescription()
{
    if (GetOrientation()==ButcherProjectLine::BPL_HORIZONTAL)
        return _("TOP");
    else
        return _("LEFT");
}




bool ButcherProjectLineStart::IsMoveable()
{
    // only root masks can have margins, and global areas can be resized
    return
        (GetMask()->GetParent()==NULL);
}


