//=============================================================================
/**
 *  @file   ButcherProjectLineDefault.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectLineDefault.h"



/////////////////////////////////
// CLASS
//      ButcherProjectLineDefault
/////////////////////////////////
ButcherProjectLineDefault::ButcherProjectLineDefault(ButcherProjectMaskContainer *container, ButcherProjectLine *start,
    ButcherProjectLine *end,
    orientation_t orientation, unsigned int position) throw() :
    ButcherProjectLine(container, start, end, orientation), position_(position)
{

}


