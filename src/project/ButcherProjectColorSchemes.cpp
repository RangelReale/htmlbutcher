//=============================================================================
/**
 *  @file   ButcherProjectColorSchemes.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectColorSchemes.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectColorSchemes
/////////////////////////////////
ButcherProjectColorSchemes::ButcherProjectColorSchemes(ButcherProject *project) :
    ButcherList<ButcherProjectColorScheme>(), ButcherProjectBaseModify(project), current_(0)
{
    ButcherProjectBaseAutoDisable autodis(this);

    LoadDefault();
}




BLID_t ButcherProjectColorSchemes::Add(const wxString &name, BLID_t id)
{
    return op_add(new ButcherProjectColorScheme(GetProject(), name), id);
}




void ButcherProjectColorSchemes::Edit(BLID_t id, const wxString &name)
{
    Get(id)->SetName(name);
    op_edit(id);
}




void ButcherProjectColorSchemes::do_deleting(BLID_t id, ButcherProjectColorScheme* item)
{
    if (current_==id) current_=DefaultId();
    DoProjectModified(ButcherProjectEvent::BPE_COLORSCHEMEDELETED, id);
}




void ButcherProjectColorSchemes::do_modified(BLID_t id)
{
    ProjectModified(id);
}




void ButcherProjectColorSchemes::SetCurrentId(BLID_t cur)
{
    if (!Exists(cur)) throw ButcherException(_("Color scheme does not exists"));

    current_=cur;
    ProjectModified();
}



void ButcherProjectColorSchemes::LoadDefault()
{
    Clear();
    Add(_("DEFAULT"), 1);
    current_=1;
}




void ButcherProjectColorSchemes::LoadMetadata(ButcherMetadataDataList *metadatalist)
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
    if (Count()==0)
        LoadDefault();
}




void ButcherProjectColorSchemes::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}




BLID_t ButcherProjectColorSchemes::Add(ButcherMetadataData *metadata)
{
    ButcherProjectColorScheme *mdadd=new ButcherProjectColorScheme(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}


