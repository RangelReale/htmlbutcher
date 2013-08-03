//=============================================================================
/**
 *  @file   ButcherProjectLineEnd.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectLineEnd.h"
#include "ButcherProjectMask.h"



/////////////////////////////////
// CLASS
//      ButcherProjectLineEnd
/////////////////////////////////
long ButcherProjectLineEnd::GetPosition()
{
    return GetMask()->GetSize(GetOrientation())-GetContainer()->GetMarginEnd(GetOrientation());
}




ButcherProjectLineMinMax ButcherProjectLineEnd::GetMoveMinMax()
{
    ButcherProjectLineMinMax ret=ButcherProjectLine::GetMoveMinMax();
/*
	if (GetContainer()->GetMask()->GetParent()==NULL)
		// if root mask, end line have infinite maximum
		ret.max=100000;
	else
*/
		// ignore margins
		ret.max=GetMask()->GetSize(GetOrientation());//-1;
    return ret;
}




void ButcherProjectLineEnd::SetPosition(long p)
{
    if (GetOrientation()==ButcherProjectLine::BPL_HORIZONTAL)
        GetContainer()->SetMarginBottom(GetMask()->GetHeight()-p);
    else
        GetContainer()->SetMarginRight(GetMask()->GetWidth()-p);
    ProjectModified();
}




wxString ButcherProjectLineEnd::GetPosDescription()
{
    if (GetOrientation()==ButcherProjectLine::BPL_HORIZONTAL)
        return _("BOTTOM");
    else
        return _("RIGHT");
}




bool ButcherProjectLineEnd::IsMoveable()
{
    // only root masks can have margins, and global areas can be resized
    return
        (GetMask()->GetParent()==NULL);
}


