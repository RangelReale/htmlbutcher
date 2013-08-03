//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerJoined.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskContainerJoined.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectLineStart.h"
#include "ButcherProjectLineEnd.h"
#include "ButcherProjectLineDefault.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherFileDefs.h"




/////////////////////////////////
// CLASS
//      ButcherProjectMaskContainerJoined
/////////////////////////////////
ButcherProjectMaskContainerJoined::ButcherProjectMaskContainerJoined(ButcherProjectMask *mask,
    ButcherProjectArea::areaclass_t areaclass) :
    ButcherProjectMaskContainer(mask, areaclass),
    marginleft_(0), margintop_(0), marginright_(0), marginbottom_(0)
{
    ButcherProjectBaseAutoDisable autodis(this);

    Initialize();

/*
    line_left_   = lines_.Get(lines_.Add(new ButcherProjectLineStart(this, NULL, NULL, ButcherProjectLine::BPL_VERTICAL)));
    line_top_    = lines_.Get(lines_.Add(new ButcherProjectLineStart(this, NULL, NULL, ButcherProjectLine::BPL_HORIZONTAL)));
    line_right_  = lines_.Get(lines_.Add(new ButcherProjectLineEnd(this, NULL, NULL, ButcherProjectLine::BPL_VERTICAL)));
    line_bottom_ = lines_.Get(lines_.Add(new ButcherProjectLineEnd(this, NULL, NULL, ButcherProjectLine::BPL_HORIZONTAL)));

    line_left_->start_ = line_right_->start_ = line_top_;
    line_left_->end_ = line_right_->end_ = line_bottom_;

    line_top_->start_ = line_bottom_->start_ = line_left_;
    line_top_->end_ = line_bottom_->end_ = line_right_;

    ButcherProjectArea *area=new ButcherProjectArea(this, line_left_, line_top_, line_right_, line_bottom_);

    line_left_->areas_after_.insert(area);
    line_top_->areas_after_.insert(area);
    line_right_->areas_before_.insert(area);
    line_bottom_->areas_before_.insert(area);

    areas_.Add(area);
*/
}




ButcherProjectMaskContainerJoined::~ButcherProjectMaskContainerJoined()
{

}



void ButcherProjectMaskContainerJoined::Initialize()
{
    lines_.op_clear();
    areas_.op_clear();

    line_left_   = lines_.Get(lines_.Add(new ButcherProjectLineStart(this, NULL, NULL, ButcherProjectLine::BPL_VERTICAL)));
    line_top_    = lines_.Get(lines_.Add(new ButcherProjectLineStart(this, NULL, NULL, ButcherProjectLine::BPL_HORIZONTAL)));
    line_right_  = lines_.Get(lines_.Add(new ButcherProjectLineEnd(this, NULL, NULL, ButcherProjectLine::BPL_VERTICAL)));
    line_bottom_ = lines_.Get(lines_.Add(new ButcherProjectLineEnd(this, NULL, NULL, ButcherProjectLine::BPL_HORIZONTAL)));

    line_left_->start_ = line_right_->start_ = line_top_;
    line_left_->end_ = line_right_->end_ = line_bottom_;

    line_top_->start_ = line_bottom_->start_ = line_left_;
    line_top_->end_ = line_bottom_->end_ = line_right_;

    ButcherProjectArea *area=new ButcherProjectArea(this, line_left_, line_top_, line_right_, line_bottom_);

    line_left_->areas_after_.insert(area);
    line_top_->areas_after_.insert(area);
    line_right_->areas_before_.insert(area);
    line_bottom_->areas_before_.insert(area);

    areas_.Add(area);
}




ButcherProjectLine *ButcherProjectMaskContainerJoined::AddLine(ButcherProjectLine *start, ButcherProjectLine *end,
    ButcherProjectLine::orientation_t orientation, unsigned long position,
    BLID_t id)
{


	ButcherProjectLine *newl=new ButcherProjectLineDefault(this, start, end, orientation, position);
    try {
        return lines_.Get(InternalAddLine(newl, id));
    } catch(...) {
        delete newl;
        throw;
    }
}



