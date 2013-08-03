//=============================================================================
/**
 *  @file   ButcherProjectAssortedFileGroups.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAssortedFileGroups.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAssortedFileGroups
/////////////////////////////////
ButcherProjectAssortedFileGroups::ButcherProjectAssortedFileGroups(ButcherProject *project) :
    ButcherList<ButcherProjectAssortedFileGroup>(), ButcherProjectBaseModify(project)
{

}




BLID_t ButcherProjectAssortedFileGroups::Add(const wxString &name)
{
    return op_add(new ButcherProjectAssortedFileGroup(GetProject(), name));
}




void ButcherProjectAssortedFileGroups::Edit(BLID_t id, const wxString &name)
{
    Get(id)->SetName(name);
    op_edit(id);
}





bool ButcherProjectAssortedFileGroups::can_delete(BLID_t id, ButcherProjectAssortedFileGroup* item)
{
    // remove dependant items
    //GetProject()->Views().remove_file(id);
    return true;
}



void ButcherProjectAssortedFileGroups::LoadMetadata(ButcherMetadataDataList *metadatalist)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if (metadatalist->Count()>0)
    {
        Clear();

        for (unsigned long i=0; i<metadatalist->Count(); i++)
        {
            Add(&metadatalist->Get(i));
        }
    }
}




void ButcherProjectAssortedFileGroups::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}




BLID_t ButcherProjectAssortedFileGroups::Add(ButcherMetadataData *metadata)
{
    ButcherProjectAssortedFileGroup *mdadd=new ButcherProjectAssortedFileGroup(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}



