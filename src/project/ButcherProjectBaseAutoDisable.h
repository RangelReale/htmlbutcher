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
#ifndef __BPROJECT_BUTCHERPROJECTBASEAUTODISABLE_H__
#define __BPROJECT_BUTCHERPROJECTBASEAUTODISABLE_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"

/**
 * @class ButcherProjectBaseAutoDisable
 *
 * @brief Project base auto-disable class
 */
class ButcherProjectBaseAutoDisable {
public:
    ButcherProjectBaseAutoDisable(ButcherProjectBaseModify *project);
    ButcherProjectBaseAutoDisable(ButcherProject *project);
    ~ButcherProjectBaseAutoDisable() { release(); }

    void release();
    void abort() { active_=false; }
private:
    ButcherProject *project_;
    bool active_;
};

#endif // __BPROJECT_BUTCHERPROJECTBASEAUTODISABLE_H__