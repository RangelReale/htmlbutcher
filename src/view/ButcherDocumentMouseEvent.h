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

class ButcherView;

/**
 * @class ButcherDocumentMouseEvent
 *
 * @brief document mouse event
 */
DECLARE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTMOUSE_ACTION, -1 )

class ButcherDocumentMouseEvent : public wxMouseEvent
{
public:
    ButcherDocumentMouseEvent(wxEventType origCommandType,
        wxEventType commandType = wxEVT_BUTCHERDOCUMENTMOUSE_ACTION);
    ButcherDocumentMouseEvent(const wxMouseEvent &event);

    // required for sending with wxPostEvent()
    virtual wxEvent* Clone() const;

    wxEventType GetOriginEventType() { return orig_; }
private:
    wxEventType orig_;
};

typedef void (wxEvtHandler::*ButcherDocumentMouseEventFunction)(ButcherDocumentMouseEvent&);

#define ButcherDocumentMouseEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherDocumentMouseEventFunction, &func)

#define EVT_BUTCHERDOCUMENTMOUSE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERDOCUMENTMOUSE_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherDocumentMouseEventFunction, & fn ), (wxObject *) NULL ),

#endif // __BVIEW_BUTCHERDOCUMENTMOUSEEVENT_H__
