//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerArea.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREA_H__
#define __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREA_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectMaskContainerJoined.h"
#include "ButcherProjectArea.h"

class ButcherProjectMask;

/**
 * @class ButcherProjectMaskContainerArea
 *
 * @brief Project mask container area
 */
class ButcherProjectMaskContainerArea : public ButcherProjectMaskContainerJoined {
public:
    ButcherProjectMaskContainerArea(ButcherProjectMask *mask) :
        ButcherProjectMaskContainerJoined(mask, ButcherProjectArea::AC_DEFAULT) {}
protected:
    virtual ButcherProjectArea::areaflags_t InternalGetFlags(ButcherProjectArea::areaflags_t curflags = 0);
};


#endif // __BPROJECT_BUTCHERPROJECTMASKCONTAINERAREA_H__