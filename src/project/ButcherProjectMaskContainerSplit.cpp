//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerSplit.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskContainerSplit.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectLineDefault.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherFileDefs.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskContainerSplit
/////////////////////////////////
ButcherProjectMaskContainerSplit::ButcherProjectMaskContainerSplit(ButcherProjectMask *mask,
    ButcherProjectArea::areaclass_t areaclass) :
    ButcherProjectMaskContainer(mask, areaclass)
{

}




ButcherProjectArea *ButcherProjectMaskContainerSplit::AddArea(long left, long top, long right, long bottom, BLID_t id)
{
	// prevent sending modified event until return
    ButcherProjectBaseAutoUpdate autoupdln(&lines_);
    ButcherProjectBaseAutoUpdate autoupdar(&areas_);

    ButcherProjectLine *line_left, *line_top, *line_right, *line_bottom;



    line_left   = lines_.Get(lines_.Add(new ButcherProjectLineDefault(this, NULL, NULL, ButcherProjectLine::BPL_VERTICAL, left)));
    line_top    = lines_.Get(lines_.Add(new ButcherProjectLineDefault(this, NULL, NULL, ButcherProjectLine::BPL_HORIZONTAL, top)));
    line_right  = lines_.Get(lines_.Add(new ButcherProjectLineDefault(this, NULL, NULL, ButcherProjectLine::BPL_VERTICAL, right)));
    line_bottom = lines_.Get(lines_.Add(new ButcherProjectLineDefault(this, NULL, NULL, ButcherProjectLine::BPL_HORIZONTAL, bottom)));

    line_left->start_ = line_right->start_ = line_top;
    line_left->end_ = line_right->end_ = line_bottom;

    line_top->start_ = line_bottom->start_ = line_left;
    line_top->end_ = line_bottom->end_ = line_right;

    ButcherProjectArea *area=new ButcherProjectArea(this, line_left, line_top, line_right, line_bottom);

    line_left->areas_after_.insert(area);
    line_top->areas_after_.insert(area);
    line_right->areas_before_.insert(area);
    line_bottom->areas_before_.insert(area);

    areas_.Add(area, id);

	// by default set area kind to NONE
	area->Configs().Get(ButcherProjectAreaConfigs::DEFAULT_CONFIG)->SetAreaKind(ButcherProjectAreaConfigBase::AK_NONE);

    return area;
}



void ButcherProjectMaskContainerSplit::DeleteArea(ButcherProjectArea *area)
{
    // prevent sending modified event until return
    ButcherProjectBaseAutoUpdate autoupdln(&lines_);
    ButcherProjectBaseAutoUpdate autoupdar(&areas_);

    lines_.Delete(area->GetLeft());
    lines_.Delete(area->GetTop());
    lines_.Delete(area->GetRight());
    lines_.Delete(area->GetBottom());

    areas_.Delete(area);
}




void ButcherProjectMaskContainerSplit::Clear()
{
    lines_.op_clear();
    areas_.op_clear();

    ProjectModified();
}




void ButcherProjectMaskContainerSplit::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    ButcherProjectMaskContainer::LoadMetadata(metadata);

    if (metadata->Exists(BFILE_MDI_AREAS) &&
        metadata->Get(BFILE_MDI_AREAS).GetMetadata().Exists(BFILE_MD_AREA))
        LoadMetadataAreas(&metadata->Get(BFILE_MDI_AREAS).GetMetadata().Get(BFILE_MD_AREA));
}



void ButcherProjectMaskContainerSplit::LoadMetadataAreas(ButcherMetadataDataList *metadatalist)
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

        area=AddArea(
            metadata->Get(BFILE_MDI_LEFT).GetInteger(),
            metadata->Get(BFILE_MDI_TOP).GetInteger(),
            metadata->Get(BFILE_MDI_RIGHT).GetInteger(),
            metadata->Get(BFILE_MDI_BOTTOM).GetInteger(),
            metadata->Get(BFILE_MDI_ID).GetInteger());
        area->LoadMetadata(metadata);
    }
}




void ButcherProjectMaskContainerSplit::SaveMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectMaskContainer::SaveMetadata(metadata);
    SaveMetadataAreas(&metadata->MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_AREAS))->Get(BFILE_MDI_AREAS).GetMetadata().Get(BFILE_MD_AREA, true));
}



void ButcherProjectMaskContainerSplit::SaveMetadataAreas(ButcherMetadataDataList *metadatalist)
{
    ButcherMetadataData *metadata;

	for (ButcherProjectAreas::iterator i=areas_.begin(); i!=areas_.end(); i++)
	{
        metadata=metadatalist->Add();

        metadata->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, i->GetBLId()))->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_LEFT, i->GetLeft()->GetPosition()))->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_TOP, i->GetTop()->GetPosition()))->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_RIGHT, i->GetRight()->GetPosition()))->
            MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_BOTTOM, i->GetBottom()->GetPosition()));
        i->SaveMetadata(metadata);
	}
}


