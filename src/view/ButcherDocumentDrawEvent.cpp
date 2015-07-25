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

//DEFINE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTDRAW_ACTION )

ButcherDocumentDrawEvent::ButcherDocumentDrawEvent() :
	QEvent(staticType())
{

}


ButcherDocumentDrawEvent::ButcherDocumentDrawEvent(ButcherDocument *document,
    QPainter *painter, /*const wxRegion &updateregion,*/ int id/*, wxEventType commandType*/) :
    //wxEvent(id, commandType), 
	QEvent(staticType()),
	document_(document), painter_(painter) /*, updateregion_(updateregion)*/
{
    //m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}

#ifdef QT_HIDE_FROM

wxEvent* ButcherDocumentDrawEvent::Clone() const
{
    return new ButcherDocumentDrawEvent(*this);
}

#endif // QT_HIDE_FROM
