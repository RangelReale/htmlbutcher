//=============================================================================
/**
 *  @file   ButcherProjectMaskRoot.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskRoot.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProjectArea.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskRoot
/////////////////////////////////
ButcherProjectMaskRoot::ButcherProjectMaskRoot(ButcherProject *project,
    const wxString &name,
    int width, int height) : ButcherProjectMask(project, NULL, name, width, height),
    areacontainer_(this), areaglobalcontainer_(this)
{
    Initialize();
}




ButcherProjectMaskRoot::ButcherProjectMaskRoot(ButcherProject *project) :
    ButcherProjectMask(project),
        areacontainer_(this), areaglobalcontainer_(this)
{
    Initialize();
}



ButcherProjectMaskRoot *ButcherProjectMaskRoot::clone()
{
    ButcherProjectMaskRoot *ret=new ButcherProjectMaskRoot(GetProject());
    ButcherMetadataStorage mds;

    SaveMetadata(mds.Get(0, true).Add());
    ret->LoadMetadata(&mds.Get(0).Get(0), 0);
    return ret;
}



void ButcherProjectMaskRoot::Initialize()
{
    ButcherProjectBaseAutoDisable autodis(this);

    SetContainer(ButcherProjectArea::AC_DEFAULT, &areacontainer_);
    SetContainer(ButcherProjectArea::AC_GLOBAL, &areaglobalcontainer_);
}


