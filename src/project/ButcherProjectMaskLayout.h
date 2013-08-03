//=============================================================================
/**
 *  @file   ButcherProjectMaskLayout.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKLAYOUT_H__
#define __BPROJECT_BUTCHERPROJECTMASKLAYOUT_H__

#include <wx/wx.h>
#include <deque>
#include "ButcherProjectBase.h"
#include "ButcherProjectLine.h"

class ButcherProjectMask;

using namespace std;

/**
 * @class ButcherProjectMaskLayout
 *
 * @brief Project mask
 */
class ButcherProjectMaskLayout : public ButcherProjectBase
{
public:
    struct ButcherProjectMaskLayoutItem {
        BLID_t areaid;
        short colspan, rowspan;
        long forcewidth, forceheight;
        bool skipline;
    };


    ButcherProjectMaskLayout(ButcherProjectMask *mask, bool alignmentrow = true);

    unsigned long Count() { return static_cast<unsigned long>(list_.size()); }
    ButcherProjectMaskLayoutItem *Get(int index) { return &list_[index]; }
private:
    typedef map < long, deque<ButcherProjectLine*> > bldlines;

    void build();
    int bldlinespos(bldlines *ln, long position);


    ButcherProjectMask *mask_;
    deque< ButcherProjectMaskLayoutItem > list_;
    bldlines hlines_, vlines_;
    bool alignmentrow_;
};

#endif // __BPROJECT_BUTCHERPROJECTMASKLAYOUT_H__
