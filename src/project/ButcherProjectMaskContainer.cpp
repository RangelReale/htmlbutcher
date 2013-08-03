//=============================================================================
/**
 *  @file   ButcherProjectMaskContainer.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskContainer.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherFileDefs.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskContainer
/////////////////////////////////
ButcherProjectMaskContainer::ButcherProjectMaskContainer(ButcherProjectMask *mask,
        ButcherProjectArea::areaclass_t areaclass) :
    ButcherProjectBaseModify(mask->GetProject()), lines_(this), areas_(this), mask_(mask),
        areaclass_(areaclass)
{

}



int ButcherProjectMaskContainer::GetMarginStart(ButcherProjectLine::orientation_t orientation)
{
    if (orientation==ButcherProjectLine::BPL_HORIZONTAL)
        return GetMarginTop();
    return GetMarginLeft();
}




int ButcherProjectMaskContainer::GetMarginEnd(ButcherProjectLine::orientation_t orientation)
{
    if (orientation==ButcherProjectLine::BPL_HORIZONTAL)
        return GetMarginBottom();
    return GetMarginRight();
}




long ButcherProjectMaskContainer::GetStartMinMaxPos(ButcherProjectLine::orientation_t orientation)
{
    return 0;
}





long ButcherProjectMaskContainer::GetEndMinMaxPos(ButcherProjectLine::orientation_t orientation)
{
    return GetMask()->GetSize(orientation);
}




int ButcherProjectMaskContainer::GetClientWidth()
{
    return mask_->GetWidth()-GetMarginLeft()-GetMarginRight();
}




int ButcherProjectMaskContainer::GetClientHeight()
{
    return mask_->GetHeight()-GetMarginTop()-GetMarginBottom();
}




ButcherProjectLine *ButcherProjectMaskContainer::AddLine(ButcherProjectLine *start, ButcherProjectLine *end,
    ButcherProjectLine::orientation_t orientation, unsigned long position,
    BLID_t id)
{
    throw ButcherException(_("Operation not supported."));
}




bool ButcherProjectMaskContainer::DeleteLine(ButcherProjectLine *line)
{
    throw ButcherException(_("Operation not supported."));
	return false;
}




ButcherProjectArea *ButcherProjectMaskContainer::AddArea(long left, long top, long right, long bottom, BLID_t id)
{
    throw ButcherException(_("Operation not supported."));
}




void ButcherProjectMaskContainer::DeleteArea(ButcherProjectArea *area)
{
    throw ButcherException(_("Operation not supported."));
}




ButcherProjectArea::areaclass_t ButcherProjectMaskContainer::GetContainedAreaClass()
{
    if (GetAreaClass()!=ButcherProjectArea::AC_DEFAULT)
        return GetAreaClass();
    if (GetMask()->GetParent())
        return GetMask()->GetParent()->GetContainedAreaClass();
    return GetAreaClass();
}



bool ButcherProjectMaskContainer::IsAreaSelect(ButcherProjectArea::areaselect_t areas)
{
    if (areas==ButcherProjectArea::AS_NONE) return false;
    if ((areas&ButcherProjectArea::AS_ALL)==ButcherProjectArea::AS_ALL) return true;

    switch (GetAreaClass())
    {
    case ButcherProjectArea::AC_GLOBAL:
        return (areas&ButcherProjectArea::AS_AREAGLOBAL)==ButcherProjectArea::AS_AREAGLOBAL;
    case ButcherProjectArea::AC_MAP:
        return (areas&ButcherProjectArea::AS_AREAMAP)==ButcherProjectArea::AS_AREAMAP;
    case ButcherProjectArea::AC_DEFAULT:
        return (areas&ButcherProjectArea::AS_AREA)==ButcherProjectArea::AS_AREA;
    default:
        return false;
    }
}




ButcherProjectArea *ButcherProjectMaskContainer::FindAreaByLines(BLID_t left, BLID_t top, BLID_t right, BLID_t bottom)
{
	for (ButcherProjectAreas::iterator i=areas_.begin(); i!=areas_.end(); i++)
	{
        if (i->GetLeft()->GetBLId()==left &&
            i->GetTop()->GetBLId()==top &&
            i->GetRight()->GetBLId()==right &&
            i->GetBottom()->GetBLId()==bottom)
            return &*i;
	}

    return NULL;
}




ButcherProjectArea::areaflags_t ButcherProjectMaskContainer::GetFlags()
{
    return InternalGetFlags();
}



ButcherProjectArea::areaflags_t ButcherProjectMaskContainer::InternalGetFlags(ButcherProjectArea::areaflags_t curflags)
{
    if (GetMask()->GetParent())
        return GetMask()->GetParent()->GetContainer()->InternalGetFlags(curflags);
    return curflags;
}





void ButcherProjectMaskContainer::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    mask_->ProjectModified();
}





void ButcherProjectMaskContainer::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_AREACLASS, ButcherMetadataDataItem::DT_INTEGER)))
        throw ButcherException(_("Invalid metadata for container"));

    Clear();

    areaclass_=static_cast<ButcherProjectArea::areaclass_t>(metadata->Get(BFILE_MDI_AREACLASS).GetInteger());
}




void ButcherProjectMaskContainer::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_AREACLASS, areaclass_));
}


