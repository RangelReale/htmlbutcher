//=============================================================================
/**
 *  @file   ButcherProjectCSSFiles.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTCSSFILES_H__
#define __BPROJECT_BUTCHERPROJECTCSSFILES_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectCSSFile.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"

class ButcherProject;

/**
 * @class ButcherProjectCSSFiles
 *
 * @brief Project CSS files
 */
class ButcherProjectCSSFiles : public ButcherList<ButcherProjectCSSFile>, public ButcherProjectBaseModify
{
public:
    ButcherProjectCSSFiles(ButcherProject *project);
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
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_CSSFILEMODIFIED; }

    virtual bool can_delete(BLID_t id, ButcherProjectCSSFile* item);
    virtual void do_modified(BLID_t id = 0) { ProjectModified(id); }
    virtual void do_deleting(unsigned int id, ButcherProjectCSSFile* item) { DoProjectModified(ButcherProjectEvent::BPE_CSSFILEDELETED, id); }
private:
    BLID_t Add(ButcherMetadataData *metadata);
};

#endif // __BPROJECT_BUTCHERPROJECTCSSFILES_H__