BLID_t ButcherProjectMaskContainerJoined::InternalAddLine(ButcherProjectLine *line, BLID_t id)
{
    // prevent sending modified event until return
    ButcherProjectBaseAutoUpdate autoupdln(&lines_);
    ButcherProjectBaseAutoUpdate autoupdar(&areas_);
    //ButcherProjectLines::autoupdate_t autoupdln(&lines_);
    //ButcherProjectAreas::autoupdate_t autoupdar(&areas_);

    if (lines_.Count() >= 4) {
        // create new areas
        ButcherProjectModifyAreas mdarea;
        bool check=LineAreas(mdarea, line->GetStart(), line->GetEnd(), line->GetOrientation(),
            line->GetPosition());
        if (!check)
            throw ButcherException(_("Cannot add this line"));

        for (ButcherProjectModifyAreas::iterator i=mdarea.begin(); i!=mdarea.end(); i++) {
            // new area will be the start of old area
            ButcherProjectArea *newarea=new ButcherProjectArea(this,
                line, (*i)->GetStart(line->GetInvOrientation()),
                (*i)->GetEnd(line->GetOrientation()), (*i)->GetEnd(line->GetInvOrientation()));

            // line will be the end of previous area
            (*i)->SetEnd(line);

            areas_.Add(newarea);

            // the new line will be the end of old area, and start of new area
            line->areas_before_.insert(*i);
            line->areas_after_.insert(newarea);

            // add the new area to all it's lines
            newarea->GetStart(line->GetInvOrientation())->areas_after_.insert(newarea);
            newarea->GetEnd(line->GetOrientation())->areas_before_.insert(newarea);
            newarea->GetEnd(line->GetInvOrientation())->areas_before_.insert(newarea);

            // remove old area from end line
            newarea->GetEnd(line->GetOrientation())->areas_before_.erase(*i);
        }
    } else
        id=0;

    return lines_.Add(line, id);
}




BLID_t ButcherProjectMaskContainerJoined::InternalRemoveLine(ButcherProjectLine *line,
    bool doremove)
{
    // prevent sending modified event until return
    ButcherProjectBaseAutoUpdate autoupdln(&lines_);
    ButcherProjectBaseAutoUpdate autoupdar(&areas_);

    BLID_t ret=0;
    if (line->CanEdit()) {
        ButcherProjectLine::ButcherProjectLineAreas::iterator areai;

        ButcherProjectLine::ButcherProjectLineAreas *a[2] = { &line->areas_before_, &line->areas_after_ };

        // for all connected areas, find and remove lines that start/end on this line
        ButcherListIdList rmlist;

        for (int al=0; al<2; al++) {
            if (!a[al]->empty())
                for (areai=a[al]->begin(); areai!=a[al]->end(); areai++)
                {
                    for (int arealn=0; arealn<(*areai)->GetLineCount(); arealn++)
                    {
                        if ((*areai)->GetLine(arealn)->Connected(line))
                            rmlist.insert((*areai)->GetLine(arealn)->GetBLId());
                    }
                }
        }
        ret=static_cast<BLID_t>(rmlist.size());

        if (!rmlist.empty())
            for (ButcherListIdList::const_iterator rmi=rmlist.begin(); rmi!=rmlist.end(); rmi++)
            {
                if (doremove)
                    DeleteLine(lines_.Get(*rmi));
                ret++;
            }

        // merge areas
        if (doremove)
            // join "before" areas with "after" ones
            if (!line->areas_before_.empty())
            {
                ButcherProjectArea* joina;
                for (areai=line->areas_before_.begin(); areai!=line->areas_before_.end(); areai++)
                {
                    joina=line->FindJoinArea(ButcherProjectLine::LAP_AFTER, *areai);
                    if (joina)
                    {
                        // "before" area end will be "after" area end
                        (*areai)->SetEnd(joina->GetEnd(line->GetOrientation()));
                        (*areai)->GetEnd(line->GetOrientation())->areas_before_.erase(joina);
                        (*areai)->GetEnd(line->GetOrientation())->areas_before_.insert(*areai);

                        // remove join area from bordering lines
                        (*areai)->GetStart(line->GetInvOrientation())->areas_after_.erase(joina);
                        (*areai)->GetEnd(line->GetInvOrientation())->areas_before_.erase(joina);

                        areas_.op_delete(joina->GetBLId());
                    } else
                        throw ButcherException(_("Could not find join area"));
                }
            }
    }
    return ret;
}




