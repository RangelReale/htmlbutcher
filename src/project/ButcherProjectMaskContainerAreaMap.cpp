//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerAreaMap.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskContainerAreaMap.h"
#include "ButcherProjectMask.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskContainerAreaMap
/////////////////////////////////
ButcherProjectArea::areaflags_t ButcherProjectMaskContainerAreaMap::InternalGetFlags(
    ButcherProjectArea::areaflags_t curflags)
{
    ButcherProjectArea::areaflags_t nflags=ButcherProjectMaskContainerSplit::InternalGetFlags(curflags) & ~ButcherProjectArea::AF_HAVEIMAGE;
    return nflags;
}


