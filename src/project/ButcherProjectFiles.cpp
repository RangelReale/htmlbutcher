//=============================================================================
/**
 *  @file   ButcherProjectFiles.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectFiles.h"
#include "ButcherProject.h"
#include "ButcherProjectBaseAutoUpdate.h"



/////////////////////////////////
// CLASS
//      ButcherProjectFiles
/////////////////////////////////
ButcherProjectFiles::ButcherProjectFiles(ButcherProject *project) :
    ButcherList_2<ButcherProjectFile>(), ButcherProjectBaseModify(project)
{

}




BLID_t ButcherProjectFiles::Add(const wxString &name, const wxString &filename)
{


	int w, h;
    if (!filename.IsEmpty() && !GetProject()->IsValidImageFile(filename, &w, &h))
        throw ButcherException(_("File is not a recognized image format"));

    return op_add(new ButcherProjectFile(GetProject(), name, filename, w, h));
}

BLID_t ButcherProjectFiles::Add(const wxString &name, wxInputStream &filedata)
{


	int w, h;
    if (!GetProject()->IsValidImageFile(filedata, &w, &h))
        throw ButcherException(_("File is not a recognized image format"));

    return op_add(new ButcherProjectFile(GetProject(), name, filedata, w, h));
}




void ButcherProjectFiles::Edit(BLID_t id, const wxString &name, const wxString &filename)
{
    int w, h;
    if (!GetProject()->IsValidImageFile(filename, &w, &h))
        throw ButcherException(_("File is not a recognized image format"));

    Get(id)->Set(name, filename, w, h);
    op_edit(id);
}



bool ButcherProjectFiles::can_delete(BLID_t id, ButcherProjectFile* item)
{
    // remove dependant items
    //GetProject()->Views().remove_file(id);
    return true;
}




void ButcherProjectFiles::LoadMetadata(ButcherMetadataDataList *metadatalist)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if (metadatalist->Count()>0)
    {
        Clear();

        for (unsigned int i=0; i<metadatalist->Count(); i++)
        {
            Add(&metadatalist->Get(i));
        }
    }
}




void ButcherProjectFiles::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}




void ButcherProjectFiles::UpdateMetadata(ButcherMetadataDataList *metadatalist)
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



BLID_t ButcherProjectFiles::Add(ButcherMetadataData *metadata)
{
    ButcherProjectFile *mdadd=new ButcherProjectFile(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}



