//=============================================================================
/**
 *  @file   ButcherProjectAssortedFiles.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAssortedFiles.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAssortedFiles
/////////////////////////////////
ButcherProjectAssortedFiles::ButcherProjectAssortedFiles(ButcherProject *project) :
    ButcherList<ButcherProjectAssortedFile>(), ButcherProjectBaseModify(project)
{

}



BLID_t ButcherProjectAssortedFiles::Add(const wxString &filename)
{
    return op_add(new ButcherProjectAssortedFile(GetProject(), filename));
}




void ButcherProjectAssortedFiles::Edit(BLID_t id, const wxString &filename)
{
    Get(id)->SetFilename(filename);
    op_edit(id);
}




void ButcherProjectAssortedFiles::SaveFiles(const wxString &path, const ButcherProjectBaseIDList &groups)
{
	for (iterator i=begin(); i!=end(); i++)
	{
        if (i->Groups().Exists(groups))
        {
            i->SaveFileToPath(path);
        }
	}
}




bool ButcherProjectAssortedFiles::can_delete(BLID_t id, ButcherProjectAssortedFile* item)
{
    // remove dependant items
    //GetProject()->Views().remove_file(id);
    return true;
}



void ButcherProjectAssortedFiles::LoadMetadata(ButcherMetadataDataList *metadatalist)
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




void ButcherProjectAssortedFiles::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}




void ButcherProjectAssortedFiles::UpdateMetadata(ButcherMetadataDataList *metadatalist)
{
    if (metadatalist->Count()>0)
    {
        for (unsigned long i=0; i<metadatalist->Count(); i++)
        {
            Get(metadatalist->Get(i).Get(BFILE_MDI_ID).GetInteger())->
                UpdateMetadata(&metadatalist->Get(i));
        }
    }
}



BLID_t ButcherProjectAssortedFiles::Add(ButcherMetadataData *metadata)
{
    ButcherProjectAssortedFile *mdadd=new ButcherProjectAssortedFile(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}


