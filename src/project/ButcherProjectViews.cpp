//=============================================================================
/**
 *  @file   ButcherProjectViews.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectViews.h"
#include "ButcherProjectBaseAutoUpdate.h"



/////////////////////////////////
// CLASS
//      ButcherProjectViews
/////////////////////////////////
ButcherProjectViews::ButcherProjectViews(ButcherProject *project) :
    ButcherList_1<ButcherProjectView>(), ButcherProjectBaseModify(project)
{

}



BLID_t ButcherProjectViews::Add(const wxString &name, BLID_t fileid, BLID_t maskid)
{


	if (name.IsEmpty())
        throw ButcherException(_("View name cannot be blank"));

    return op_add(new ButcherProjectView(GetProject(), name, fileid, maskid));
}




/*
void ButcherProjectViews::Edit(BLID_t id, const wxString &name, BLID_t fileid, BLID_t maskid,
    BLID_t maskconfigid, int items)
{
    Get(id)->Set(name, fileid, maskid, maskconfigid, items);
    op_edit(id);
}
*/

unsigned int ButcherProjectViews::remove_file(BLID_t fileid, bool doremove)
{
    unsigned int ret = 0;

    ButcherProjectBaseAutoUpdate upd(this);

    //BeginUpdate();

	for (iterator i=begin(); i!=end(); i++)
	{
        if (i->GetFileId() == fileid) {
            ret++;
            if (doremove)
                Delete(i.first());
        }
	}

/*
    for (ButcherListIdList::const_iterator i=ids.begin(); i!=ids.end(); i++) {
        if (Get(*i)->GetFileId() == fileid) {
            ret++;
            if (doremove)
                Delete(*i);
        }
    }
*/

    //EndUpdate();

    return ret;
}




unsigned int ButcherProjectViews::remove_mask(BLID_t maskid, bool doremove)
{
    unsigned int ret = 0;

    ButcherProjectBaseAutoUpdate upd(this);

    //BeginUpdate();

	for (iterator i=begin(); i!=end(); i++)
	{
        if (i->GetMaskId() == maskid) {
            ret++;
            if (doremove)
                Delete(i.first());
        }
	}

/*
	for (ButcherListIdList::const_iterator i=ids.begin(); i!=ids.end(); i++) {
        if (Get(*i)->GetMaskId() == maskid) {
            ret++;
            if (doremove)
                Delete(*i);
        }
    }
*/

    //EndUpdate();

    return ret;
}




void ButcherProjectViews::LoadMetadata(ButcherMetadataDataList *metadatalist)
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



void ButcherProjectViews::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}




BLID_t ButcherProjectViews::Add(ButcherMetadataData *metadata)
{
    ButcherProjectView *mdadd=new ButcherProjectView(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}




void ButcherProjectViews::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseModify::ProjectEvent(event);
    // processevent
    if (event.GetProjectEvent().GetEId()>0)
        switch (event.GetProjectEvent().GetEvent())
        {
        case ButcherProjectEvent::BPE_FILEDELETED:
            event.AddRemove(remove_file(event.GetProjectEvent().GetEId(), event.GetIsExecute()));
            break;
        case ButcherProjectEvent::BPE_MASKDELETED:
            event.AddRemove(remove_mask(event.GetProjectEvent().GetEId(), event.GetIsExecute()));
            break;
        default:
            break;
        }

    // forward event
	for (iterator i=begin(); i!=end(); i++)
        i->ProjectEvent(event);
}


