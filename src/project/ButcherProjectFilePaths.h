//=============================================================================
/**
 *  @file   ButcherProjectFilePaths.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTFILEPATHS_H__
#define __BPROJECT_BUTCHERPROJECTFILEPATHS_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectFilePath.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"

class ButcherProject;

/**
 * @class ButcherProjectFilePaths
 *
 * @brief Project file paths
 */
class ButcherProjectFilePaths : public ButcherList<ButcherProjectFilePath>, public ButcherProjectBaseModify
{
public:
    ButcherProjectFilePaths(ButcherProject *project);
    BLID_t Add(const wxString &name, const wxString &path = wxEmptyString, BLID_t id=0);
    void Edit(BLID_t id, const wxString &name, const wxString &path = wxEmptyString);

    bool Delete(BLID_t id) { return op_delete(id); }
    void Clear() { op_clear(); }

    void LoadDefault();

    wxString GetFilePath(BLID_t filepathid, const wxString &basepath,
        bool includetrailing = true, const wxString &defpath = wxEmptyString);

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);
protected:
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_FILEPATHDELETED; }

    virtual void do_modified(BLID_t id = 0) { ProjectModified(id); }
    virtual void do_deleting(BLID_t id, ButcherProjectFilePath* item) { DoProjectModified(ButcherProjectEvent::BPE_FILEPATHDELETED, id); }
    virtual bool can_delete(BLID_t id, ButcherProjectFilePath *item) { return Count()>1; } // need at least 1
private:
    BLID_t Add(ButcherMetadataData *metadata);
};

#endif // __BPROJECT_BUTCHERPROJECTFILEPATHS_H__
