//=============================================================================
/**
 *  @file   ButcherProjectCSSFiles.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectCSSFiles.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectCSSFiles
/////////////////////////////////
ButcherProjectCSSFiles::ButcherProjectCSSFiles(ButcherProject *project) :
    ButcherList<ButcherProjectCSSFile>(), ButcherProjectBaseModify(project)
{

}




BLID_t ButcherProjectCSSFiles::Add(const wxString &name)
{
    return op_add(new ButcherProjectCSSFile(GetProject(), name));
}




void ButcherProjectCSSFiles::Edit(BLID_t id, const wxString &name)
{
    Get(id)->SetName(name);
    op_edit(id);
}




bool ButcherProjectCSSFiles::can_delete(BLID_t id, ButcherProjectCSSFile* item)
{
    // remove dependant items
    //GetProject()->Views().remove_file(id);
    return true;
}



void ButcherProjectCSSFiles::LoadMetadata(ButcherMetadataDataList *metadatalist)
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




void ButcherProjectCSSFiles::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}




BLID_t ButcherProjectCSSFiles::Add(ButcherMetadataData *metadata)
{
    ButcherProjectCSSFile *mdadd=new ButcherProjectCSSFile(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}



