//=============================================================================
/**
 *  @file   ButcherProjectMasks.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMasks.h"
#include "ButcherProjectBaseAutoUpdate.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMasks
/////////////////////////////////
ButcherProjectMasks::ButcherProjectMasks(ButcherProject *project) :
    ButcherList_1<ButcherProjectMaskRoot>(), ButcherProjectBaseModify(project)
{

}




BLID_t ButcherProjectMasks::Add(const wxString &name, int width, int height, BLID_t id)
{


	if (name.IsEmpty())
        throw ButcherException(_("Mask name cannot be blank"));

    return op_add(new ButcherProjectMaskRoot(GetProject(), name, width, height), id);
}



void ButcherProjectMasks::Edit(BLID_t id, const wxString &name,
    int width, int height)
{
    Get(id)->Set(name, width, height);
    op_edit(id);
}




BLID_t ButcherProjectMasks::Duplicate(BLID_t dupid, const wxString &newname)
{
    //BLID_t ret=op_add(new ButcherProjectMaskRoot(*Get(dupid)));
    BLID_t ret=op_add(Get(dupid)->clone());
    Get(ret)->Set(newname, Get(dupid)->GetWidth(), Get(dupid)->GetHeight());
    return ret;
}




bool ButcherProjectMasks::can_delete(BLID_t id, ButcherProjectMaskRoot* item)
{
    // remove dependant items
    //GetProject()->Views().remove_mask(id);
    return true;
}





void ButcherProjectMasks::LoadMetadata(ButcherMetadataDataList *metadatalist)
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




void ButcherProjectMasks::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}




BLID_t ButcherProjectMasks::Add(ButcherMetadataData *metadata)
{
    ButcherProjectMaskRoot *mdadd=new ButcherProjectMaskRoot(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}



