//=============================================================================
/**
 *  @file   ButcherProjectAreaConfigPreview.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTAREACONFIGPREVIEW_H__
#define __BPROJECT_BUTCHERPROJECTAREACONFIGPREVIEW_H__

#include <wx/wx.h>
#include "ButcherProjectBase.h"
#include "BImage.h"

class ButcherProjectAreaConfigBase;

/**
 * @class ButcherProjectAreaConfigPreview
 *
 * @brief Project area config preview
 */
class ButcherProjectAreaConfigPreview : public ButcherProjectBase {
public:
    ButcherProjectAreaConfigPreview(ButcherProjectAreaConfigBase *areaconfig,
        BLID_t viewid);
    ~ButcherProjectAreaConfigPreview();

    ButcherImage *GetPreview();
    bool ConfigChanged();
    bool HavePreview();
    bool NeedImage();
    bool IsError() { return lasterror_; }
    BLID_t GetViewId() { return viewid_; }
private:
    void SetPreview(ButcherImage *preview);

    ButcherProjectAreaConfigBase *areaconfig_;
    BLID_t viewid_;

    ButcherImage *preview_;
    ButcherProjectAreaConfigBase *lastconfig_;
    wxRect lastarea_;
    bool lasterror_;
};

#endif // __BPROJECT_BUTCHERPROJECTAREACONFIGPREVIEW_H__
