//=============================================================================
/**
 *  @file    ButcherViewChangedEvent.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERVIEWCHANGEDEVENT_H__
#define __BVIEW_BUTCHERVIEWCHANGEDEVENT_H__

#include <wx/wx.h>

#include <QEvent>
#include <QMetaType>

class ButcherView;

/**
 * @class ButcherViewChangedEvent
 *
 * @brief view changed event
 */
//DECLARE_EVENT_TYPE( wxEVT_BUTCHERVIEWCHANGED_ACTION, -1 )

class ButcherViewChangedEvent : public QEvent //wxEvent
{
public:
    enum change_t { VC_VIEW, VC_DESTROY, VC_MOVE };

	ButcherViewChangedEvent();
    ButcherViewChangedEvent(change_t change,
        int id = 0/*, wxEventType commandType = wxEVT_BUTCHERVIEWCHANGED_ACTION*/);

	static QEvent::Type staticType()
	{
		static int type = QEvent::registerEventType();
		return static_cast<QEvent::Type>(type);
	}

    // required for sending with wxPostEvent()
    //virtual wxEvent* Clone() const;

    change_t GetChange() { return change_; }
    long GetX() { return x_; }
    long GetY() { return y_; }

    void SetMove(long x, long y) { x_=x; y_=y; }
private:
    change_t change_;
    long x_, y_;
};

#ifdef QT_HIDE_FROM

typedef void (wxEvtHandler::*ButcherViewChangedEventFunction)(ButcherViewChangedEvent&);

#define ButcherViewChangedEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherViewChangedEventFunction, &func)

#define EVT_BUTCHERVIEWCHANGED(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERVIEWCHANGED_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherViewChangedEventFunction, & fn ), (wxObject *) NULL ),

#endif // QT_HIDE_FROM

#endif // __BVIEW_BUTCHERVIEWCHANGEDEVENT_H__
