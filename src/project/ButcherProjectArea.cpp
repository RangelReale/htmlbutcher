//=============================================================================
/**
 *  @file   ButcherProjectArea.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectArea.h"
#include "ButcherProjectMaskContainer.h"
#include "ButcherProjectMaskContainerJoined.h"
#include "ButcherProjectMaskContainerSplit.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectArea
/////////////////////////////////

ButcherProjectArea::ButcherProjectArea(ButcherProjectMaskContainer *container,
    ButcherProjectLine *start1, ButcherProjectLine *start2,
    ButcherProjectLine *end1, ButcherProjectLine *end2/*,
    unsigned long areaid*/) :
    ButcherProjectBaseBLId(container->GetProject()), container_(container),
        name_(wxEmptyString)//, areaid_(areaid)
{
    ButcherProjectBaseAutoDisable autodis(this);

    if (start1->GetContainer() != container || end1->GetContainer() != container ||
        start2->GetContainer() != container || end2->GetContainer() != container)
        throw ButcherException(_("Lines are not on the same mask container"));


    if (start1->GetOrientation() != end1->GetOrientation() ||
        start2->GetOrientation() != end2->GetOrientation() ||
        start1->GetOrientation() == start2->GetOrientation())
        throw ButcherException(_("Invalid lines for area"));

    SetStart(start1);
    SetStart(start2);
    SetEnd(end1);
    SetEnd(end2);

    configs_=new ButcherProjectAreaConfigs(this);
}




ButcherProjectArea::~ButcherProjectArea()
{
    delete configs_;
}



ButcherProjectMaskContainerJoined *ButcherProjectArea::GetContainerJ()
{
    return dynamic_cast<ButcherProjectMaskContainerJoined *>(container_);
}




ButcherProjectMaskContainerSplit *ButcherProjectArea::GetContainerS()
{
    return dynamic_cast<ButcherProjectMaskContainerSplit*>(container_);
}





ButcherProjectMask *ButcherProjectArea::GetMask()
{
    return container_->GetMask();
}





long ButcherProjectArea::GetWidth()
{
    return GetRight()->GetPosition()-GetLeft()->GetPosition();
}




long ButcherProjectArea::GetHeight()
{
    return GetBottom()->GetPosition()-GetTop()->GetPosition();
}




long ButcherProjectArea::GetSize(ButcherProjectLine::orientation_t orientation)
{
    if (orientation == ButcherProjectLine::BPL_HORIZONTAL)
        return GetHeight();
    return GetWidth();
}



bool ButcherProjectArea::CanMove(long left, long top)
{
    return
        ((left == -1) || CanMove(ButcherProjectLine::BPL_VERTICAL, left)) &&
        ((top == -1) || CanMove(ButcherProjectLine::BPL_HORIZONTAL, top));
}




void ButcherProjectArea::Move(long left, long top)
{
    if (!CanMove(left, top))
        throw ButcherException(_("Cannot move to this position"));
    if (left != -1)
        Move(ButcherProjectLine::BPL_VERTICAL, left);
    if (top != -1)
        Move(ButcherProjectLine::BPL_HORIZONTAL, top);
}




bool ButcherProjectArea::CanResize(long width, long height)
{
    return
        ((width == -1) || CanResize(ButcherProjectLine::BPL_VERTICAL, width)) &&
        ((height == -1) || CanResize(ButcherProjectLine::BPL_HORIZONTAL, height));
}

void ButcherProjectArea::Resize(long width, long height)
{
    if (!CanResize(width, height))
        throw ButcherException(_("Cannot change to this size"));
    if (width != -1)
        Resize(ButcherProjectLine::BPL_VERTICAL, width);
    if (height != -1)
        Resize(ButcherProjectLine::BPL_HORIZONTAL, height);
}



bool ButcherProjectArea::IsFirst()
{
    ButcherProjectMaskContainerJoined *cjoin=dynamic_cast<ButcherProjectMaskContainerJoined*>(container_);
    if (!cjoin) return true; // split areas area always the first
    // is first if both left and top lines are the same as the container
    return (cjoin->GetLineLeft()==left_) && (cjoin->GetLineTop()==top_);
}




