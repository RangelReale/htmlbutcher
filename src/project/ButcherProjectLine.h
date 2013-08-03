//=============================================================================
/**
 *  @file   ButcherProjectLine.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTLINE_H__
#define __BPROJECT_BUTCHERPROJECTLINE_H__

#include <wx/wx.h>
#include "ButcherProjectBaseBLId.h"
#include "ButcherProjectEvent.h"

class ButcherProjectMaskContainer;
class ButcherProjectMaskContainerJoined;
class ButcherProjectMaskContainerSplit;
class ButcherProjectMask;
class ButcherProjectArea;


/**
 * @class ButcherProjectLine
 *
 * @brief Project line
 */
typedef struct {
    long min, max;
} ButcherProjectLineMinMax;


class ButcherProjectLine : public ButcherProjectBaseBLId {
public:
    enum orientation_t { BPL_HORIZONTAL, BPL_VERTICAL };
    enum areapos_t { LAP_BEFORE, LAP_AFTER };

    ButcherProjectLine(ButcherProjectMaskContainer *container, ButcherProjectLine *start,
        ButcherProjectLine *end, orientation_t orientation);

    virtual ~ButcherProjectLine();

    ButcherProjectMaskContainer *GetContainer() { return container_; }
    ButcherProjectMaskContainerJoined *GetContainerJ();
    ButcherProjectMaskContainerSplit *GetContainerS();
    ButcherProjectMask *GetMask();
    ButcherProjectLine *GetStart() { return start_; }
    ButcherProjectLine *GetEnd() { return end_; }
    virtual long GetPosition() = 0;
    long GetGlobalPosition();
    long GetSize();
    orientation_t GetOrientation() { return orientation_; }
    orientation_t GetInvOrientation() { return InvOrientation(orientation_); }
    virtual bool CanEdit() { return true; }

    static orientation_t InvOrientation(orientation_t orientation) { return (orientation==BPL_HORIZONTAL?BPL_VERTICAL:BPL_HORIZONTAL); }

    bool Connectable(ButcherProjectLine *other, long position);
    ButcherProjectArea *FindArea(areapos_t pos, long position);
    ButcherProjectArea *FindJoinArea(areapos_t pos, ButcherProjectArea *basearea);
    bool Connected(ButcherProjectLine *other);

    virtual ButcherProjectLineMinMax GetMoveMinMax();
    virtual bool IsMoveable();
    bool IsResizeable();
    virtual bool IsSave() { return true; }
    bool CanMove(long position);
    void Move(long position);

    bool Select(long x, long y);

    virtual wxString GetPosDescription() { return wxEmptyString; }
    wxString GetOrientationDescription() { return (orientation_==BPL_HORIZONTAL?_("HOR"):_("VER")); }
    wxString GetDescription();
    wxString GetFullDescription();
protected:
    bool op_canmove(long position);

    virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
private:
    typedef set< ButcherProjectArea* > ButcherProjectLineAreas;

    virtual void SetPosition(long p);

    ButcherProjectMaskContainer *container_;

    ButcherProjectLine *start_, *end_;
    orientation_t orientation_;
    ButcherProjectLineAreas areas_before_, areas_after_;

    friend class ButcherProjectMaskLines;
    friend class ButcherProjectMask;
    friend class ButcherProjectMaskLayout;
    friend class ButcherList<ButcherProjectLine>;
    friend class ButcherProjectMaskContainerJoined;
    friend class ButcherProjectMaskContainerSplit;
};

#endif // __BPROJECT_BUTCHERPROJECTLINE_H__