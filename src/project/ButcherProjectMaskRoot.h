//=============================================================================
/**
 *  @file   ButcherProjectMaskRoot.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKROOT_H__
#define __BPROJECT_BUTCHERPROJECTMASKROOT_H__

#include <wx/wx.h>
#include "ButcherProjectMask.h"
#include "ButcherProjectMaskContainerArea.h"
#include "ButcherProjectMaskContainerAreaGlobal.h"

class ButcherProject;
class ButcherProjectMasks;

/**
 * @class ButcherProjectMaskRoot
 *
 * @brief Project mask root
 */
class ButcherProjectMaskRoot : public ButcherProjectMask {
public:
    ButcherProjectMaskRoot(ButcherProject *project,
        const wxString &name,
        int width = 0, int height = 0);

    ButcherProjectMaskRoot *clone();
protected:
    ButcherProjectMaskRoot(ButcherProject *project);

    friend class ButcherProjectMasks;
private:
    void Initialize();

    ButcherProjectMaskContainerArea areacontainer_;
    ButcherProjectMaskContainerAreaGlobal areaglobalcontainer_;
};

#endif // __BPROJECT_BUTCHERPROJECTMASKROOT_H__
