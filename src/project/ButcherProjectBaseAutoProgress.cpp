//=============================================================================
/**
 *  @file   ButcherProjectBaseAutoProgress.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectBaseAutoProgress.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectBaseAutoProgress
/////////////////////////////////
ButcherProjectBaseAutoProgress::ButcherProjectBaseAutoProgress(ButcherProjectBase *project,
    const wxString &description, unsigned short flags) :
        project_(project->GetProject()), active_(false)
{
    pid_=project_->BeginProgress(description, flags);
    active_=true;
}



ButcherProjectBaseAutoProgress::ButcherProjectBaseAutoProgress(ButcherProject *project,
    const wxString &description, unsigned short flags) :
        project_(project), active_(false)
{
    pid_=project_->BeginProgress(description, flags);
    active_=true;
}



void ButcherProjectBaseAutoProgress::release()
{
    if (active_)
    {
        project_->EndProgress(pid_);
        active_=false;
    }
}



void ButcherProjectBaseAutoProgress::SetProgress(int p)
{
    project_->UpdateProgress(pid_, p);
}


