//=============================================================================
/**
 *  @file   ButcherProjectMaskInner.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKINNER_H__
#define __BPROJECT_BUTCHERPROJECTMASKINNER_H__

#include <wx/wx.h>
#include "ButcherProjectMask.h"
#include "ButcherProjectArea.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectMaskContainerArea.h"
#include "ButcherProjectMaskContainerAreaMap.h"
#include "ButcherProjectAreaConfig.h"

class ButcherProject;

/**
 * @class ButcherProjectMaskInner
 *
 * @brief Project mask inner
 */
class ButcherProjectMaskInner : public ButcherProjectMask {
public:
    ButcherProjectMaskInner(ButcherProject *project, ButcherProjectArea *parent,
        const wxString &name);
    virtual ~ButcherProjectMaskInner();

    void EnableArea(bool enable);
    void EnableAreaMap(bool enable);

    virtual void LoadMetadataContainer(ButcherMetadataData *metadata);
private:
    // must create dynamically or an infinite loop can happen
    ButcherProjectMaskContainerArea* areacontainer_;
    ButcherProjectMaskContainerAreaMap* areamapcontainer_;

    friend class ButcherProjectAreaConfig;
};

#endif // __BPROJECT_BUTCHERPROJECTMASKINNER_H__