bool ButcherProjectMaskContainerJoined::CheckConnect(ButcherProjectLine *start, ButcherProjectLine *end,
    ButcherProjectLine::orientation_t orientation, unsigned long position)
{
    if ((start && start->GetOrientation() == orientation) ||
        (end && end->GetOrientation() == orientation))
        return false;
    if (start->GetPosition() == end->GetPosition()) return false;

    if (start->GetPosition() > end->GetPosition()) return false;

    if (!start->Connectable(end, position))
        return false;
    return true;
}




bool ButcherProjectMaskContainerJoined::Check(ButcherProjectLine *start, ButcherProjectLine *end,
    ButcherProjectLine::orientation_t orientation, unsigned long position)
{
    ButcherProjectModifyAreas mdarea;
    return LineAreas(mdarea, start, end, orientation, position);
}




bool ButcherProjectMaskContainerJoined::DeleteLine(ButcherProjectLine *line)
{
    if (line->CanEdit())
    {
        InternalRemoveLine(line);
        lines_.Delete(line);
		return true;
    }
	return false;
}



void ButcherProjectMaskContainerJoined::Clear()
{
    Initialize();

    ProjectModified();
}




// return areas that are cut by this line
bool ButcherProjectMaskContainerJoined::LineAreas(ButcherProjectModifyAreas &areas,
    ButcherProjectLine *start, ButcherProjectLine *end,
    ButcherProjectLine::orientation_t orientation, long position)
{


	if (!ButcherProjectMaskContainerJoined::CheckConnect(start, end, orientation, position))
        return false;



    bool ret=false;
    ButcherProjectLine *ln=start;
    ButcherProjectArea *area;
    while ((area=ln->FindArea(ButcherProjectLine::LAP_AFTER, position))) {
        // check line conflict
        if (area->GetStart(orientation)->GetPosition() == position ||
            area->GetEnd(orientation)->GetPosition() == position) {
            return false;
        }
        // check area crossable (if have a mask, maybe is not)
        if (!area->Crossable(orientation, position)) {
            return false;
        }

        ret=true;
        areas.push_back(area);
        ln=area->GetEnd(ButcherProjectLine::InvOrientation(orientation));
        if (ln==end) break;
    };
    return ret;
}






ButcherProjectLine *ButcherProjectMaskContainerJoined::GetStart(ButcherProjectLine::orientation_t orientation)
{
    if (orientation == ButcherProjectLine::BPL_HORIZONTAL)
        return line_top_;
    return line_left_;
}





ButcherProjectLine *ButcherProjectMaskContainerJoined::GetEnd(ButcherProjectLine::orientation_t orientation)
{
    if (orientation == ButcherProjectLine::BPL_HORIZONTAL)
        return line_bottom_;
    return line_right_;
}



long ButcherProjectMaskContainerJoined::GetStartMinMaxPos(ButcherProjectLine::orientation_t orientation)
{
    long ret=GetStart(orientation)->GetPosition(), newr;

    // returns the rightmost/bottommost start line of all areas
	for (ButcherProjectAreas::iterator i=areas_.begin(); i!=areas_.end(); i++)
	{
        newr=i->GetStartMinMaxPos(orientation);
        if (newr>ret)
            ret=newr;
	}

    return ret;
}




long ButcherProjectMaskContainerJoined::GetEndMinMaxPos(ButcherProjectLine::orientation_t orientation)
{
    long ret=GetEnd(orientation)->GetPosition(), newr;

    // returns the leftmost/topmost end line of all areas
	for (ButcherProjectAreas::iterator i=areas_.begin(); i!=areas_.end(); i++)
	{
        newr=i->GetEndMinMaxPos(orientation);
        if (newr<ret)
            ret=newr;
    }

    return ret;
}




