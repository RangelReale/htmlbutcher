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
class ButcherDocumentMouseEvent;
wxDECLARE_EVENT(wxEVT_BUTCHERDOCUMENTMOUSE_ACTION, ButcherDocumentMouseEvent);

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
	wxEVENT_HANDLER_CAST(ButcherDocumentMouseEventFunction, func)

#define EVT_BUTCHERDOCUMENTMOUSE(id, fn) \
    wx__DECLARE_EVT1(wxEVT_BUTCHERDOCUMENTMOUSE_ACTION, id, ButcherDocumentMouseEventHandler(fn))

#endif // __BVIEW_BUTCHERDOCUMENTMOUSEEVENT_H__
