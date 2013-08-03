//=============================================================================
/**
 *  @file   ButcherProjectBaseIDList.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTBASEIDLIST_H__
#define __BPROJECT_BUTCHERPROJECTBASEIDLIST_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"

/**
 * @class ButcherProjectBaseIDList
 *
 * @brief Project base ID list
 */
class ButcherProjectBaseIDList : public ButcherIDList
{
public:
    ButcherProjectBaseIDList(ButcherProjectBaseModify *projectbase) :
        ButcherIDList(), projectbase_(projectbase) {}

    virtual void Modified() { projectbase_->ProjectModified(); }
private:
    ButcherProjectBaseModify *projectbase_;
};

#endif // __BPROJECT_BUTCHERPROJECTBASEIDLIST_H__