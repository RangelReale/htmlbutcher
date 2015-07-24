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

//DEFINE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTKEY_ACTION )

ButcherDocumentKeyEvent::ButcherDocumentKeyEvent() :
	QEvent(staticType()), orig_(QEvent::KeyPress, 0, 0)
{


}

ButcherDocumentKeyEvent::ButcherDocumentKeyEvent(QKeyEvent origCommandType/*,
    wxEventType commandType*/) :
    //wxKeyEvent(commandType), 
	QEvent(staticType()),
	orig_(origCommandType)
{
    //m_propagationLevel=wxEVENT_PROPAGATE_MAX;
}


#ifdef QT_HIDE_FROM


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

#endif // QT_HIDE_FROM


