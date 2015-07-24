//=============================================================================
/**
 *  @file    ButcherDocumentKeyEvent.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERDOCUMENTKEYEVENT_H__
#define __BVIEW_BUTCHERDOCUMENTKEYEVENT_H__

#include <wx/wx.h>

#include <QKeyEvent>
#include <QMetaType>

class ButcherView;

/**
 * @class ButcherDocumentKeyEvent
 *
 * @brief document keyboard event
 */
//DECLARE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTKEY_ACTION, -1 )

class ButcherDocumentKeyEvent : public QEvent //wxKeyEvent
{
public:
	ButcherDocumentKeyEvent();
	ButcherDocumentKeyEvent(QKeyEvent origCommandType/*,
        wxEventType commandType = wxEVT_BUTCHERDOCUMENTKEY_ACTION*/);
	//ButcherDocumentKeyEvent(const wxKeyEvent &event);

	static QEvent::Type staticType()
	{
		static int type = QEvent::registerEventType();
		return static_cast<QEvent::Type>(type);
	}

    // required for sending with wxPostEvent()
    //virtual wxEvent* Clone() const;

	QKeyEvent GetOriginEventType() { return orig_; }
private:
	QKeyEvent orig_;
};

#ifdef QT_HIDE_FROM

typedef void (wxEvtHandler::*ButcherDocumentKeyEventFunction)(ButcherDocumentKeyEvent&);

#define ButcherDocumentKeyEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherDocumentKeyEventFunction, &func)

#define EVT_BUTCHERDOCUMENTKEY(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERDOCUMENTKEY_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherDocumentKeyEventFunction, & fn ), (wxObject *) NULL ),

#endif // QT_HIDE_FROM

#endif // __BVIEW_BUTCHERDOCUMENTKEYEVENT_H__
