//=============================================================================
/**
 *  @file   ButcherProjectMask.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASK_H__
#define __BPROJECT_BUTCHERPROJECTMASK_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectConsts.h"
#include "ButcherProjectBaseBLId.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectLine.h"
#include "ButcherViewBase.h"
#include "ButcherProjectMaskContainer.h"
#include "ButcherProjectMaskContainerArea.h"
#include "ButcherProjectMaskContainerAreaMap.h"
#include "ButcherProjectMaskContainerAreaGlobal.h"
#include "ButcherProjectMaskSelect.h"
#include "ButcherProjectMaskLayout.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"

class ButcherProject;
class ButcherProjectView;
class ButcherProjectMasks;
class ButcherProjectMaskDrawSelection;
class ButcherProjectMaskRoot;

/**
 * @class ButcherProjectMask
 *
 * @brief Project mask
 */
class ButcherProjectMask : public ButcherProjectBaseBLId {
public:
    typedef deque< ButcherProjectArea* > ButcherProjectModifyAreas;
    enum findline_t { FL_HORIZONTAL, FL_VERTICAL, FL_BOTH };
    enum drawmode_t { DM_DEFAULT, DM_NORMAL, DM_SELECTED, DM_HOVER };
    enum drawoption_t { DO_NONE=0, DO_DONTDRAWTITLES=1 };
    typedef int drawoptions_t;


    ButcherProjectMask(ButcherProject *project, ButcherProjectArea *parent,
        const wxString &name,
        int width = 0, int height = 0);
    virtual ~ButcherProjectMask();

	ButcherProjectConsts::layouttype_t GetCurrentLayoutType(BLID_t viewid);

    int GetWidth();
    int GetHeight();
    int GetSize(ButcherProjectLine::orientation_t orientation);
    const wxString &GetName() { return name_; }

    const wxString GetTableTagAdd() { return tabletagadd_; }
    void SetTableTagAdd(const wxString &t) { tabletagadd_=t; ProjectModified(); }

    long GetStartMinMaxPos(ButcherProjectLine::orientation_t orientation);
    long GetEndMinMaxPos(ButcherProjectLine::orientation_t orientation);

    wxPen *GetDrawPen(drawmode_t drawmode = DM_DEFAULT, ButcherProjectArea *area = NULL,
        ButcherProjectView *projectview = NULL, ButcherProjectMaskDrawSelection *selection = NULL);
    wxBrush *GetDrawBrush(drawmode_t drawmode = DM_DEFAULT, ButcherProjectArea *area = NULL,
        ButcherProjectMaskDrawSelection *selection = NULL);

    // returns false if the underlining image should be updated
    bool Draw(ButcherViewBase *view, wxDC *dc, ButcherProjectArea::areaselect_t areas = ButcherProjectArea::AS_ALL,
        ButcherProjectView *projectview = NULL, drawmode_t drawmode = DM_DEFAULT,
        ButcherProjectMaskDrawSelection *selection = NULL, drawoptions_t options = 0);
	void GeneratePreview(ButcherProjectArea::areaselect_t areas = ButcherProjectArea::AS_ALL,
        ButcherProjectView *projectview = NULL);
    void DrawPreview(ButcherViewBase *view, wxDC *dc, ButcherProjectArea::areaselect_t areas = ButcherProjectArea::AS_ALL,
        ButcherProjectView *projectview = NULL, drawoptions_t options = 0);

    void DrawLine(ButcherViewBase *view, wxDC *dc, ButcherProjectLine *line,
        ButcherProjectView *projectview = NULL, drawmode_t drawmode = DM_NORMAL,
        ButcherProjectMaskDrawSelection *selection = NULL);

    bool DrawArea(ButcherViewBase *view, wxDC *dc, ButcherProjectArea *area,
        ButcherProjectView *projectview = NULL, drawmode_t drawmode = DM_NORMAL,
        ButcherProjectMaskDrawSelection *selection = NULL,
        bool recurse = true,
        ButcherProjectArea::areaselect_t areas = ButcherProjectArea::AS_ALL, drawoptions_t options = 0);

    void GeneratePreviewArea(ButcherProjectArea *area,
        ButcherProjectView *projectview = NULL, bool recurse = true,
        ButcherProjectArea::areaselect_t areas = ButcherProjectArea::AS_ALL);
    void DrawPreviewArea(ButcherViewBase *view, wxDC *dc, ButcherProjectArea *area,
        ButcherProjectView *projectview = NULL, bool recurse = true,
        ButcherProjectArea::areaselect_t areas = ButcherProjectArea::AS_ALL, drawoptions_t options = 0);

    ButcherProjectMaskContainer *Container(ButcherProjectArea::areaclass_t areaclass);
    ButcherProjectMaskContainerArea *Areas();
    ButcherProjectMaskContainerAreaGlobal *AreasGlobal();
    ButcherProjectMaskContainerAreaMap *AreasMap();

    //ButcherProjectMaskConfigs &Configs();

    bool HaveArea();
	bool HaveVariableSizeArea(BLID_t viewid);

    bool FindLine(ButcherProjectArea::areaselect_t areas, findline_t type, long x, long y,
        ButcherProjectMaskLineSelect &select, BLID_t viewid = 0, int maxlevel = -1);
    bool FindArea(ButcherProjectArea::areaselect_t areas, long x, long y, ButcherProjectMaskAreaSelect &select,
        BLID_t viewid = 0, int maxlevel = -1);

    ButcherProjectMaskLayout *CreateLayout(bool alignmentrow) { return new ButcherProjectMaskLayout(this, alignmentrow); }

    ButcherProjectArea *GetParent() { return parent_; }
	ButcherProjectMask *GetMaskRoot();
    int GetLevel();

    long GetDrawStartX();
    long GetDrawStartY();
    long GetDrawStart(ButcherProjectLine::orientation_t orientation);
    wxPoint GetDrawStartPoint() { return wxPoint(GetDrawStartX(), GetDrawStartY()); }

    long GlobalToMask(ButcherProjectLine::orientation_t orientation, long position);
    long MaskToGlobal(ButcherProjectLine::orientation_t orientation, long position);
    wxPoint GlobalToMask(const wxPoint &position);
    wxPoint MaskToGlobal(const wxPoint &position);

    void LoadMetadata(ButcherMetadataData *metadata, int loadid = -1);
    void SaveMetadata(ButcherMetadataData *metadata);

    virtual void ProjectEvent(ButcherProjectEventNotify &event);
protected:
    ButcherProjectMask(ButcherProject *project);

    bool CheckSet(int width, int height);
    void Set(const wxString &name,
        int width, int height);

    void SetContainer(ButcherProjectArea::areaclass_t areaclass,
        ButcherProjectMaskContainer *container);

    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_MASKMODIFIED; }

    void LoadMetadataContainer(ButcherMetadataData *metadata);

    friend class ButcherProjectMasks;
    friend class ButcherList_1<ButcherProjectMaskRoot>;
    friend class ButcherProjectView;
private:
    typedef map< ButcherProjectArea::areaclass_t, ButcherProjectMaskContainer* > MaskContainers_t;

    ButcherProjectArea *parent_;
    wxString name_;
    int width_, height_;
    wxString tabletagadd_;

    //ButcherProjectMaskConfigs *configs_;
    MaskContainers_t containers_;
};

// implementation of ButcherProjectMaskSelect template
template <class T>
ButcherProjectMaskSelect<T>::ButcherProjectMaskSelect(ButcherProjectMask *mask) :
    ButcherProjectBase(mask->GetProject()), mask_(mask), list_()
{

}

#endif // __BPROJECT_BUTCHERPROJECTMASK_H__
