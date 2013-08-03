//=============================================================================
/**
 *  @file   ButcherProjectColorSchemes.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTCOLORSCHEMES_H__
#define __BPROJECT_BUTCHERPROJECTCOLORSCHEMES_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectEvent.h"
#include "ButcherProjectColorScheme.h"
#include "ButcherProjectEvent.h"
#include "ButcherProjectEventNotify.h"

class ButcherProject;

/**
 * @class ButcherProjectColorSchemes
 *
 * @brief Color scheme list
 */
class ButcherProjectColorSchemes : public ButcherList<ButcherProjectColorScheme>, public ButcherProjectBaseModify
{
public:
    ButcherProjectColorSchemes(ButcherProject *project);

    BLID_t Add(const wxString &name, BLID_t id = 0);
    void Edit(BLID_t id, const wxString &name);
    bool Delete(BLID_t id) { if (id==current_) current_=DefaultId(); return op_delete(id); }
    void Clear() { op_clear(); }

    BLID_t GetCurrentId() { return current_; }
    ButcherProjectColorScheme *Current() { return Get(current_); }
    void SetCurrentId(BLID_t cur);

    void LoadDefault();

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);

    virtual void ProjectEvent(ButcherProjectEventNotify &event) {
        ButcherProjectBaseModify::ProjectEvent(event);

		for (iterator i=begin(); i!=end(); i++)
			i->ProjectEvent(event);
    }
protected:
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_COLORSCHEMEMODIFIED; }

    virtual void do_deleting(BLID_t id, ButcherProjectColorScheme* item);
    virtual void do_modified(BLID_t id = 0);
    virtual bool can_delete(BLID_t id, ButcherProjectColorScheme *item) { return Count()>1; } // need at least 1
private:
    BLID_t Add(ButcherMetadataData *metadata);

    BLID_t current_;
};

#endif // __BPROJECT_BUTCHERPROJECTCOLORSCHEMES_H__