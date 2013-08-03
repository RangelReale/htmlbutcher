//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerAreaGlobal.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskContainerAreaGlobal.h"
#include "ButcherProjectMask.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskContainerAreaGlobal
/////////////////////////////////
ButcherProjectArea::areaflags_t ButcherProjectMaskContainerAreaGlobal::InternalGetFlags(
    ButcherProjectArea::areaflags_t curflags)
{
    return ButcherProjectMaskContainerSplit::InternalGetFlags(curflags|ButcherProjectArea::AF_HAVEIMAGE);
}


