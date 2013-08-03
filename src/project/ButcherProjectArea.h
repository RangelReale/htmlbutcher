//=============================================================================
/**
 *  @file   ButcherProjectArea.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTAREA_H__
#define __BPROJECT_BUTCHERPROJECTAREA_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseBLId.h"
#include "ButcherProjectLine.h"
#include "ButcherProjectAreaConfigs.h"
#include "ButcherProjectColorScheme.h"
#include "ButcherMetadataFile.h"

class ButcherProjectMaskContainer;
class ButcherProjectMaskContainerJoined;
class ButcherProjectMaskContainerSplit;
class ButcherProjectMask;

/**
 * @class ButcherProjectArea
 *
 * @brief Project area base
 */
class ButcherProjectArea : public ButcherProjectBaseBLId
{
public:
    enum areaclass_t { AC_DEFAULT, AC_GLOBAL, AC_MAP };
    enum areaselectflag_t { AS_NONE=0, AS_ALL=1, AS_AREA=2, AS_AREAGLOBAL=4, AS_AREAMAP=8 };
    typedef unsigned short areaselect_t;
    enum areaflag_t { AF_HAVEIMAGE=1 };
    typedef unsigned short areaflags_t;

    ButcherProjectArea(ButcherProjectMaskContainer *container,
        ButcherProjectLine *start1, ButcherProjectLine *start2,
        ButcherProjectLine *end1, ButcherProjectLine *end2);
    virtual ~ButcherProjectArea();

    wxRect GetRect();
    wxRect GetGlobalRect();

    bool Select(long x, long y);
    bool Select(wxPoint pos);

    ButcherProjectLine *GetLeft() { return left_; }
    ButcherProjectLine *GetTop() { return top_; }
    ButcherProjectLine *GetRight() { return right_; }
    ButcherProjectLine *GetBottom() { return bottom_; }

    bool CanMove(long left = -1, long top = -1);
    void Move(long left = -1, long top = -1);
    bool CanResize(long width = -1, long height = -1);
    void Resize(long width = -1, long height = -1);

    bool IsFirst();

    long GetWidth();
    long GetHeight();
    long GetSize(ButcherProjectLine::orientation_t orientation);

    unsigned short GetLineCount() { return 4; }
    ButcherProjectLine *GetLine(unsigned short l);

    ButcherProjectLine *GetStart(ButcherProjectLine::orientation_t orientation);
    ButcherProjectLine *GetEnd(ButcherProjectLine::orientation_t orientation);

    long GetStartMinMaxPos(ButcherProjectLine::orientation_t orientation);
    long GetEndMinMaxPos(ButcherProjectLine::orientation_t orientation);

    bool Crossable(ButcherProjectLine::orientation_t orientation, long position);

    const wxString &GetName() { return name_; }
    void SetName(const wxString &name) { name_=name; ProjectModified(); }

    ButcherProjectAreaConfigs &Configs() { return *configs_; }

    ButcherProjectMaskContainer *GetContainer() { return container_; }
    ButcherProjectMaskContainerJoined *GetContainerJ();
    ButcherProjectMaskContainerSplit *GetContainerS();

    ButcherProjectMask *GetMask();
    virtual ButcherProjectMask *GetMaskRoot();
    ButcherProjectArea *GetParentRoot();

    virtual wxString GetDescription();
    wxString GetFullDescription();
    virtual wxString GetBaseFilename(int alternatefile = -1);
    wxString GetImageFilename(BLID_t viewid = 0, bool fileext = true, int alternatefile = -1);
	bool Customizable(BLID_t viewid);
	bool Editable(BLID_t viewid);

    virtual ButcherProjectColorScheme::coloritem_t GetDefaultColorItem();
    virtual areaclass_t GetAreaClass();
    areaclass_t GetContainedAreaClass();
    bool IsAreaSelect(areaselect_t areas);
    ButcherProjectArea::areaflags_t GetFlags();

    virtual void LoadMetadata(ButcherMetadataData *metadata);
    virtual void SaveMetadata(ButcherMetadataData *metadata);

    virtual void ProjectEvent(ButcherProjectEventNotify &event);
protected:
    void SetStart(ButcherProjectLine *line);
    void SetEnd(ButcherProjectLine *line);

    bool CanMove(ButcherProjectLine::orientation_t orientation,
        long pos, bool domove = false);
    void Move(ButcherProjectLine::orientation_t orientation, long pos);
    bool CanResize(ButcherProjectLine::orientation_t orientation,
        long size, bool doresize = false);
    void Resize(ButcherProjectLine::orientation_t orientation, long size);

    virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
private:
    ButcherProjectMaskContainer *container_;

    ButcherProjectLine *left_, *top_, *right_, *bottom_;

    wxString name_;
    ButcherProjectAreaConfigs *configs_;

    friend class ButcherProjectMaskLines;
    friend class ButcherList<ButcherProjectArea>;
    friend class ButcherProjectMaskContainerJoined;
};

#endif // __BPROJECT_BUTCHERPROJECTAREA_H__
