//=============================================================================
/**
 *  @file   ButcherProjectBaseAutoUpdate.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTBASEAUTOUPDATE_H__
#define __BPROJECT_BUTCHERPROJECTBASEAUTOUPDATE_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"

class ButcherProject;

/**
 * @class ButcherProjectBaseAutoUpdate
 *
 * @brief Project base auto-update class
 */
class ButcherProjectBaseAutoUpdate {
public:
    ButcherProjectBaseAutoUpdate(ButcherProjectBaseModify *project);
    ButcherProjectBaseAutoUpdate(ButcherProject *project);
    ~ButcherProjectBaseAutoUpdate() { release(); }

    void release();
    void abort() { active_=false; }
private:
    ButcherProject *project_;
    bool active_;
};

#endif // __BPROJECT_BUTCHERPROJECTBASEAUTOUPDATE_H__