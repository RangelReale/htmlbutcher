//=============================================================================
/**
 *  @file   ButcherProjectAreas.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTAREAS_H__
#define __BPROJECT_BUTCHERPROJECTAREAS_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectEventNotify.h"

class ButcherProjectMaskContainer;
class ButcherProjectMaskContainerJoined;
class ButcherProjectMaskContainerSplit;

/**
 * @class ButcherProjectAreas
 *
 * @brief Project lines
 */
class ButcherProjectAreas : public ButcherList<ButcherProjectArea>, public ButcherProjectBaseModify
{
public:
    ButcherProjectAreas(ButcherProjectMaskContainer *container);

    virtual void ProjectEvent(ButcherProjectEventNotify &event) {
        ButcherProjectBaseModify::ProjectEvent(event);
		for (iterator i=begin(); i!=end(); i++)
			i->ProjectEvent(event);
    }
protected:
    BLID_t Add(ButcherProjectArea* item, BLID_t id = 0) {
        return op_add(item, id);
    }
    bool Delete(ButcherProjectArea* item) {
        return op_delete(item->GetBLId());
    }
    virtual void do_modified(BLID_t id = 0);

    virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);

    friend class ButcherProjectMaskContainer;
    friend class ButcherProjectMaskContainerJoined;
    friend class ButcherProjectMaskContainerSplit;
private:
    ButcherProjectMaskContainer *container_;
};

#endif // __BPROJECT_BUTCHERPROJECTAREAS_H__
