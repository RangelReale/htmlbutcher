//=============================================================================
/**
 *  @file   ButcherProjectAreaConfigs.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAreaConfigs.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectMetadataFile.h"
#include "ButcherProjectMask.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAreaConfigs
/////////////////////////////////

#ifndef __WXMSW__
// initializer
const BLID_t ButcherProjectAreaConfigs::DEFAULT_CONFIG;
#endif

ButcherProjectAreaConfigs::ButcherProjectAreaConfigs(ButcherProjectArea *area) :
    ButcherProjectBaseModify(area->GetProject()), area_(area), list_()
{
    ButcherProjectBaseAutoDisable autodis(this);

    Add(DEFAULT_CONFIG);
}



ButcherProjectAreaConfig *ButcherProjectAreaConfigs::Get(BLID_t id)
{
    if (list_.find(id)!=list_.end())
        return &*list_[id];
    if (id==DEFAULT_CONFIG) throw ButcherException(_("Default area config not found")); // just in case
    return Get(DEFAULT_CONFIG);
}




void ButcherProjectAreaConfigs::Add(BLID_t id)
{


	if (list_.find(id)==list_.end())
    {
        list_[id]=std::shared_ptr<ButcherProjectAreaConfig>(new ButcherProjectAreaConfig(area_, id));
/*
		// set new configs as kind mask
		if (area_->GetAreaClass()==ButcherProjectArea::AC_DEFAULT && id>DEFAULT_CONFIG)
			Get(id)->SetAreaKind(ButcherProjectAreaConfigBase::AK_MASK);
*/
        ProjectModified();
    }
}




void ButcherProjectAreaConfigs::Delete(BLID_t id)
{
    if (id==DEFAULT_CONFIG) throw ButcherException(_("Default config cannot be deleted"));
    if (list_.find(id)!=list_.end())
    {
        list_.erase(id);
        ProjectModified();
    }
}




bool ButcherProjectAreaConfigs::Exists(BLID_t id)
{
    return list_.find(id)!=list_.end();
}




void ButcherProjectAreaConfigs::Clear()
{
    list_.clear();
    ProjectModified();
}




void ButcherProjectAreaConfigs::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    area_->ProjectModified();
}




void ButcherProjectAreaConfigs::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseModify::ProjectEvent(event);
    // mask config deleted
/*
	if (event.GetProjectEvent().GetEvent()==ButcherProjectEvent::BPE_MASKCONFIGDELETED)
        if (event.GetProjectEvent().GetEId()==area_->GetMaskRoot()->GetBLId()&&Exists(event.GetProjectEvent().GetEId2()))
        {
            event.AddChange();
            if (event.GetIsExecute())
                Delete(event.GetProjectEvent().GetEId2());
        }
*/
    // view deleted
	if (event.GetProjectEvent().GetEvent()==ButcherProjectEvent::BPE_VIEWDELETED)
        if (Exists(event.GetProjectEvent().GetEId()))
        {
            event.AddChange();
            if (event.GetIsExecute())
                Delete(event.GetProjectEvent().GetEId());
        }

	// recurse
    for (list_t::const_iterator i=list_.begin(); i!=list_.end(); i++)
        Get(i->first)->ProjectEvent(event);
}




void ButcherProjectAreaConfigs::LoadMetadata(ButcherMetadataDataList *metadatalist)
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
    // DEFAULT_CONFIG must always exist
    if (!Exists(DEFAULT_CONFIG))
        Add(DEFAULT_CONFIG);
}




void ButcherProjectAreaConfigs::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
    for (list_t::const_iterator i=list_.begin(); i!=list_.end(); i++)
    {
        i->second->SaveMetadata(metadatalist->Add());
    }
    if (!Exists(DEFAULT_CONFIG))
        Add(DEFAULT_CONFIG);
}




BLID_t ButcherProjectAreaConfigs::Add(ButcherMetadataData *metadata)
{
    ButcherProjectAreaConfig *mdadd=new ButcherProjectAreaConfig(area_);
    mdadd->LoadMetadata(metadata);

	// COMPAT BEGIN
	// ButcherFile <2 - area config id was MASKCONFIGID, now it is VIEWID, update it
	if (metadata->GetFile() && dynamic_cast<ButcherProjectMetadataFile*>(metadata->GetFile())->GetBVersion()<2)
	{
		if (mdadd->GetBLId()>1) {
			BLID_t newid=0;
			wxLogDebug(wxT("Area configs = mask %d area %d config %d (%s)"),
				area_->GetMaskRoot()->GetBLId(), area_->GetBLId(), mdadd->GetBLId(),
				area_->GetFullDescription().c_str());


			for (ButcherProjectViews::iterator i=GetProject()->Views().begin(); i!=GetProject()->Views().end(); i++)
			{
				//wxLogDebug(wxT("** View %d mask %d config %d"), *i, GetProject()->Views()[*i]->GetMaskId(), GetProject()->Views()[*i]->GetMaskConfigId());

				if (i->GetMaskId()==area_->GetMaskRoot()->GetBLId() &&
					i->GetCOMPATMaskConfigId()==mdadd->GetBLId())
				{
					wxLogDebug(wxT("-- Area config view [mask %d area %d config %d] = %d"), area_->GetMaskRoot()->GetBLId(), area_->GetBLId(), mdadd->GetBLId(), i.first());
					newid=i.first();
				}
			}

			if (newid!=0)
			{
				mdadd->SetBLId(newid);
			} else {
				delete mdadd;
				return 0;
			}
		} else {
			mdadd->SetBLId(DEFAULT_CONFIG);
		}
	}
	// COMPAT END

    list_[mdadd->GetBLId()]=std::shared_ptr<ButcherProjectAreaConfig>(mdadd);
    return mdadd->GetBLId();
}


