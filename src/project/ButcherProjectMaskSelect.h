//=============================================================================
/**
 *  @file   ButcherProjectMaskSelect.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKSELECT_H__
#define __BPROJECT_BUTCHERPROJECTMASKSELECT_H__

#include <wx/wx.h>
#include "ButcherProjectBase.h"
#include "ButcherProjectLine.h"
#include "ButcherProjectArea.h"

class ButcherProjectMask;

/**
 * @class ButcherProjectMaskSelect
 *
 * @brief Mask selection template
 */
template <class T>
class ButcherProjectMaskSelect : public ButcherProjectBase
{
private:
    struct maskselectdata_t {
        ButcherProjectMask *mask;
        T* item;
    };
    typedef deque<maskselectdata_t> maskselectlist_t;
public:
    ButcherProjectMaskSelect(ButcherProjectMask *mask);
    virtual ~ButcherProjectMaskSelect() {}

    void Add(ButcherProjectMask *mask, T* item) {
        maskselectdata_t d;
        d.mask=mask;
        d.item=item;
        list_.push_back(d);
    }

    unsigned long GetCount() { return static_cast<unsigned long>(list_.size()); }
    ButcherProjectMask* GetMask(int index) { return list_[index].mask; }
    T* GetItem(int index) { return list_[index].item; }
    bool Exists(T* item) {
        unsigned int i;
        for (i=0; i<list_.size(); i++)
            if (list_[i].item==item) return true;
        return false;
    }

    ButcherProjectMask *GetMask() { return mask_; }
private:

    ButcherProjectMask *mask_;
    maskselectlist_t list_;
};

/**
 * @class ButcherProjectMaskLineSelect
 *
 * @brief Line selection
 */
class ButcherProjectMaskLineSelect : public ButcherProjectMaskSelect<ButcherProjectLine>
{
public:
    ButcherProjectMaskLineSelect(ButcherProjectMask *mask) :
        ButcherProjectMaskSelect<ButcherProjectLine>(mask) {}
};

/**
 * @class ButcherProjectMaskAreaSelect
 *
 * @brief Area selection
 */
class ButcherProjectMaskAreaSelect : public ButcherProjectMaskSelect<ButcherProjectArea>
{
public:
    ButcherProjectMaskAreaSelect(ButcherProjectMask *mask) :
        ButcherProjectMaskSelect<ButcherProjectArea>(mask) {}
};

#endif // __BPROJECT_BUTCHERPROJECTMASKSELECT_H__
