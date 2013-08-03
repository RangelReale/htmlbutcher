//=============================================================================
/**
 *  @file    ButcherViewChangedEvent.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherViewChangedEvent.h"



/////////////////////////////////
// CLASS
//      ButcherViewChangedEvent
/////////////////////////////////



DEFINE_EVENT_TYPE( wxEVT_BUTCHERVIEWCHANGED_ACTION )

ButcherViewChangedEvent::ButcherViewChangedEvent(ButcherViewChangedEvent::change_t change,
        int id, wxEventType commandType) :
    wxEvent(id, commandType), change_(change)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}




wxEvent* ButcherViewChangedEvent::Clone() const
{
    return new ButcherViewChangedEvent(*this);
}


