//=============================================================================
/**
 *  @file    ButcherViewSelectEvent.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherViewSelectEvent.h"
#include "ButcherProjectView.h"
#include "ButcherProjectMaskSelect.h"



/////////////////////////////////
// CLASS
//      ButcherViewSelectEvent
/////////////////////////////////

//DEFINE_EVENT_TYPE( wxEVT_BUTCHERSELECT_ACTION )

ButcherViewSelectEvent::ButcherViewSelectEvent() :
	//wxEvent(id, commandType), 
	QEvent(staticType())
{
	//m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}

ButcherViewSelectEvent::ButcherViewSelectEvent(ButcherView *view, ButcherProjectView *pview,
        ButcherViewSelectEvent::select_t select,
        long x, long y,
        ButcherProjectMaskLineSelect *lineselect,
        ButcherProjectMaskAreaSelect *areaselect,
        int id/*, wxEventType commandType*/) :
    //wxEvent(id, commandType), 
	QEvent(staticType()),
	view_(view), pview_(pview), select_(select),
    x_(x), y_(y), lineselect_(lineselect),
    areaselect_(areaselect)
{
    //m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}




ButcherViewSelectEvent::~ButcherViewSelectEvent()
{
    if (lineselect_) delete lineselect_;
    if (areaselect_) delete areaselect_;
}



ButcherViewSelectEvent::ButcherViewSelectEvent(const ButcherViewSelectEvent &event) :
	//wxEvent(event.GetId(), event.GetEventType()), 
	QEvent(staticType()),
	view_(event.view_), pview_(event.pview_), select_(event.select_),
		x_(event.x_), y_(event.y_), lineselect_(NULL),
		areaselect_(NULL)
{
    // copy entrire classes
    if (event.lineselect_) lineselect_=new ButcherProjectMaskLineSelect(*event.lineselect_);
    if (event.areaselect_) areaselect_=new ButcherProjectMaskAreaSelect(*event.areaselect_);
}


#ifdef QT_HIDE_FROM


wxEvent* ButcherViewSelectEvent::Clone() const
{
    ButcherViewSelectEvent *cln=new ButcherViewSelectEvent(*this);
    // copy entrire classes
    if (lineselect_) cln->lineselect_=new ButcherProjectMaskLineSelect(*lineselect_);
    if (areaselect_) cln->areaselect_=new ButcherProjectMaskAreaSelect(*areaselect_);

    return new ButcherViewSelectEvent(*this);
}


#endif // QT_HIDE_FROM


bool ButcherViewSelectEvent::IsSameArea(const ButcherViewSelectEvent &event)
{
	if (view_==event.view_ &&
		pview_==event.pview_ &&
		select_==event.select_)
	{
		if (lineselect_ && event.lineselect_)
		{
			if (lineselect_->GetCount()!=event.lineselect_->GetCount()) return false;

			for (unsigned int i=0; i<lineselect_->GetCount(); i++)
			{
				if (lineselect_->GetItem(i)!=event.lineselect_->GetItem(i)) return false;
			}
		}
		if (areaselect_ && event.areaselect_)
		{
			if (areaselect_->GetCount()!=event.areaselect_->GetCount()) return false;

			for (unsigned int i=0; i<areaselect_->GetCount(); i++)
			{
				if (areaselect_->GetItem(i)!=event.areaselect_->GetItem(i)) return false;
			}
		}

		return true;
	}
	return false;
}


