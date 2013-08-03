//=============================================================================
/**
 *  @file   ButcherProjectMaskInner.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskInner.h"
#include "ButcherFileDefs.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskInner
/////////////////////////////////
ButcherProjectMaskInner::ButcherProjectMaskInner(ButcherProject *project,
    ButcherProjectArea *parent, const wxString &name) :
        ButcherProjectMask(project, parent, name),
    areacontainer_(NULL), areamapcontainer_(NULL)
{
    if (!GetParent())
        throw ButcherException(_("Inner areas must have a parent"));
}




ButcherProjectMaskInner::~ButcherProjectMaskInner()
{
    if (areacontainer_) delete areacontainer_;
    if (areamapcontainer_) delete areamapcontainer_;
}



void ButcherProjectMaskInner::EnableArea(bool enable)
{
    if (enable)
    {
        if (!areacontainer_) areacontainer_=new ButcherProjectMaskContainerArea(this);
        SetContainer(ButcherProjectArea::AC_DEFAULT, areacontainer_);
    }
    else
        SetContainer(ButcherProjectArea::AC_DEFAULT, NULL);
}




void ButcherProjectMaskInner::EnableAreaMap(bool enable)
{
    if (enable)
    {
        if (!areamapcontainer_) areamapcontainer_=new ButcherProjectMaskContainerAreaMap(this);
        SetContainer(ButcherProjectArea::AC_MAP, areamapcontainer_);
    }
    else
        SetContainer(ButcherProjectArea::AC_MAP, NULL);
}





void ButcherProjectMaskInner::LoadMetadataContainer(ButcherMetadataData *metadata)
{
    ButcherProjectMask::LoadMetadataContainer(metadata);

    if (metadata->Exists(BFILE_MDI_AREACLASS))
    {
        if (areacontainer_ &&
            metadata->Get(BFILE_MDI_AREACLASS).GetInteger()==ButcherProjectArea::AC_DEFAULT)
            areacontainer_->LoadMetadata(metadata);
        else if (areamapcontainer_ &&
            metadata->Get(BFILE_MDI_AREACLASS).GetInteger()==ButcherProjectArea::AC_GLOBAL)
            areamapcontainer_->LoadMetadata(metadata);
    }
}


