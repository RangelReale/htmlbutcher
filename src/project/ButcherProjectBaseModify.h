//=============================================================================
/**
 *  @file   ButcherProjectBaseModify.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTBASEMODIFY_H__
#define __BPROJECT_BUTCHERPROJECTBASEMODIFY_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBase.h"
#include "ButcherProjectEvent.h"

class ButcherProject;

/**
 * @class ButcherProjectBaseModify
 *
 * @brief Project base class
 */
class ButcherProjectBaseModify : public ButcherProjectBase {
public:
    ButcherProjectBaseModify(ButcherProject *project) :
        ButcherProjectBase(project) {}
    virtual ~ButcherProjectBaseModify() {}

    virtual void ProjectModified() { ProjectModified(0); }
    void ProjectModified(BLID_t id, BLID_t id2 = 0) { DoProjectModified(GetProjectModifiedItem(), id, id2); }
protected:
    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_MODIFIED; }

    void DoProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2 = 0);
    virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
};


#endif // __BPROJECT_BUTCHERPROJECTBASEMODIFY_H__