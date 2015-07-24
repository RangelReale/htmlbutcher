//=============================================================================
/**
 *  @file    ButcherViewSelectEvent.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERVIEWSELECTEVENT_H__
#define __BVIEW_BUTCHERVIEWSELECTEVENT_H__

#include <wx/wx.h>

#include <QEvent>
#include <QMetaType>

class ButcherView;
class ButcherProjectView;
class ButcherProjectMaskLineSelect;
class ButcherProjectMaskAreaSelect;

/**
 * @class ButcherViewSelectEvent
 *
 * @brief view item selected event
 */
//DECLARE_EVENT_TYPE( wxEVT_BUTCHERSELECT_ACTION, -1 )

class ButcherViewSelectEvent : public QEvent // wxEvent
{
public:
    enum select_t { SEL_NONE, SEL_HOVER, SEL_POSITION, SEL_LINE, SEL_LINEHOVER, SEL_LINEEDIT,
        SEL_DEFLINE, SEL_AREA, SEL_AREAHOVER, SEL_AREAEDIT };

	ButcherViewSelectEvent();
    ButcherViewSelectEvent(ButcherView *view, ButcherProjectView *pview,
        select_t select, long x=0, long y=0,
        ButcherProjectMaskLineSelect *lineselect = NULL,
        ButcherProjectMaskAreaSelect *areaselect = NULL,
        int id = 0/*, wxEventType commandType = wxEVT_BUTCHERSELECT_ACTION*/);
	// copy constructor
	ButcherViewSelectEvent(const ButcherViewSelectEvent &event);
    virtual ~ButcherViewSelectEvent();

	static QEvent::Type staticType()
	{
		static int type = QEvent::registerEventType();
		return static_cast<QEvent::Type>(type);
	}

    // required for sending with wxPostEvent()
    //virtual wxEvent* Clone() const;

    ButcherView *GetView() { return view_; }
    ButcherProjectView *GetProjectView() { return pview_; }
    select_t GetSelect() { return select_; }
    ButcherProjectMaskLineSelect *GetLineSelect() { return lineselect_; }
    ButcherProjectMaskAreaSelect *GetAreaSelect() { return areaselect_; }
    long GetX() { return x_; }
    long GetY() { return y_; }

	bool IsSameArea(const ButcherViewSelectEvent &event);
private:
    ButcherView *view_;
    ButcherProjectView *pview_;
    select_t select_;
    long x_, y_;
    ButcherProjectMaskLineSelect *lineselect_;
    ButcherProjectMaskAreaSelect *areaselect_;
};

#ifdef QT_HIDE_FROM

typedef void (wxEvtHandler::*ButcherViewSelectEventFunction)(ButcherViewSelectEvent&);

#define ButcherViewSelectEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherViewSelecEventFunction, &func)

#define EVT_BUTCHERVIEWSELECT(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERSELECT_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherViewSelectEventFunction, & fn ), (wxObject *) NULL ),

#endif //QT_HIDE_FROM


#endif // __BVIEW_BUTCHERVIEWSELECTEVENT_H__
