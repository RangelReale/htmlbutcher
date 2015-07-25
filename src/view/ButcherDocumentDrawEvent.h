//=============================================================================
/**
 *  @file    ButcherDocumentDrawEvent.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERDOCUMENTDRAWEVENT_H__
#define __BVIEW_BUTCHERDOCUMENTDRAWEVENT_H__

#include <wx/wx.h>

#include <QEvent>
#include <QMetaType>
#include <QPainter>

class ButcherView;
class ButcherDocument;

/**
 * @class ButcherDocumentDrawEvent
 *
 * @brief document draw event
 */
//DECLARE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTDRAW_ACTION, -1 )

class ButcherDocumentDrawEvent : public QEvent // wxEvent
{
public:
	ButcherDocumentDrawEvent();
    ButcherDocumentDrawEvent(ButcherDocument *document, QPainter *painter,
        //const wxRegion &updateregion,
        int id = 0/*, wxEventType commandType = wxEVT_BUTCHERDOCUMENTDRAW_ACTION*/);

	static QEvent::Type staticType()
	{
		static int type = QEvent::registerEventType();
		return static_cast<QEvent::Type>(type);
	}

    // required for sending with wxPostEvent()
    //virtual wxEvent* Clone() const;

    ButcherDocument *GetDocument() { return document_; }
    //wxDC *GetDC() { return dc_; }
	QPainter *GetPainter() { return painter_; }
    //const wxRegion &GetUpdateRegion() { return updateregion_; }
private:
    ButcherDocument *document_;
    //wxDC *dc_;
	QPainter *painter_;
    //wxRegion updateregion_;
};

#ifdef QT_HIDE_FROM

typedef void (wxEvtHandler::*ButcherDocumentDrawEventFunction)(ButcherDocumentDrawEvent&);

#define ButcherDocumentDrawEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherDocumentDrawEventFunction, &func)

#define EVT_BUTCHERDOCUMENTDRAW(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERDOCUMENTDRAW_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherDocumentDrawEventFunction, & fn ), (wxObject *) NULL ),

#endif // QT_HIDE_FROM


#endif // __BVIEW_BUTCHERDOCUMENTDRAWEVENT_H__
