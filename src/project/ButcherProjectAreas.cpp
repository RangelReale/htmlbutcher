//=============================================================================
/**
 *  @file   ButcherProjectAreas.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAreas.h"
#include "ButcherProjectMaskContainer.h"
#include "ButcherProjectMaskContainerJoined.h"
#include "ButcherProjectMaskContainerSplit.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAreas
/////////////////////////////////
ButcherProjectAreas::ButcherProjectAreas(ButcherProjectMaskContainer *container) :
    ButcherList<ButcherProjectArea>(), ButcherProjectBaseModify(container->GetProject()), container_(container)
{

}




void ButcherProjectAreas::do_modified(BLID_t id)
{
    ProjectModified(id);
}




void ButcherProjectAreas::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    container_->ProjectModified();
}


