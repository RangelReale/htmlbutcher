//=============================================================================
/**
 *  @file   ButcherProjectBaseAutoProgress.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTAUTOPROGRESS_H__
#define __BPROJECT_BUTCHERPROJECTAUTOPROGRESS_H__

#include <wx/wx.h>
#include "ButcherProjectBase.h"

class ButcherProject;

/**
 * @class ButcherProjectBaseAutoProgress
 *
 * @brief Project base auto-progress class
 */
class ButcherProjectBaseAutoProgress {
public:
    ButcherProjectBaseAutoProgress(ButcherProjectBase *project,
        const wxString &description, unsigned short flags = 0);
    ButcherProjectBaseAutoProgress(ButcherProject *project,
        const wxString &description, unsigned short flags = 0);
    ~ButcherProjectBaseAutoProgress() { release(); }

    void release();
    void abort() { active_=false; }
    void SetProgress(int p = -1);
private:
    ButcherProject *project_;
    bool active_;
    unsigned long pid_;
};

#endif // __BPROJECT_BUTCHERPROJECTAUTOPROGRESS_H__