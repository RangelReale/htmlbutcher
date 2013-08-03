//=============================================================================
/**
 *  @file   ButcherProjectViews.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTVIEWS_H__
#define __BPROJECT_BUTCHERPROJECTVIEWS_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectView.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"

class ButcherProject;

/**
 * @class ButcherProjectViews
 *
 * @brief Project views
 */
class ButcherProjectViews : public ButcherList_1<ButcherProjectView>, public ButcherProjectBaseModify
{
public:
    ButcherProjectViews(ButcherProject *project);
    BLID_t Add(const wxString &name, BLID_t fileid, BLID_t maskid);
/*
    void Edit(BLID_t id, const wxString &name, BLID_t fileid, BLID_t maskid,
        BLID_t maskconfigid,
        int items = ButcherProjectView::ITEM_HTML|ButcherProjectView::ITEM_IMAGES|ButcherProjectView::ITEM_IMAGESGLOBAL);
*/

    bool Delete(BLID_t id) { return op_delete(id); }
    void Clear() { op_clear(); }

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);

    virtual void ProjectEvent(ButcherProjectEventNotify &event);
protected:
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_VIEWMODIFIED; }

    virtual void do_modified(BLID_t id = 0) { ProjectModified(id); }
    virtual void do_deleting(BLID_t id, ButcherProjectView* item) { DoProjectModified(ButcherProjectEvent::BPE_VIEWDELETED, id); }
private:
    unsigned int remove_file(BLID_t fileid, bool doremove = true);
    unsigned int remove_mask(BLID_t maskid, bool doremove = true);

    BLID_t Add(ButcherMetadataData *metadata);
};

#endif // __BPROJECT_BUTCHERPROJECTVIEWS_H__
