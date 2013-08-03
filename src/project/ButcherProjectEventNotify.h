//=============================================================================
/**
 *  @file   ButcherProjectEventNotify.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTEVENTNOTIFY_H__
#define __BPROJECT_BUTCHERPROJECTEVENTNOTIFY_H__

#include <wx/wx.h>
#include "ButcherProjectEvent.h"

class ButcherProject;

/**
 * @class ButcherProjectEventNotify
 *
 * @brief Project event notify
 */
class ButcherProjectEventNotify
{
public:
    ButcherProjectEventNotify(ButcherProjectEvent *event, bool isexecute = true) :
        event_(event), owns_(false), isexecute_(isexecute), changecount_(0), removecount_(0) {}
    ButcherProjectEventNotify(ButcherProjectEvent &event, bool isexecute = true) :
        event_(&event), owns_(false), isexecute_(isexecute), changecount_(0), removecount_(0) {}
    ButcherProjectEventNotify(ButcherProject *project, ButcherProjectEvent::event_t event,
        BLID_t eid = 0, BLID_t eid2 = 0, bool isexecute = true) :
        event_(new ButcherProjectEvent(project, event, eid, eid2)),
        owns_(true), isexecute_(isexecute), changecount_(0), removecount_(0) {}

    ~ButcherProjectEventNotify() { if (owns_) delete event_; }

    ButcherProjectEvent &GetProjectEvent() { return *event_; }
    bool GetIsExecute() { return isexecute_; }

    long GetChangeCount() { return changecount_; }
    long GetRemoveCount() { return removecount_; }

    void AddChange(int change=1) { changecount_+=change; }
    void AddRemove(int remove=1) { removecount_+=remove; }
private:
    ButcherProjectEvent *event_;
    bool owns_;
    bool isexecute_;
    long changecount_, removecount_;
};

#endif // __BPROJECT_BUTCHERPROJECTEVENTNOTIFY_H__