bool ButcherProjectArea::CanMove(ButcherProjectLine::orientation_t orientation,
    long pos, bool domove)
{
    bool ret=false;
    ButcherProjectLineMinMax
        mms=GetStart(orientation)->GetMoveMinMax(),
        mme=GetEnd(orientation)->GetMoveMinMax();
    long areasize=GetSize(orientation);

    if (pos>=GetStart(orientation)->GetPosition())
    {
        // start can go at maximum where end can go less 1
        if (GetEnd(orientation)->CanMove(pos+areasize) &&
            GetStart(orientation)->IsMoveable() &&
            pos >= mms.min && pos < mme.max)
        {
            ret=true;
            if (domove)
            {
                GetEnd(orientation)->Move(pos+areasize);
                GetStart(orientation)->Move(pos);
            }
        }
    }
    else
    {
        // end can go at maximum where start can go plus 1
        if (GetStart(orientation)->CanMove(pos) &&
            GetEnd(orientation)->IsMoveable() &&
            pos+areasize > mms.min &&
            pos+areasize <= mme.max)
        {
            ret=true;
            if (domove)
            {
                GetStart(orientation)->Move(pos);
                GetEnd(orientation)->Move(pos+areasize);
            }
        }
    }
    return ret;
}




void ButcherProjectArea::Move(ButcherProjectLine::orientation_t orientation, long pos)
{
    CanMove(orientation, pos, true);
}




bool ButcherProjectArea::CanResize(ButcherProjectLine::orientation_t orientation,
    long size, bool doresize)
{
    bool ret=false;
    if (GetEnd(orientation)->CanMove(GetStart(orientation)->GetPosition()+size))
    {
        ret=true;
        if (doresize)
            GetEnd(orientation)->Move(GetStart(orientation)->GetPosition()+size);
    }
    else if (GetStart(orientation)->CanMove(GetEnd(orientation)->GetPosition()-size))
    {
        ret=true;
        if (doresize)
            GetStart(orientation)->Move(GetEnd(orientation)->GetPosition()-size);
    }
    return ret;
}



void ButcherProjectArea::Resize(ButcherProjectLine::orientation_t orientation, long size)
{
    CanResize(orientation, size, true);
}




ButcherProjectMask *ButcherProjectArea::GetMaskRoot()
{
    if (GetMask()->GetParent())
        return GetMask()->GetParent()->GetMaskRoot();
    return GetMask();
}




ButcherProjectArea *ButcherProjectArea::GetParentRoot()
{
    if (GetMask()->GetParent())
        return GetMask()->GetParent()->GetParentRoot();
    return this;
}




ButcherProjectArea::areaclass_t ButcherProjectArea::GetAreaClass()
{
    return container_->GetAreaClass();
}




ButcherProjectArea::areaclass_t ButcherProjectArea::GetContainedAreaClass()
{
    return container_->GetContainedAreaClass();
}





bool ButcherProjectArea::IsAreaSelect(areaselect_t areas)
{
    return container_->IsAreaSelect(areas);
}




ButcherProjectArea::areaflags_t ButcherProjectArea::GetFlags()
{
    return container_->GetFlags();
}




wxRect ButcherProjectArea::GetGlobalRect()
{
    wxRect rc=GetRect();
    rc.SetX(rc.GetX()+GetMask()->GetDrawStartX());
    rc.SetY(rc.GetY()+GetMask()->GetDrawStartY());
    return rc;
}




wxString ButcherProjectArea::GetDescription()
{
    wxString aread(_("Area"));
    switch (GetAreaClass())
    {
    case AC_GLOBAL:
        aread=_("GArea");
        break;
    case AC_MAP:
        aread=_("MArea");
        break;
    default:
        break;
    }

//#ifndef __WXDEBUG__
    if (!name_.IsEmpty())
        return name_;
    return wxString::Format(wxT("%s %d"), aread.c_str(), GetBLId());
/*
#else
	wxString arealay=wxString::Format(wxT("[%d %d %d %d]"),
		GetLeft()->GetBLId(), GetTop()->GetBLId(),
		GetRight()->GetBLId(), GetBottom()->GetBLId());

	if (!name_.IsEmpty())
        return name_+wxT(" ")+arealay;
    return wxString::Format(wxT("%s %d %s"), aread.c_str(), GetBLId(), arealay.c_str());
#endif
*/
}



wxString ButcherProjectArea::GetFullDescription()
{
    wxString temp=GetDescription();
    ButcherProjectMask *ms;
    ms=GetMask();
    while (ms->GetParent())
    {
        temp=ms->GetParent()->GetDescription()+wxT("->")+temp;
        ms=ms->GetParent()->GetMask();
    };
    return temp;
}




