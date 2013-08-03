//=============================================================================
/**
 *  @file   ButcherProjectBase.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTBASE_H__
#define __BPROJECT_BUTCHERPROJECTBASE_H__

#include "ButcherProjectEventNotify.h"

class ButcherProject;

/**
 * @class ButcherProjectBase
 *
 * @brief Project base class
 */
class ButcherProjectBase {
public:
    ButcherProjectBase(ButcherProject *project) : project_(project) {}
    virtual ~ButcherProjectBase() {}

    ButcherProject *GetProject() const { return project_; }

    virtual void ProjectEvent(ButcherProjectEventNotify &event) {}
private:
    ButcherProject *project_;
};

#endif // __BPROJECT_BUTCHERPROJECTBASE_H__