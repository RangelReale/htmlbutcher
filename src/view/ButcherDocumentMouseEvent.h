//=============================================================================
/**
 *  @file    ButcherDocumentMouseEvent.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERDOCUMENTMOUSEEVENT_H__
#define __BVIEW_BUTCHERDOCUMENTMOUSEEVENT_H__

#include <wx/wx.h>

#include <QMouseEvent>
#include <QMetaType>

class ButcherView;

/**
 * @class ButcherDocumentMouseEvent
 *
 * @brief document mouse event
 */
//DECLARE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTMOUSE_ACTION, -1 )

class ButcherDocumentMouseEvent : public QEvent //wxMouseEvent
{
public:
	ButcherDocumentMouseEvent();
    ButcherDocumentMouseEvent(QMouseEvent origCommandType/*,
        wxEventType commandType = wxEVT_BUTCHERDOCUMENTMOUSE_ACTION*/);
    //ButcherDocumentMouseEvent(const wxMouseEvent &event);

	static QEvent::Type staticType()
	{
		static int type = QEvent::registerEventType();
		return static_cast<QEvent::Type>(type);
	}

    // required for sending with wxPostEvent()
    //virtual wxEvent* Clone() const;

	QMouseEvent GetOriginEventType() { return orig_; }
private:
	QMouseEvent orig_;
};

#ifdef QT_HIDE_FROM

typedef void (wxEvtHandler::*ButcherDocumentMouseEventFunction)(ButcherDocumentMouseEvent&);

#define ButcherDocumentMouseEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherDocumentMouseEventFunction, &func)

#define EVT_BUTCHERDOCUMENTMOUSE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERDOCUMENTMOUSE_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherDocumentMouseEventFunction, & fn ), (wxObject *) NULL ),

#endif // QT_HIDE_FROM

#endif // __BVIEW_BUTCHERDOCUMENTMOUSEEVENT_H__