wxString ButcherProjectArea::GetBaseFilename(int alternatefile)
{
    wxString aread(wxT("area"));
    switch (GetAreaClass())
    {
    case AC_GLOBAL:
        aread=wxT("garea");
        break;
    case AC_MAP:
        aread=wxT("marea");
        break;
    default:
        break;
    }
    aread=wxString::Format(wxT("%s%d"), aread.c_str(), GetBLId());
    if (alternatefile!=-1)
        aread+=wxString::Format(wxT("alt%d"), alternatefile+1);
    return aread;
}




wxString ButcherProjectArea::GetImageFilename(BLID_t viewid, bool fileext, int alternatefile)
{


	if (alternatefile==-1) alternatefile=Configs().Get(GetProject()->Views().Get(viewid)->GetBLId())->GetAlternateFile();

    wxString ret(wxEmptyString);
    wxString curfn=wxEmptyString;
    bool curfnp=true, addunder=false;
    if (viewid>0)
    {
        curfn=Configs().Get(viewid)->GetValidImageInfo(alternatefile).GetFilename();
        curfnp=Configs().Get(viewid)->GetValidImageInfo(alternatefile).GetFilenamePrepend();
    }

    if (curfn.IsEmpty() || curfnp)
    {
        if (GetMask() && GetMask()->GetParent())
        {
            ret+=GetMask()->GetParent()->GetImageFilename(viewid, false);
            addunder=true;
        }
        else
        {
            if (viewid>0)
            {
                ret+=GetProject()->Views().Get(viewid)->GetBaseFilename();
                addunder=true;
            }
        }
    }
    if (!curfn.IsEmpty() && (curfn==wxT("!")) && !fileext)
        ; // if ! don't add a name for this area, if it is not the last one
    else
    {
        if (addunder) ret+=wxT("_");
        if (!curfn.IsEmpty())
            ret+=curfn;
        else
            ret+=GetBaseFilename(alternatefile);
    }
    if (viewid>0 && fileext)
    {
        ret+=wxT(".");
        ret+=ButcherImage::GetFormatExt(Configs().Get(viewid)->GetValidImageInfo(alternatefile).GetImageFormat()->GetFormat());
    }

    return ret;
}




bool ButcherProjectArea::Customizable(BLID_t viewid)
{
	// global areas are always customizable
	if (GetAreaClass()==ButcherProjectArea::AC_GLOBAL)
		return true;

	// is customizable if not inside another currently customized area
    ButcherProjectMask *ms;
    ms=GetMask();
    while (ms->GetParent())
    {
		if (ms->GetParent()->Configs().Exists(viewid)) return false;
        ms=ms->GetParent()->GetMask();
    };
	return true;
}

bool ButcherProjectArea::Editable(BLID_t viewid)
{
	// global areas are only editable if customized
	if (GetAreaClass()==ButcherProjectArea::AC_GLOBAL)
		return Configs().Exists(viewid);

	return true;
}



ButcherProjectColorScheme::coloritem_t ButcherProjectArea::GetDefaultColorItem()
{
    switch (GetAreaClass())
    {
    case AC_GLOBAL:
        return ButcherProjectColorScheme::BCOLOR_AREAGLOBALBORDER;
        break;
    case AC_MAP:
        return ButcherProjectColorScheme::BCOLOR_AREAMAPBORDER;
        break;
    default:
        if (GetMask()->GetParent())
            return GetMask()->GetParent()->GetDefaultColorItem();
        return ButcherProjectColorScheme::BCOLOR_AREABORDER;
        break;
    }
}




bool ButcherProjectArea::Select(long x, long y)
{
	return GetRect().Contains(x, y);
}




bool ButcherProjectArea::Select(wxPoint pos)
{
    return Select(pos.x, pos.y);
}




wxRect ButcherProjectArea::GetRect() {
    return wxRect(wxPoint(GetLeft()->GetPosition(), GetTop()->GetPosition()),
        wxPoint(GetRight()->GetPosition()-1, GetBottom()->GetPosition()-1));
}





ButcherProjectLine *ButcherProjectArea::GetStart(ButcherProjectLine::orientation_t orientation)
{
    if (orientation == ButcherProjectLine::BPL_HORIZONTAL)
        return top_;
    else
        return left_;
}




ButcherProjectLine *ButcherProjectArea::GetEnd(ButcherProjectLine::orientation_t orientation)
{
    if (orientation == ButcherProjectLine::BPL_HORIZONTAL)
        return bottom_;
    else
        return right_;
}




