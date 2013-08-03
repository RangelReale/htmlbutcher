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
DECLARE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTKEY_ACTION, -1 )

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
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherDocumentKeyEventFunction, &func)

#define EVT_BUTCHERDOCUMENTKEY(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERDOCUMENTKEY_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherDocumentKeyEventFunction, & fn ), (wxObject *) NULL ),

#endif // __BVIEW_BUTCHERDOCUMENTKEYEVENT_H__
