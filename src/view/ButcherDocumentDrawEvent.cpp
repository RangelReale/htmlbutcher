//=============================================================================
/**
 *  @file    ButcherDocumentDrawEvent.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherDocumentDrawEvent.h"
#include "ButcherDocument.h"



/////////////////////////////////
// CLASS
//      ButcherDocumentDrawEvent
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTDRAW_ACTION )

ButcherDocumentDrawEvent::ButcherDocumentDrawEvent(ButcherDocument *document,
    wxDC *dc, const wxRegion &updateregion, int id, wxEventType commandType) :
    wxEvent(id, commandType), document_(document), dc_(dc), updateregion_(updateregion)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}




wxEvent* ButcherDocumentDrawEvent::Clone() const
{
    return new ButcherDocumentDrawEvent(*this);
}


