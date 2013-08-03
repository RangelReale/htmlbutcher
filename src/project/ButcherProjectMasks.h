//=============================================================================
/**
 *  @file   ButcherProjectMasks.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKS_H__
#define __BPROJECT_BUTCHERPROJECTMASKS_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectMaskRoot.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"

class ButcherProject;

/**
 * @class ButcherProjectMasks
 *
 * @brief Project masks
 */
class ButcherProjectMasks : public ButcherList_1<ButcherProjectMaskRoot>, public ButcherProjectBaseModify
{
public:
    ButcherProjectMasks(ButcherProject *project);
    BLID_t Add(const wxString &name, int width, int height, BLID_t id = 0);
    void Edit(BLID_t id, const wxString &name, int width, int height);
    BLID_t Duplicate(BLID_t dupid, const wxString &newname);

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
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_MASKMODIFIED; }

    virtual bool can_delete(BLID_t id, ButcherProjectMaskRoot* item);
    virtual void do_modified(BLID_t id = 0) { ProjectModified(id); }
    virtual void do_deleting(BLID_t id, ButcherProjectMaskRoot* item) { DoProjectModified(ButcherProjectEvent::BPE_MASKDELETED, id); }
private:
    BLID_t Add(ButcherMetadataData *metadata);
};

#endif // __BPROJECT_BUTCHERPROJECTMASKS_H__
