//=============================================================================
/**
 *  @file   ButcherProjectImageFormat.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTIMAGEFORMAT_H__
#define __BPROJECT_BUTCHERPROJECTIMAGEFORMAT_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseBLId.h"
#include "BImage.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"

class ButcherProject;

/**
 * @class ButcherProjectImageFormat
 *
 * @brief Project image format
 */
class ButcherProjectImageFormat : public ButcherProjectBaseBLId {
public:
    ButcherProjectImageFormat(ButcherProject *project, const wxString &name,
        ButcherImage::format_t format, int flags);

    void SetName(const wxString &name) { name_=name; ProjectModified(); }
    const wxString &GetName() { return name_; }

    void SetFormat(ButcherImage::format_t format) { format_=format; ProjectModified(); }
    ButcherImage::format_t GetFormat() { return format_; }

    void SetFlags(int flags) { flags_=flags; ProjectModified(); }
    int GetFlags() { return flags_; }

    void Set(const wxString &name,
        ButcherImage::format_t format, int flags);

    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
protected:
    ButcherProjectImageFormat(ButcherProject *project);

    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_IMAGEFORMATMODIFIED; }

    friend class ButcherList<ButcherProjectImageFormat>;
    friend class ButcherProjectImageFormats;
private:
    wxString name_;
    ButcherImage::format_t format_;
    int flags_;
};

#endif // __BPROJECT_BUTCHERPROJECTIMAGEFORMAT_H__