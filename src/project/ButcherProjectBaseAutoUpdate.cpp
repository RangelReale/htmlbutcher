//=============================================================================
/**
 *  @file   ButcherProjectBaseAutoUpdate.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"

/////////////////////////////////
// CLASS
//      ButcherProjectBaseAutoUpdate
/////////////////////////////////
ButcherProjectBaseAutoUpdate::ButcherProjectBaseAutoUpdate(ButcherProjectBaseModify *project) :
    project_(project->GetProject()), active_(false)
{
    project_->BeginUpdate();
    active_=true;
}



ButcherProjectBaseAutoUpdate::ButcherProjectBaseAutoUpdate(ButcherProject *project) :
    project_(project), active_(false)
{
    project_->BeginUpdate();
    active_=true;
}



void ButcherProjectBaseAutoUpdate::release()
{
    if (active_)
    {
        project_->EndUpdate();
        active_=false;
    }
}


