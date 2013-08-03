//=============================================================================
/**
 *  @file   ButcherProjectOptions.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectOptions.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherFileDefs.h"



/////////////////////////////////
// CLASS
//      ButcherProjectOptions
/////////////////////////////////

ButcherProjectOptions::ButcherProjectOptions(ButcherProject *project) :
	ButcherProjectBaseModify(project)
{
	do_reset();
}

void ButcherProjectOptions::Reset()
{
    ButcherProjectBaseAutoUpdate autoupd(this);

	do_reset();
}

void ButcherProjectOptions::do_reset()
{
	layouttype_=ButcherProjectConsts::LTYPE_TABLE;
}

void ButcherProjectOptions::LoadMetadata(ButcherMetadataDataList *metadatalist)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    Reset();

    if (metadatalist->Count()>0)
    {
		ButcherMetadataData *metadata = &metadatalist->Get(0);

		if (metadata->Exists(BFILE_MDI_LAYOUTTYPE, ButcherMetadataDataItem::DT_INTEGER))
			layouttype_=static_cast<ButcherProjectConsts::layouttype_t>(metadata->Get(BFILE_MDI_LAYOUTTYPE).GetInteger());
    }
}

void ButcherProjectOptions::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	ButcherMetadataData *metadata=metadatalist->Add();

    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_LAYOUTTYPE, layouttype_));
}
