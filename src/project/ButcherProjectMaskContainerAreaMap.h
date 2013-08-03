//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerAreaMap.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREAMAP_H__
#define __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREAMAP_H__

#include <wx/wx.h>
#include "ButcherProjectMaskContainerSplit.h"
#include "ButcherProjectArea.h"

class ButcherProjectMask;

/**
 * @class ButcherProjectMaskContainerAreaMap
 *
 * @brief Project mask container area map
 */
class ButcherProjectMaskContainerAreaMap : public ButcherProjectMaskContainerSplit {
public:
    ButcherProjectMaskContainerAreaMap(ButcherProjectMask *mask) :
        ButcherProjectMaskContainerSplit(mask, ButcherProjectArea::AC_MAP) {}
protected:
    virtual ButcherProjectArea::areaflags_t InternalGetFlags(ButcherProjectArea::areaflags_t curflags = 0);
};

#endif // __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREAMAP_H__