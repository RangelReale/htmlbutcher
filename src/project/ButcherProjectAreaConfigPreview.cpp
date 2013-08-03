//=============================================================================
/**
 *  @file   ButcherProjectAreaConfigPreview.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAreaConfigPreview.h"
#include "ButcherProjectAreaConfigBase.h"
#include "ButcherProjectArea.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAreaConfigPreview
/////////////////////////////////
ButcherProjectAreaConfigPreview::ButcherProjectAreaConfigPreview(ButcherProjectAreaConfigBase *areaconfig,
    BLID_t viewid) :
    ButcherProjectBase(areaconfig->GetProject()), areaconfig_(areaconfig), viewid_(viewid),
    preview_(NULL), lastconfig_(NULL), lastarea_(), lasterror_(false)
{

}



ButcherProjectAreaConfigPreview::~ButcherProjectAreaConfigPreview()
{
    if (preview_) delete preview_;
    if (lastconfig_) delete lastconfig_;
}




bool ButcherProjectAreaConfigPreview::ConfigChanged()
{
    return
        (lastconfig_== NULL && !lasterror_) ||
        (!areaconfig_->Compare(*lastconfig_)) ||
        (lastarea_!=areaconfig_->GetArea()->GetGlobalRect());
}




ButcherImage *ButcherProjectAreaConfigPreview::GetPreview()
{
    if (ConfigChanged())
    {
        if (GetProject()->Views().Exists(viewid_) && NeedImage())
        {
            ButcherProjectBaseAutoProgress progress(this,
                wxString::Format(_("Generating preview for %s, please wait..."),
                    areaconfig_->GetArea()->GetDescription().c_str()));
            try
            {
                std::auto_ptr<ButcherImage> simage(GetProject()->Views()[viewid_]->CreateAreaImage(areaconfig_->GetArea()));
                if (HavePreview() && simage.get())
                    SetPreview(simage->Save(areaconfig_->ImageInfo().GetSaveParams()));
                else
                    SetPreview(simage.release());
            } catch(...) {
                //lasterror_=true;
                //throw;
                SetPreview(GetProject()->Views()[viewid_]->CreateAreaErrorImage(areaconfig_->GetArea()));
            }
        }
        else
            SetPreview(NULL);
    }
    return preview_;
}




void ButcherProjectAreaConfigPreview::SetPreview(ButcherImage *preview)
{
    if (preview_) delete preview_;
    preview_=preview;
    if (lastconfig_) delete lastconfig_;
    lastconfig_=new ButcherProjectAreaConfigBase(*areaconfig_);
    lastarea_=areaconfig_->GetArea()->GetGlobalRect();
    lasterror_=false;
}




bool ButcherProjectAreaConfigPreview::HavePreview()
{
    return
        (areaconfig_->GetAreaKind() == ButcherProjectAreaConfigBase::AK_IMAGE) ||
        (areaconfig_->GetBackground());
}




bool ButcherProjectAreaConfigPreview::NeedImage()
{
    return
        (HavePreview());
}


