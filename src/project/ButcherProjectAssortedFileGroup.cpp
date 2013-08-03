//=============================================================================
/**
 *  @file   ButcherProjectAssortedFileGroup.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAssortedFileGroup.h"
#include "ButcherProject.h"
#include "ButcherProjectAssortedFileGroups.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAssortedFileGroup
/////////////////////////////////
ButcherProjectAssortedFileGroup::ButcherProjectAssortedFileGroup(ButcherProject *project, const wxString &name) :
    ButcherProjectBaseBLId(project), name_(name)
{

}




ButcherProjectAssortedFileGroup::ButcherProjectAssortedFileGroup(ButcherProject *project) :
    ButcherProjectBaseBLId(project), name_(wxEmptyString)
{

}




ButcherProjectAssortedFileGroup::~ButcherProjectAssortedFileGroup()
{

}




void ButcherProjectAssortedFileGroup::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING)))
        throw ButcherException(_("Invalid metadata for assorted file group"));

    SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    name_=metadata->Get(BFILE_MDI_NAME).GetString();
}



void ButcherProjectAssortedFileGroup::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_));
}


