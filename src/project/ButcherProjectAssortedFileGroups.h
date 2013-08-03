//=============================================================================
/**
 *  @file   ButcherProjectAssortedFileGroups.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTASSORTEDFILEGROUPS_H__
#define __BPROJECT_BUTCHERPROJECTASSORTEDFILEGROUPS_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectAssortedFileGroup.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"
#include "ButcherProjectFile.h"

class ButcherProject;

/**
 * @class ButcherProjectAssortedFileGroups
 *
 * @brief Project assorted file groups
 */
class ButcherProjectAssortedFileGroups : public ButcherList<ButcherProjectAssortedFileGroup>, public ButcherProjectBaseModify
{
public:
    ButcherProjectAssortedFileGroups(ButcherProject *project);
    BLID_t Add(const wxString &name);
    void Edit(BLID_t id, const wxString &name);

    bool Delete(BLID_t id) { return op_delete(id); }
    void Clear() { op_clear(); }

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);

    virtual void ProjectEvent(ButcherProjectEventNotify &event) {
        ButcherProjectBaseModify::ProjectEvent(event);
		for (iterator i=begin(); i!=end(); i++)
			i->ProjectEvent(event);
	}
protected:
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_ASSORTEDFILEGROUPMODIFIED; }

    virtual bool can_delete(BLID_t id, ButcherProjectAssortedFileGroup* item);
    virtual void do_modified(BLID_t id = 0) { ProjectModified(id); }
    virtual void do_deleting(unsigned int id, ButcherProjectFile* item) { DoProjectModified(ButcherProjectEvent::BPE_ASSORTEDFILEGROUPDELETED, id); }
private:
    BLID_t Add(ButcherMetadataData *metadata);
};

#endif // __BPROJECT_BUTCHERPROJECTASSORTEDFILEGROUPS_H__
