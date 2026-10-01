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

class ButcherView;

/**
 * @class ButcherDocumentKeyEvent
 *
 * @brief document keyboard event
 */
class ButcherDocumentKeyEvent;
wxDECLARE_EVENT(wxEVT_BUTCHERDOCUMENTKEY_ACTION, ButcherDocumentKeyEvent);

class ButcherDocumentKeyEvent : public wxKeyEvent
{
public:
    ButcherDocumentKeyEvent(wxEventType origCommandType,
        wxEventType commandType = wxEVT_BUTCHERDOCUMENTKEY_ACTION);
    ButcherDocumentKeyEvent(const wxKeyEvent &event);

    // required for sending with wxPostEvent()
    virtual wxEvent* Clone() const;

    wxEventType GetOriginEventType() { return orig_; }
private:
    wxEventType orig_;
};

typedef void (wxEvtHandler::*ButcherDocumentKeyEventFunction)(ButcherDocumentKeyEvent&);

#define ButcherDocumentKeyEventHandler(func) \
	wxEVENT_HANDLER_CAST(ButcherDocumentKeyEventFunction, func)

#define EVT_BUTCHERDOCUMENTKEY(id, fn) \
    wx__DECLARE_EVT1(wxEVT_BUTCHERDOCUMENTKEY_ACTION, id, ButcherDocumentKeyEventHandler(fn))

#endif // __BVIEW_BUTCHERDOCUMENTKEYEVENT_H__