ButcherProjectArea *ButcherProjectMaskContainerJoined::FirstArea()
{
    // find an area that borders both left and top lines
    ButcherProjectLine::ButcherProjectLineAreas::const_iterator ileft, itop;
    for (ileft=line_left_->areas_after_.begin(); ileft!=line_left_->areas_after_.end(); ileft++)
    {
        for (itop=line_top_->areas_after_.begin(); itop!=line_top_->areas_after_.end(); itop++)
        {
            if (*ileft == *itop)
                return *ileft;
        }
    }
    return NULL;
}



void ButcherProjectMaskContainerJoined::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    ButcherProjectMaskContainer::LoadMetadata(metadata);

    if (/*(!metadata->Exists(BFILE_MDI_LEFT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_TOP, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_RIGHT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_BOTTOM, ButcherMetadataDataItem::DT_INTEGER)) ||*/
        (!metadata->Exists(BFILE_MDI_MARGINLEFT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_MARGINTOP, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_MARGINRIGHT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_MARGINBOTTOM, ButcherMetadataDataItem::DT_INTEGER)))
        //(!metadata->Exists(BFILE_MDI_LINES, ButcherMetadataDataItem::DT_METADATA)))
        //(!metadata->Exists(BFILE_MDI_AREAS, ButcherMetadataDataItem::DT_METADATA)))
        throw ButcherException(_("Invalid metadata for container"));

    marginleft_=metadata->Get(BFILE_MDI_MARGINLEFT).GetInteger();
    margintop_=metadata->Get(BFILE_MDI_MARGINTOP).GetInteger();
    marginright_=metadata->Get(BFILE_MDI_MARGINRIGHT).GetInteger();
    marginbottom_=metadata->Get(BFILE_MDI_MARGINBOTTOM).GetInteger();

    if (metadata->Exists(BFILE_MDI_LINES) &&
        metadata->Get(BFILE_MDI_LINES).GetMetadata().Exists(BFILE_MD_LINE))
        LoadMetadataLines(&metadata->Get(BFILE_MDI_LINES).GetMetadata().Get(BFILE_MD_LINE));
    if (metadata->Exists(BFILE_MDI_AREAS) &&
        metadata->Get(BFILE_MDI_AREAS).GetMetadata().Exists(BFILE_MD_AREA))
        LoadMetadataAreas(&metadata->Get(BFILE_MDI_AREAS).GetMetadata().Get(BFILE_MD_AREA));
}




void ButcherProjectMaskContainerJoined::SaveMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectMaskContainer::SaveMetadata(metadata);

    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MARGINLEFT, marginleft_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MARGINTOP, margintop_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MARGINRIGHT, marginright_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MARGINBOTTOM, marginbottom_));

    SaveMetadataLines(&metadata->MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_LINES))->Get(BFILE_MDI_LINES).GetMetadata().Get(BFILE_MD_LINE, true));
    SaveMetadataAreas(&metadata->MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_AREAS))->Get(BFILE_MDI_AREAS).GetMetadata().Get(BFILE_MD_AREA, true));
}




void ButcherProjectMaskContainerJoined::LoadMetadataLines(ButcherMetadataDataList *metadatalist)
{
    ButcherMetadataData *metadata;
    for (unsigned long i=0; i<metadatalist->Count(); i++)
    {
        metadata=&metadatalist->Get(i);

        if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
            (!metadata->Exists(BFILE_MDI_START, ButcherMetadataDataItem::DT_INTEGER)) ||
            (!metadata->Exists(BFILE_MDI_END, ButcherMetadataDataItem::DT_INTEGER)) ||
            (!metadata->Exists(BFILE_MDI_ORIENTATION, ButcherMetadataDataItem::DT_INTEGER)) ||
            (!metadata->Exists(BFILE_MDI_POSITION, ButcherMetadataDataItem::DT_INTEGER)))
            throw ButcherException(_("Invalid metadata for line"));

        if (metadata->Get(BFILE_MDI_ID).GetInteger()>4)
        {
            if ((!lines_.Exists(metadata->Get(BFILE_MDI_START).GetInteger())) ||
                (!lines_.Exists(metadata->Get(BFILE_MDI_END).GetInteger())))
                throw ButcherException(_("Could not find start and/or end line"));

            AddLine(
                lines_.Get(metadata->Get(BFILE_MDI_START).GetInteger()),
                lines_.Get(metadata->Get(BFILE_MDI_END).GetInteger()),
                static_cast<ButcherProjectLine::orientation_t>(metadata->Get(BFILE_MDI_ORIENTATION).GetInteger()),
                metadata->Get(BFILE_MDI_POSITION).GetInteger(),
                metadata->Get(BFILE_MDI_ID).GetInteger());
        }
    }
}



