//=============================================================================
/**
 *  @file   ButcherProjectLine.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectLine.h"
#include "ButcherProjectMaskContainer.h"
#include "ButcherProjectMaskContainerJoined.h"
#include "ButcherProjectMaskContainerSplit.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectArea.h"




/////////////////////////////////
// CLASS
//      ButcherProjectLine
/////////////////////////////////
ButcherProjectLine::ButcherProjectLine(ButcherProjectMaskContainer *container, ButcherProjectLine *start,
    ButcherProjectLine *end, orientation_t orientation) :
        ButcherProjectBaseBLId(container->GetProject()), container_(container), start_(start), end_(end),
            orientation_(orientation), areas_before_(), areas_after_()
{
    if ((start_ && start_->GetContainer() != container) || (end_ && end_->GetContainer() != container))
        throw ButcherException(_("Lines are not on the same mask container"));

    if (start_ && end_ && start_->GetPosition() > end_->GetPosition())
    {
        ButcherProjectLine *tmp=start_;
        start_=end_;
        end_=tmp;
    }
}




ButcherProjectLine::~ButcherProjectLine()
{

}




ButcherProjectMaskContainerJoined *ButcherProjectLine::GetContainerJ()
{
    return dynamic_cast<ButcherProjectMaskContainerJoined*>(container_);
}



ButcherProjectMaskContainerSplit *ButcherProjectLine::GetContainerS()
{
    return dynamic_cast<ButcherProjectMaskContainerSplit*>(container_);
}




ButcherProjectMask *ButcherProjectLine::GetMask()
{
    return container_->GetMask();
}




long ButcherProjectLine::GetGlobalPosition()
{
    return GetPosition()+GetMask()->GetDrawStart(orientation_);
}




long ButcherProjectLine::GetSize()
{
    return GetEnd()->GetPosition()-GetStart()->GetPosition();
}




bool ButcherProjectLine::Connectable(ButcherProjectLine *other, long position)
{
    // must not be the same line
    if (this == other) return false;
    // must be the same orientation
    if (GetOrientation() != other->GetOrientation()) return false;
    // cannot be on the same position - cannot connect 2 lines on the same orientation
    if (GetPosition() == other->GetPosition()) return false;
    // lines must have a common area
    if (start_->GetPosition() > other->GetEnd()->GetPosition()) return false;
    if (end_->GetPosition() < other->GetStart()->GetPosition()) return false;
    // position must be whithin both lines
    if (start_->GetPosition() > position || end_->GetPosition() < position) return false;
    if (other->GetStart()->GetPosition() > position || other->GetEnd()->GetPosition() < position) return false;

    return true;
}



bool ButcherProjectLine::Connected(ButcherProjectLine *other)
{
    return (other == start_ || other == end_);
}




bool ButcherProjectLine::Select(long x, long y)
{
    const long SERROR=3;

    long vor=(orientation_==BPL_HORIZONTAL?y:x), vior=(orientation_==BPL_HORIZONTAL?x:y);
    if (GetPosition()-SERROR <= vor && GetPosition()+SERROR >= vor)
        if (start_->GetPosition()-SERROR <= vior && end_->GetPosition()+SERROR >= vior)
            return true;
    return false;
}




bool ButcherProjectLine::IsMoveable()
{
    return true;
}




bool ButcherProjectLine::CanMove(long position)
{
    return IsMoveable() && op_canmove(position);
}




bool ButcherProjectLine::op_canmove(long position)
{
    ButcherProjectLineMinMax mm=GetMoveMinMax();
    return position >= mm.min && position <= mm.max;
}




void ButcherProjectLine::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    container_->ProjectModified();
}



void ButcherProjectLine::Move(long position)
{
    if (!CanMove(position))
        throw ButcherException(_("Cannot move line"));
    SetPosition(position);
}




void ButcherProjectLine::SetPosition(long p)
{
    throw ButcherException(_("Cannot set position"));
}





ButcherProjectLineMinMax ButcherProjectLine::GetMoveMinMax()
{
    //ButcherProjectLineMinMax m={GetPosition(), GetPosition()};
    ButcherProjectLineMinMax m={GetContainer()->GetMarginStart(GetOrientation()),
        GetMask()->GetSize(GetOrientation())-GetContainer()->GetMarginEnd(GetOrientation())}; // all mask area less margins
    //if (!CanEdit()) return m;

    ButcherProjectLineAreas *alist[2] = {&areas_before_, &areas_after_};
    long tpos;

    ButcherProjectLineAreas::iterator i;
    for (int alct=0; alct<2; alct++) {
        for (i=alist[alct]->begin(); i!=alist[alct]->end(); i++)
        {
            if (alct==0) {
                // before
                tpos=(*i)->GetStartMinMaxPos(GetOrientation())+1;
                if (tpos > m.min)
                    m.min=tpos;
            } else {
                // after
                tpos=(*i)->GetEndMinMaxPos(GetOrientation())-1;
                if (tpos < m.max)
                    m.max=tpos;//-1; why -1??
            }
        }
    }

    return m;
}




ButcherProjectArea *ButcherProjectLine::FindArea(areapos_t pos, long position)
{
    ButcherProjectLineAreas *alist=(pos==LAP_BEFORE?&areas_before_:&areas_after_);

    ButcherProjectLineAreas::iterator i;
    for (i=alist->begin(); i!=alist->end(); i++)
    {
        if ((*i)->GetStart(GetInvOrientation())->GetPosition() <= position && (*i)->GetEnd(GetInvOrientation())->GetPosition() >= position)
            return *i;
    }
    return NULL;
}



// find an area which start and end of inverted orientation are the same
ButcherProjectArea *ButcherProjectLine::FindJoinArea(areapos_t pos, ButcherProjectArea *basearea)
{
    ButcherProjectLineAreas *alist=(pos==LAP_BEFORE?&areas_before_:&areas_after_);

    ButcherProjectLineAreas::iterator i;
    for (i=alist->begin(); i!=alist->end(); i++)
    {
        if ((*i)->GetStart(GetInvOrientation()) == basearea->GetStart(GetInvOrientation()) &&
            (*i)->GetEnd(GetInvOrientation()) == basearea->GetEnd(GetInvOrientation()))
            return *i;
    }
    return NULL;
}





wxString ButcherProjectLine::GetDescription()
{
    return wxString::Format(_("Line %d"), GetBLId());
}




wxString ButcherProjectLine::GetFullDescription()
{
    wxString temp=GetDescription(), temp2=GetPosDescription(), temp3=GetOrientationDescription();
    if (!temp2.IsEmpty())
        temp+=wxString::Format(wxT(" (%s)"), temp2.c_str());
    if (!temp3.IsEmpty())
        temp+=wxString::Format(wxT(" (%s)"), temp3.c_str());
    ButcherProjectMask *ms;
    ms=GetMask();
    while (ms->GetParent())
    {
        temp=ms->GetParent()->GetDescription()+wxT("->")+temp;
        ms=ms->GetParent()->GetMask();
    };
    return temp;
}




