//=============================================================================
/**
 *  @file   ButcherProjectAreaConfig.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTAREACONFIG_H__
#define __BPROJECT_BUTCHERPROJECTAREACONFIG_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectAreaConfigBase.h"
#include "ButcherProjectAreaConfigPreview.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"

class ButcherProjectArea;
class ButcherProjectMask;
class ButcherProjectMaskInner;

/**
 * @class ButcherProjectAreaConfig
 *
 * @brief Project area config
 */
class ButcherProjectAreaConfig : public ButcherProjectAreaConfigBase {
public:
    ButcherProjectAreaConfig(ButcherProjectArea *area, BLID_t id = 0);
    virtual ~ButcherProjectAreaConfig();

    virtual void SetAreaKind(areakind_t k);
    virtual void SetHaveMap(bool s);

    bool CanSlice();
    bool WillSliceClearAreas();
    void Slice(int rows, int cols);

    ButcherProjectMask *GetInnerMask();
    ButcherProjectAreaConfigPreview *GetPreview(BLID_t viewid);

    virtual void LoadMetadata(ButcherMetadataData *metadata);
    virtual void SaveMetadata(ButcherMetadataData *metadata);

    virtual void ProjectEvent(ButcherProjectEventNotify &event);
private:
    ButcherProjectMaskInner* innermask_;
    ButcherProjectAreaConfigPreview *preview_;
};

#endif // __BPROJECT_BUTCHERPROJECTAREACONFIG_H__
