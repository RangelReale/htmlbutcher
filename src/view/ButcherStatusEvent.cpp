//=============================================================================
/**
 *  @file    ButcherStatusEvent.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherStatusEvent.h"



/////////////////////////////////
// CLASS
//      ButcherStatusEvent
/////////////////////////////////

//DEFINE_EVENT_TYPE( wxEVT_BUTCHERSTATUS_ACTION )

ButcherStatusEvent::ButcherStatusEvent() :
	//wxEvent(id, commandType), 
	QEvent(staticType())
{
}

ButcherStatusEvent::ButcherStatusEvent(ButcherStatusEvent::status_t status, const wxString &message,
        int id/*, wxEventType commandType*/) :
    //wxEvent(id, commandType), 
	QEvent(staticType()),
	status_(status), message_(message)
{
    //m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}


#ifdef QT_HIDE_FROM


wxEvent* ButcherStatusEvent::Clone() const
{
    return new ButcherStatusEvent(*this);
}


#endif // QT_HIDE_FROM
