//=============================================================================
/**
 *  @file   ButcherProjectLineDefault.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTLINEDEFAULT_H__
#define __BPROJECT_BUTCHERPROJECTLINEDEFAULT_H__

#include <wx/wx.h>
#include "ButcherProjectLine.h"

class ButcherProjectMaskContainer;

/**
 * @class ButcherProjectLineDefault
 *
 * @brief Project default line
 */
class ButcherProjectLineDefault : public ButcherProjectLine {
public:
    ButcherProjectLineDefault(ButcherProjectMaskContainer *container,
        ButcherProjectLine *start, ButcherProjectLine *end,
        orientation_t orientation,
        unsigned int position) throw();

    virtual long GetPosition() { return position_; }
private:
    virtual void SetPosition(long p) { position_=p; ProjectModified(); }

    long position_;
};

#endif // __BPROJECT_BUTCHERPROJECTLINEDEFAULT_H__