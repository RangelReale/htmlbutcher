//=============================================================================
/**
 *  @file    ButcherDocumentKeyEvent.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherDocumentKeyEvent.h"



/////////////////////////////////
// CLASS
//      ButcherDocumentKeyEvent
/////////////////////////////////

wxDEFINE_EVENT(wxEVT_BUTCHERDOCUMENTKEY_ACTION, ButcherDocumentKeyEvent);

ButcherDocumentKeyEvent::ButcherDocumentKeyEvent(wxEventType origCommandType,
    wxEventType commandType) :
    wxKeyEvent(commandType), orig_(origCommandType)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX;
}




ButcherDocumentKeyEvent::ButcherDocumentKeyEvent(const wxKeyEvent &event) :
    wxKeyEvent(event)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX;
    orig_=GetEventType();
    SetEventType(wxEVT_BUTCHERDOCUMENTKEY_ACTION);
}




// required for sending with wxPostEvent()
wxEvent* ButcherDocumentKeyEvent::Clone() const
{
    return new ButcherDocumentKeyEvent(*this);
}



