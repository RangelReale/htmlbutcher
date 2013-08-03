//=============================================================================
/**
 *  @file   ButcherProjectFilePaths.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectFilePaths.h"
#include "ButcherProject.h"
#include "ButcherProjectBaseAutoUpdate.h"



/////////////////////////////////
// CLASS
//      ButcherProjectFilePaths
/////////////////////////////////
ButcherProjectFilePaths::ButcherProjectFilePaths(ButcherProject *project) :
    ButcherList<ButcherProjectFilePath>(), ButcherProjectBaseModify(project)
{

}



BLID_t ButcherProjectFilePaths::Add(const wxString &name, const wxString &path, BLID_t id)
{
    if (name.IsEmpty())
        throw ButcherException(_("File path name cannot be blank"));

    return op_add(new ButcherProjectFilePath(GetProject(), name, path), id);
}




void ButcherProjectFilePaths::Edit(BLID_t id, const wxString &name, const wxString &path)
{
    Get(id)->Set(name, path);
    op_edit(id);
}




void ButcherProjectFilePaths::LoadDefault()
{
    Clear();
    Add(_("images"), _("images"), 1);
    Add(_("root"), wxEmptyString, 2);
}




wxString ButcherProjectFilePaths::GetFilePath(BLID_t filepathid, const wxString &basepath,
    bool includetrailing, const wxString &defpath)
{
    if (filepathid>0)
    {
        return Get(filepathid)->GetFormattedPath(basepath, includetrailing);
    }
    else
        return ButcherProjectFilePath::GetFormattedPath(defpath, basepath, includetrailing);
}



void ButcherProjectFilePaths::LoadMetadata(ButcherMetadataDataList *metadatalist)
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
    if (Count()==0)
        LoadDefault();
}




void ButcherProjectFilePaths::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}




BLID_t ButcherProjectFilePaths::Add(ButcherMetadataData *metadata)
{
    ButcherProjectFilePath *mdadd=new ButcherProjectFilePath(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}


