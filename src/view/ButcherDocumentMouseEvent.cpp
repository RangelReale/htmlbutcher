//=============================================================================
/**
 *  @file    ButcherDocumentMouseEvent.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherDocumentMouseEvent.h"



/////////////////////////////////
// CLASS
//      ButcherDocumentMouseEvent
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTMOUSE_ACTION )

ButcherDocumentMouseEvent::ButcherDocumentMouseEvent(wxEventType origCommandType,
    wxEventType commandType) :
    wxMouseEvent(commandType), orig_(origCommandType)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX;
}




ButcherDocumentMouseEvent::ButcherDocumentMouseEvent(const wxMouseEvent &event) :
    wxMouseEvent(event)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX;
    orig_=GetEventType();
    SetEventType(wxEVT_BUTCHERDOCUMENTMOUSE_ACTION);
}




// required for sending with wxPostEvent()
wxEvent* ButcherDocumentMouseEvent::Clone() const
{
    return new ButcherDocumentMouseEvent(*this);
}


