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



//DEFINE_EVENT_TYPE( wxEVT_BUTCHERVIEWCHANGED_ACTION )

ButcherViewChangedEvent::ButcherViewChangedEvent() :
	QEvent(staticType())
{

}


ButcherViewChangedEvent::ButcherViewChangedEvent(ButcherViewChangedEvent::change_t change,
	int id/*, wxEventType commandType*/) :
	//wxEvent(id, commandType),
	QEvent(staticType()),
	change_(change)
{
    //m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}



#ifdef QT_HIDE_FROM

wxEvent* ButcherViewChangedEvent::Clone() const
{
    return new ButcherViewChangedEvent(*this);
}

#endif // QT_HIDE_FROM