long ButcherProjectArea::GetStartMinMaxPos(ButcherProjectLine::orientation_t orientation)
{
    //long ret=GetStart(orientation)->GetPosition()+GetMask()->GetDrawStart(orientation), newr;
    long ret=GetStart(orientation)->GetPosition(), newr;

    // locate in inner masks
    if (Configs().Exists(ButcherProjectAreaConfigs::DEFAULT_CONFIG)) // && Configs().Get(*i)->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK)
    {
        newr=GetStart(orientation)->GetPosition()+
            Configs().Get(ButcherProjectAreaConfigs::DEFAULT_CONFIG)->GetInnerMask()->GetStartMinMaxPos(orientation);
        if (newr>ret)
            ret=newr;
    }
	for (ButcherProjectViews::iterator i=GetProject()->Views().begin(); i!=GetProject()->Views().end(); i++)
	{
        if (Configs().Exists(i->GetBLId())) // && Configs().Get(*i)->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK)
        {
            newr=GetStart(orientation)->GetPosition()+
                Configs().Get(i->GetBLId())->GetInnerMask()->GetStartMinMaxPos(orientation);
            if (newr>ret)
                ret=newr;
        }
	}
    return ret;
}



long ButcherProjectArea::GetEndMinMaxPos(ButcherProjectLine::orientation_t orientation)
{
    long ret=GetEnd(orientation)->GetPosition(), newr;

    // locate in inner masks
    if (Configs().Exists(ButcherProjectAreaConfigs::DEFAULT_CONFIG)) // && Configs().Get(*i)->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK)
    {
        newr=GetEnd(orientation)->GetPosition()+
            Configs().Get(ButcherProjectAreaConfigs::DEFAULT_CONFIG)->GetInnerMask()->GetEndMinMaxPos(orientation);
        if (newr<ret)
            ret=newr;
    }
	for (ButcherProjectViews::iterator i=GetProject()->Views().begin(); i!=GetProject()->Views().end(); i++)
	{
        if (Configs().Exists(i->GetBLId())) // && Configs().Get(*i)->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK)
        {
            newr=GetEnd(orientation)->GetPosition()+
                Configs().Get(i->GetBLId())->GetInnerMask()->GetEndMinMaxPos(orientation);
            if (newr<ret)
                ret=newr;
        }
	}

    return ret;
}




void ButcherProjectArea::SetStart(ButcherProjectLine *line)
{
    if (line->GetOrientation() == ButcherProjectLine::BPL_HORIZONTAL)
        top_=line;
    else
        left_=line;
    ProjectModified();
}




void ButcherProjectArea::SetEnd(ButcherProjectLine *line)
{
    if (line->GetOrientation() == ButcherProjectLine::BPL_HORIZONTAL)
        bottom_=line;
    else
        right_=line;
    ProjectModified();
}




void ButcherProjectArea::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    container_->ProjectModified();
}



ButcherProjectLine *ButcherProjectArea::GetLine(unsigned short l) {
    switch (l) {
    case 0: return left_;
    case 1: return top_;
    case 2: return right_;
    case 3: return bottom_;
    default:
        throw ButcherException(_("Invalid line"));
    }
    return NULL;
}




bool ButcherProjectArea::Crossable(ButcherProjectLine::orientation_t orientation, long position)
{
/*

*/

	return position >= GetStartMinMaxPos(orientation) &&
        position <= GetEndMinMaxPos(orientation);
}




void ButcherProjectArea::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    // id is loaded by caller

    if ((!metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING)) ||
        (!metadata->Exists(BFILE_MDI_CONFIGS, ButcherMetadataDataItem::DT_METADATA)))
        throw ButcherException(_("Invalid metadata for area"));

    //SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    name_=metadata->Get(BFILE_MDI_NAME).GetString();

    if (metadata->Get(BFILE_MDI_CONFIGS).GetMetadata().Exists(BFILE_MD_CONFIG))
        configs_->LoadMetadata(&metadata->Get(BFILE_MDI_CONFIGS).GetMetadata().Get(BFILE_MD_CONFIG));
}




void ButcherProjectArea::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        //MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_))->
        MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_CONFIGS));

    configs_->SaveMetadata(&metadata->Get(BFILE_MDI_CONFIGS).GetMetadata().Get(BFILE_MD_CONFIG, true));
}




void ButcherProjectArea::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseBLId::ProjectEvent(event);
    if (configs_)
        configs_->ProjectEvent(event);
}


