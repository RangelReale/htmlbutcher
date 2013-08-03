//=============================================================================
/**
 *  @file   ButcherProjectFiles.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTFILES_H__
#define __BPROJECT_BUTCHERPROJECTFILES_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectFile.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"

class ButcherProject;

/**
 * @class ButcherProjectFiles
 *
 * @brief Project files
 */
class ButcherProjectFiles : public ButcherList_2<ButcherProjectFile>, public ButcherProjectBaseModify
{
public:
    ButcherProjectFiles(ButcherProject *project);
    BLID_t Add(const wxString &name, const wxString &filename);
    BLID_t Add(const wxString &name, wxInputStream &filedata);
    void Edit(BLID_t id, const wxString &name, const wxString &filename);

    bool Delete(BLID_t id) { return op_delete(id); }
    void Clear() { op_clear(); }

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);
    void UpdateMetadata(ButcherMetadataDataList *metadatalist);

    virtual void ProjectEvent(ButcherProjectEventNotify &event) {
        ButcherProjectBaseModify::ProjectEvent(event);
		for (iterator i=begin(); i!=end(); i++)
			i->ProjectEvent(event);
    }
protected:
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_FILEMODIFIED; }

    virtual bool can_delete(BLID_t id, ButcherProjectFile* item);
    virtual void do_modified(BLID_t id = 0) { ProjectModified(id); }
    virtual void do_deleting(unsigned int id, ButcherProjectFile* item) { DoProjectModified(ButcherProjectEvent::BPE_FILEDELETED, id); }
private:
    BLID_t Add(ButcherMetadataData *metadata);
};

#endif // __BPROJECT_BUTCHERPROJECTFILES_H__
