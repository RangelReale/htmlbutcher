//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerAreaGlobal.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREAGLOBAL_H__
#define __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREAGLOBAL_H__

#include <wx/wx.h>
#include "ButcherProjectMaskContainerSplit.h"
#include "ButcherProjectArea.h"

class ButcherProjectMask;

/**
 * @class ButcherProjectMaskContainerAreaGlobal
 *
 * @brief Project mask container area global
 */
class ButcherProjectMaskContainerAreaGlobal : public ButcherProjectMaskContainerSplit {
public:
    ButcherProjectMaskContainerAreaGlobal(ButcherProjectMask *mask) :
        ButcherProjectMaskContainerSplit(mask, ButcherProjectArea::AC_GLOBAL) {}
protected:
    virtual ButcherProjectArea::areaflags_t InternalGetFlags(ButcherProjectArea::areaflags_t curflags = 0);
};

#endif // __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREAGLOBAL_H__
