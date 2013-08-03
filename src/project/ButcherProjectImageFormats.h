//=============================================================================
/**
 *  @file   ButcherProjectImageFormats.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTIMAGEFORMATS_H__
#define __BPROJECT_BUTCHERPROJECTIMAGEFORMATS_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectImageFormat.h"
#include "ButcherProjectBaseModify.h"
#include "BImage.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"

class ButcherProject;

/**
 * @class ButcherProjectImageFormats
 *
 * @brief Image format list
 */
class ButcherProjectImageFormats : public ButcherList<ButcherProjectImageFormat>, public ButcherProjectBaseModify
{
public:
    ButcherProjectImageFormats(ButcherProject *project);

    BLID_t Add(const wxString &name, ButcherImage::format_t format, int flags, BLID_t id = 0);
    void Edit(BLID_t id, const wxString &name, ButcherImage::format_t format, int flags);
    bool Delete(BLID_t id) { return op_delete(id); }
    void Clear() { op_clear(); }

    void LoadDefault();

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);

    virtual void ProjectEvent(ButcherProjectEventNotify &event) {
        ButcherProjectBaseModify::ProjectEvent(event);
		for (iterator i=begin(); i!=end(); i++)
			i->ProjectEvent(event);
    }
protected:
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_IMAGEFORMATMODIFIED; }

    virtual void do_deleting(BLID_t id, ButcherProjectImageFormat* item) { DoProjectModified(ButcherProjectEvent::BPE_IMAGEFORMATDELETED, id); }
    virtual void do_modified(BLID_t id = 0) { ProjectModified(id); }
    virtual bool can_delete(BLID_t id, ButcherProjectImageFormat *item) { return Count()>1; } // need at least 1 image format
private:
    BLID_t Add(ButcherMetadataData *metadata);
};

#endif // __BPROJECT_BUTCHERPROJECTIMAGEFORMATS_H__
