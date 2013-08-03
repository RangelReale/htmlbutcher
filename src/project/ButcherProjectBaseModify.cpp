//=============================================================================
/**
 *  @file   ButcherProjectBaseModify.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectBaseModify.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectBaseModify
/////////////////////////////////
void ButcherProjectBaseModify::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id,
    BLID_t id2)
{
    GetProject()->Modified(mod, id, id2);
}



void ButcherProjectBaseModify::DoProjectModified(ButcherProjectEvent::event_t mod, BLID_t id,
    BLID_t id2)
{
    //if (updateenabled_)
        SendProjectModified(mod, id, id2);
}


