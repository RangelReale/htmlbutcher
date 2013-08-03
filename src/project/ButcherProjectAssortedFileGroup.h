//=============================================================================
/**
 *  @file   ButcherProjectAssortedFileGroup.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTASSORTEDFILEGROUP_H__
#define __BPROJECT_BUTCHERPROJECTASSORTEDFILEGROUP_H__

#include <wx/wx.h>
#include "ButcherProjectBaseBLId.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"

class ButcherProject;
class ButcherProjectAssortedFileGroups;

/**
 * @class ButcherProjectAssortedFileGroup
 *
 * @brief Project assorted file group
 */
class ButcherProjectAssortedFileGroup : public ButcherProjectBaseBLId {
public:
    ButcherProjectAssortedFileGroup(ButcherProject *project, const wxString &name);
    virtual ~ButcherProjectAssortedFileGroup();

    const wxString &GetName() { return name_; }
    void SetName(const wxString &n) { name_=n; ProjectModified(); }

    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
protected:
    ButcherProjectAssortedFileGroup(ButcherProject *project);

    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_ASSORTEDFILEGROUPMODIFIED; }

    friend class ButcherProjectAssortedFileGroups;
    friend class ButcherList<ButcherProjectAssortedFileGroup>;
private:
    wxString name_;
};


#endif // __BPROJECT_BUTCHERPROJECTASSORTEDFILEGROUP_H__
