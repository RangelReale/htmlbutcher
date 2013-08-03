//=============================================================================
/**
 *  @file   ButcherProjectViewFileCheck.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectViewFileCheck.h"



/////////////////////////////////
// CLASS
//      ButcherProjectViewFileCheck
/////////////////////////////////
ButcherProjectViewFileCheck::ButcherProjectViewFileCheck(ButcherProject *project) :
    ButcherProjectBase(project), filelist_()
{

}



void ButcherProjectViewFileCheck::Add(BLID_t viewid, const wxString &description, const wxString &filepath)
{
    check_t c;
    c.viewid=viewid;
    c.description=description;

    ccu_Path_wxString curpath(filepath);
    filelist_[curpath].push_back(c);
}


