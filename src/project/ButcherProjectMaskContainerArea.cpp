//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerArea.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskContainerArea.h"
#include "ButcherProjectMask.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskContainerArea
/////////////////////////////////
ButcherProjectArea::areaflags_t ButcherProjectMaskContainerArea::InternalGetFlags(
    ButcherProjectArea::areaflags_t curflags)
{
    return ButcherProjectMaskContainerJoined::InternalGetFlags(curflags|ButcherProjectArea::AF_HAVEIMAGE);
}



