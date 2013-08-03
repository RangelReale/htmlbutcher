//=============================================================================
/**
 *  @file   ButcherProjectBaseAutoDisable.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectBaseAutoDisable
/////////////////////////////////
ButcherProjectBaseAutoDisable::ButcherProjectBaseAutoDisable(ButcherProjectBaseModify *project) :
    project_(project->GetProject()), active_(false)
{
    project_->DisableUpdate();
    active_=true;
}



ButcherProjectBaseAutoDisable::ButcherProjectBaseAutoDisable(ButcherProject *project) :
    project_(project), active_(false)
{
    project_->DisableUpdate();
    active_=true;
}



void ButcherProjectBaseAutoDisable::release()
{
    if (active_)
    {
        project_->EnableUpdate();
        active_=false;
    }
}


