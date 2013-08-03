//=============================================================================
/**
 *  @file   ButcherProjectAreaConfig.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAreaConfig.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectMaskInner.h"
#include "ButcherFileDefs.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAreaConfig
/////////////////////////////////
ButcherProjectAreaConfig::ButcherProjectAreaConfig(ButcherProjectArea *area, BLID_t id) :
    ButcherProjectAreaConfigBase(area, id),
        preview_(NULL)
{
    innermask_=new ButcherProjectMaskInner(GetProject(), GetArea(), wxEmptyString);
}




ButcherProjectAreaConfig::~ButcherProjectAreaConfig()
{
    delete innermask_;
    if (preview_) delete preview_;
}




ButcherProjectAreaConfigPreview *ButcherProjectAreaConfig::GetPreview(BLID_t viewid)
{
    if (!preview_ || preview_->GetViewId() != viewid)
    {
        if (preview_) delete preview_;
        preview_=new ButcherProjectAreaConfigPreview(this, viewid);
    }
    return preview_;
}



ButcherProjectMask *ButcherProjectAreaConfig::GetInnerMask()
{
    return innermask_;
}




void ButcherProjectAreaConfig::SetAreaKind(areakind_t k)
{
    innermask_->EnableArea(k==AK_MASK);
    ButcherProjectAreaConfigBase::SetAreaKind(k);
}




void ButcherProjectAreaConfig::SetHaveMap(bool s)
{
    innermask_->EnableAreaMap(s);
    ButcherProjectAreaConfigBase::SetHaveMap(s);
}




bool ButcherProjectAreaConfig::CanSlice()
{
    return
        (GetArea()->GetAreaClass()!=ButcherProjectArea::AC_MAP);/* &&
        (GetAreaKind()==AK_MASK) &&
        (GetInnerMask()->Areas()) &&
        (GetInnerMask()->Areas()->Areas().Count()==1);*/
}




bool ButcherProjectAreaConfig::WillSliceClearAreas()
{
    return (
        innermask_->areacontainer_ &&
        innermask_->areacontainer_->Areas().Count()>1);
}




void ButcherProjectAreaConfig::Slice(int rows, int cols)
{
    if (!CanSlice() || rows<1 || cols<1)
        throw ButcherException(_("This area cannot be sliced"));
    if (rows>GetArea()->GetHeight()/5 ||
        cols>GetArea()->GetWidth()/5)
        throw ButcherException(_("Area size too small for slicing"));

    if (GetAreaKind()!=AK_MASK)
        SetAreaKind(AK_MASK);
    if (GetInnerMask()->Areas()->Areas().Count()>1)
        GetInnerMask()->Areas()->Clear();

    float sz=(float)GetArea()->GetHeight()/(float)rows;
    for (int r=1; r<rows; r++)
    {
        GetInnerMask()->Areas()->AddLine(
            GetInnerMask()->Areas()->GetLineLeft(),
            GetInnerMask()->Areas()->GetLineRight(),
            ButcherProjectLine::BPL_HORIZONTAL,
            (int)((float)r*sz));
    }

    sz=(float)GetArea()->GetWidth()/(float)cols;
    for (int c=1; c<cols; c++)
    {
        GetInnerMask()->Areas()->AddLine(
            GetInnerMask()->Areas()->GetLineTop(),
            GetInnerMask()->Areas()->GetLineBottom(),
            ButcherProjectLine::BPL_VERTICAL,
            (int)((float)c*sz));
    }
}




void ButcherProjectAreaConfig::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectAreaConfigBase::LoadMetadata(metadata);

    innermask_->EnableArea(GetAreaKind()==AK_MASK);
    innermask_->EnableAreaMap(GetHaveMap());

    if (metadata->Exists(BFILE_MDI_MASK) &&
        metadata->Get(BFILE_MDI_MASK).GetMetadata().Exists(BFILE_MD_MASK) &&
        metadata->Get(BFILE_MDI_MASK).GetMetadata().Get(BFILE_MD_MASK).Count()>0)
        innermask_->LoadMetadata(&metadata->Get(BFILE_MDI_MASK).GetMetadata().Get(BFILE_MD_MASK).Get(0)); // only 1 mask allowed
}




void ButcherProjectAreaConfig::SaveMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectAreaConfigBase::SaveMetadata(metadata);

    if (innermask_->HaveArea())
        // only 1 mask allowed
        innermask_->SaveMetadata(metadata->MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_MASK))->
            Get(BFILE_MDI_MASK).GetMetadata().Get(BFILE_MD_MASK, true).Add());
}




void ButcherProjectAreaConfig::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectAreaConfigBase::ProjectEvent(event);
    if (GetAreaKind()==AK_MASK)
        GetInnerMask()->ProjectEvent(event);
    if (preview_)
        preview_->ProjectEvent(event);
}