void ButcherProjectMaskContainerJoined::LoadMetadataAreas(ButcherMetadataDataList *metadatalist)
{
    ButcherMetadataData *metadata;
    ButcherProjectArea *area;
    for (unsigned long i=0; i<metadatalist->Count(); i++)
    {
        metadata=&metadatalist->Get(i);

        if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
            (!metadata->Exists(BFILE_MDI_LEFT, ButcherMetadataDataItem::DT_INTEGER)) ||
            (!metadata->Exists(BFILE_MDI_TOP, ButcherMetadataDataItem::DT_INTEGER)) ||
            (!metadata->Exists(BFILE_MDI_RIGHT, ButcherMetadataDataItem::DT_INTEGER)) ||
            (!metadata->Exists(BFILE_MDI_BOTTOM, ButcherMetadataDataItem::DT_INTEGER)))
            throw ButcherException(_("Invalid metadata for area"));

        // locate area based onlines
        area=FindAreaByLines(
            metadata->Get(BFILE_MDI_LEFT).GetInteger(),
            metadata->Get(BFILE_MDI_TOP).GetInteger(),
            metadata->Get(BFILE_MDI_RIGHT).GetInteger(),
            metadata->Get(BFILE_MDI_BOTTOM).GetInteger());
        if (area)
        {
            // move item to the real position
            areas_.op_move(area->GetBLId(), metadata->Get(BFILE_MDI_ID).GetInteger());
            area->LoadMetadata(metadata);
        }
        else
            throw ButcherException(wxString::Format(_("Could not find area %d (%d-%d-%d-%d)"),
                metadata->Get(BFILE_MDI_ID).GetInteger(),
                metadata->Get(BFILE_MDI_LEFT).GetInteger(),
                metadata->Get(BFILE_MDI_TOP).GetInteger(),
                metadata->Get(BFILE_MDI_RIGHT).GetInteger(),
                metadata->Get(BFILE_MDI_BOTTOM).GetInteger()));
    }
}



void ButcherProjectMaskContainerJoined::SaveMetadataLines(ButcherMetadataDataList *metadatalist)
{
    ButcherMetadataData *metadata;

	for (ButcherProjectLines::iterator i=lines_.begin(); i!=lines_.end(); i++)
	{
        if (i->IsSave())
        {
            metadata=metadatalist->Add();

            metadata->
                MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, i->GetBLId()))->
                MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_START, i->GetStart()->GetBLId()))->
                MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_END, i->GetEnd()->GetBLId()))->
                MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ORIENTATION, i->GetOrientation()))->
                MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_POSITION, i->GetPosition()));
        }
	}
}




void ButcherProjectMaskContainerJoined::SaveMetadataAreas(ButcherMetadataDataList *metadatalist)
{
    ButcherMetadataData *metadata;

	for (ButcherProjectAreas::iterator i=areas_.begin(); i!=areas_.end(); i++)
	{
        metadata=metadatalist->Add();

        metadata->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, i->GetBLId()))->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_LEFT, i->GetLeft()->GetBLId()))->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_TOP, i->GetTop()->GetBLId()))->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_RIGHT, i->GetRight()->GetBLId()))->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_BOTTOM, i->GetBottom()->GetBLId()));
        i->SaveMetadata(metadata);
	}
}


