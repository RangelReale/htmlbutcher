//=============================================================================
/**
 *  @file   ButcherProjectLineEnd.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTLINEEND_H__
#define __BPROJECT_BUTCHERPROJECTLINEEND_H__

#include <wx/wx.h>
#include "ButcherProjectLine.h"

/**
 * @class ButcherProjectLineEnd
 *
 * @brief Project mask end line
 */
class ButcherProjectLineEnd : public ButcherProjectLine {
public:
    ButcherProjectLineEnd(ButcherProjectMaskContainer *container, ButcherProjectLine *start,
        ButcherProjectLine *end,
        orientation_t orientation) : ButcherProjectLine(container, start, end, orientation) {}

    virtual long GetPosition();
    virtual bool CanEdit() { return false; }
    virtual bool IsMoveable();
    virtual bool IsSave() { return false; }
    virtual ButcherProjectLineMinMax GetMoveMinMax();

    virtual wxString GetPosDescription();
private:
    virtual void SetPosition(long p);
};

#endif // __BPROJECT_BUTCHERPROJECTLINEEND_H__
