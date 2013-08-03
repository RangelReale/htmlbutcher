//=============================================================================
/**
 *  @file   ButcherProjectAssortedFiles.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTASSORTEDFILES_H__
#define __BPROJECT_BUTCHERPROJECTASSORTEDFILES_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectAssortedFile.h"
#include "ButcherProjectBaseIDList.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"

class ButcherProject;

/**
 * @class ButcherProjectAssortedFiles
 *
 * @brief Project assorted files
 */
class ButcherProjectAssortedFiles : public ButcherList<ButcherProjectAssortedFile>, public ButcherProjectBaseModify
{
public:
    ButcherProjectAssortedFiles(ButcherProject *project);
    BLID_t Add(const wxString &filename);
    void Edit(BLID_t id, const wxString &filename);

    bool Delete(BLID_t id) { return op_delete(id); }
    void Clear() { op_clear(); }

    void SaveFiles(const wxString &path, const ButcherProjectBaseIDList &groups);

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);
    void UpdateMetadata(ButcherMetadataDataList *metadatalist);

    virtual void ProjectEvent(ButcherProjectEventNotify &event) {
        ButcherProjectBaseModify::ProjectEvent(event);
		for (iterator i=begin(); i!=end(); i++)
			i->ProjectEvent(event);
    }
protected:
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_ASSORTEDFILEMODIFIED; }

    virtual bool can_delete(BLID_t id, ButcherProjectAssortedFile* item);
    virtual void do_modified(BLID_t id = 0) { ProjectModified(id); }
    virtual void do_deleting(unsigned int id, ButcherProjectAssortedFile* item) { DoProjectModified(ButcherProjectEvent::BPE_ASSORTEDFILEDELETED, id); }
private:
    BLID_t Add(ButcherMetadataData *metadata);
};

#endif // __BPROJECT_BUTCHERPROJECTASSORTEDFILES_H__
