//=============================================================================
/**
 *  @file   ButcherProjectMaskDrawSelection.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKDRAWSELECTION_H__
#define __BPROJECT_BUTCHERPROJECTMASKDRAWSELECTION_H__

#include <wx/wx.h>
#include "ButcherProjectBase.h"
#include "ButcherProjectMask.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectLine.h"
#include "ButcherProjectMaskSelect.h"
#include "ButcherViewBase.h"

class ButcherProjectView;

/**
 * @class ButcherProjectMaskDrawSelection
 *
 * @brief Mask draw selection
 */
class ButcherProjectMaskDrawSelection : public ButcherProjectBase
{
public:
    ButcherProjectMaskDrawSelection(ButcherProjectMask *mask);
    ~ButcherProjectMaskDrawSelection();

    void Clear();

    ButcherProjectMask *GetMask() { return mask_; }

    void Add(ButcherProjectMask::drawmode_t drawmode, ButcherProjectArea *area);
    void Add(ButcherProjectMask::drawmode_t drawmode, ButcherProjectLine *line);

    void Set(ButcherProjectMask::drawmode_t drawmode, ButcherProjectMaskAreaSelect *areaselect);
    void Set(ButcherProjectMask::drawmode_t drawmode, ButcherProjectMaskLineSelect *lineselect);

    ButcherProjectMask::drawmode_t GetDrawMode(ButcherProjectArea *area,
        ButcherProjectMask::drawmode_t dmdefault = ButcherProjectMask::DM_DEFAULT);
    ButcherProjectMask::drawmode_t GetDrawMode(ButcherProjectLine *line,
        ButcherProjectMask::drawmode_t dmdefault = ButcherProjectMask::DM_DEFAULT);

	void Draw(ButcherViewBase *view, wxDC *dc, ButcherProjectView *projectview = NULL, ButcherProjectMask::drawoptions_t options = 0);
protected:
    ButcherProjectMaskAreaSelect *GetAHover(bool recreate = false,
        bool set = false, ButcherProjectMaskAreaSelect *copy = NULL);
    ButcherProjectMaskAreaSelect *GetASelect(bool recreate = false,
        bool set = false, ButcherProjectMaskAreaSelect *copy = NULL);
    ButcherProjectMaskLineSelect *GetLHover(bool recreate = false,
        bool set = false, ButcherProjectMaskLineSelect *copy = NULL);
    ButcherProjectMaskLineSelect *GetLSelect(bool recreate = false,
        bool set = false, ButcherProjectMaskLineSelect *copy = NULL);
private:
    ButcherProjectMask *mask_;

    ButcherProjectMaskAreaSelect *ahover_, *aselect_;
    ButcherProjectMaskLineSelect *lhover_, *lselect_;
};

#endif // __BPROJECT_BUTCHERPROJECTMASKDRAWSELECTION_H__
