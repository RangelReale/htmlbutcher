//=============================================================================
/**
 *  @file   ButcherProjectEvent.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectEvent.h"



/////////////////////////////////
// CLASS
//      ButcherProjectEvent
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_BUTCHERPROJECT_ACTION )

ButcherProjectEvent::ButcherProjectEvent(ButcherProject *project,
    event_t event, BLID_t eid, BLID_t eid2, int id, wxEventType commandType) :
    wxEvent(id, commandType), project_(project), event_(event), eid_(eid), eid2_(eid2)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}



wxEvent* ButcherProjectEvent::Clone() const
{
    return new ButcherProjectEvent(*this);
}


