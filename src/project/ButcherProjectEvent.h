//=============================================================================
/**
 *  @file   ButcherProjectEvent.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTEVENT_H__
#define __BPROJECT_BUTCHERPROJECTEVENT_H__

#include <wx/wx.h>
#include "BList.h"

class ButcherProject;

/**
 * @class ButcherProjectEvent
 *
 * @brief Project event
 */
DECLARE_EVENT_TYPE( wxEVT_BUTCHERPROJECT_ACTION, -1 )

class ButcherProjectEvent : public wxEvent
{
public:
    enum event_t {
        BPE_PROJECTOPEN, BPE_PROJECTCLOSE, BPE_PROJECTCLOSING, BPE_PROJECTNEW,
        BPE_PROJECTSAVE,

        BPE_MODIFIED,

        BPE_FILEMODIFIED, BPE_MASKMODIFIED, BPE_MASKCONFIGMODIFIED, BPE_VIEWMODIFIED, BPE_COLORSCHEMEMODIFIED,
        BPE_IMAGEFORMATMODIFIED, BPE_ASSORTEDFILEMODIFIED, BPE_ASSORTEDFILEGROUPMODIFIED, BPE_FILEPATHMODIFIED,
        BPE_CSSFILEMODIFIED,

        BPE_FILEDELETED, BPE_MASKDELETED, BPE_MASKCONFIGDELETED, BPE_VIEWDELETED, BPE_COLORSCHEMEDELETED,
        BPE_IMAGEFORMATDELETED, BPE_ASSORTEDFILEDELETED, BPE_ASSORTEDFILEGROUPDELETED, BPE_FILEPATHDELETED,
        BPE_CSSFILEDELETED,

		BPE_OPTIONSMODIFIED,

        BPE_PROGRESS };

    ButcherProjectEvent(ButcherProject *project, event_t event, BLID_t eid, BLID_t eid2,
        int id = 0, wxEventType commandType = wxEVT_BUTCHERPROJECT_ACTION);

    // required for sending with wxPostEvent()
    virtual wxEvent* Clone() const;

    ButcherProject *GetProject() { return project_; }
    event_t GetEvent() const { return event_; }
    BLID_t GetEId() const { return eid_; }
    BLID_t GetEId2() const { return eid2_; }
private:
    ButcherProject *project_;
    event_t event_;
    BLID_t eid_, eid2_;
};

typedef void (wxEvtHandler::*ButcherProjectEventFunction)(ButcherProjectEvent&);

#define ButcherProjectEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction) \
	wxStaticCastEvent(ButcherProjectEventFunction, &func)

#define EVT_BUTCHERPROJECT(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERPROJECT_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( ButcherProjectEventFunction, & fn ), (wxObject *) NULL ),

#endif // __BPROJECT_BUTCHERPROJECTEVENT_H__