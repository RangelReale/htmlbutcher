//=============================================================================
/**
 *  @file   ButcherOutputFile.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTBASEBLID_H__
#define __BPROJECT_BUTCHERPROJECTBASEBLID_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"

class ButcherProject;

/**
 * @class ButcherProjectBaseBLId
 *
 * @brief Project base class with id
 */
class ButcherProjectBaseBLId : public ButcherProjectBaseModify {
public:
    ButcherProjectBaseBLId(ButcherProject *project) :
        ButcherProjectBaseModify(project), blid_(0) {}

    BLID_t GetBLId() { return blid_; }

    virtual void ProjectModified() { ButcherProjectBaseModify::ProjectModified(blid_); }
protected:
    void SetBLId(BLID_t id) { blid_=id; }
private:
    BLID_t blid_;
};


#endif // __BPROJECT_BUTCHERPROJECTBASEBLID_H__