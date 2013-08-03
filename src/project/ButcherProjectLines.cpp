//=============================================================================
/**
 *  @file   ButcherProjectLines.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectLines.h"
#include "ButcherProjectMaskContainer.h"
#include "ButcherProjectMaskContainerJoined.h"
#include "ButcherProjectMaskContainerSplit.h"



/////////////////////////////////
// CLASS
//      ButcherProjectLines
/////////////////////////////////
ButcherProjectLines::ButcherProjectLines(ButcherProjectMaskContainer *container) :
    ButcherList<ButcherProjectLine>(), ButcherProjectBaseModify(container->GetProject()), container_(container)
{

}




void ButcherProjectLines::do_modified(BLID_t id)
{
    ProjectModified(id);
}



void ButcherProjectLines::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    container_->ProjectModified();
}


