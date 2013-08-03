//=============================================================================
/**
 *  @file    ButcherStatusEvent.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERSTATUSEVENT_H__
#define __BVIEW_BUTCHERSTATUSEVENT_H__

#include <wx/wx.h>

class ButcherView;

/**
 * @class ButcherStatusEvent
 *
 * @brief status displaying event
 */
DECLARE_EVENT_TYPE( wxEVT_BUTCHERSTATUS_ACTION, -1 )

class ButcherStatusEvent : public wxEvent
{
public:
    enum status_t { ST_OPERATION };

    ButcherStatusEvent(status_t status, const wxString &message,
        int id = 0, wxEventType commandType = wxEVT_BUTCHERSTATUS_ACTION);

    // required for sending with wxPostEvent()
    virtual wxEvent* Clone() const;

    const wxString &GetMessage() { return message_; }
    status_t GetStatus() { return status_; }
private:
    status_t status_;
    wxString message_;
};

typedef void (wxEvtHandler::*ButcherStatusEventFunction)(ButcherStatusEvent&);

#define ButcherStatusEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherStatusFunction, &func)

#define EVT_BUTCHERSTATUS(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERSTATUS_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherStatusEventFunction, & fn ), (wxObject *) NULL ),

#endif // __BVIEW_BUTCHERSTATUSEVENT_H__
