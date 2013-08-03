//=============================================================================
/**
 *  @file   ButcherProjectMaskContainer.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKCONTAINER_H__
#define __BPROJECT_BUTCHERPROJECTMASKCONTAINER_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectLine.h"
#include "ButcherProjectLines.h"
#include "ButcherProjectAreas.h"
#include "ButcherMetadataFile.h"

class ButcherProjectMask;

/**
 * @class ButcherProjectMaskContainer
 *
 * @brief Project mask container
 */
class ButcherProjectMaskContainer : public ButcherProjectBaseModify {
public:
    ButcherProjectMaskContainer(ButcherProjectMask *mask,
        ButcherProjectArea::areaclass_t areaclass);

    virtual int GetMarginLeft() { return 0; }
    virtual int GetMarginTop() { return 0; }
    virtual int GetMarginRight() { return 0; }
    virtual int GetMarginBottom() { return 0; }

    int GetMarginStart(ButcherProjectLine::orientation_t orientation);
    int GetMarginEnd(ButcherProjectLine::orientation_t orientation);

    virtual long GetStartMinMaxPos(ButcherProjectLine::orientation_t orientation);
    virtual long GetEndMinMaxPos(ButcherProjectLine::orientation_t orientation);

    int GetClientWidth();
    int GetClientHeight();

    virtual ButcherProjectLine *AddLine(ButcherProjectLine *start, ButcherProjectLine *end,
        ButcherProjectLine::orientation_t orientation, unsigned long position,
        BLID_t id = 0);
    virtual bool DeleteLine(ButcherProjectLine *line);

    virtual ButcherProjectArea *AddArea(long left, long top, long right, long bottom, BLID_t id = 0);
    virtual void DeleteArea(ButcherProjectArea *area);

    ButcherProjectArea *FindAreaByLines(BLID_t left, BLID_t top, BLID_t right, BLID_t bottom);

    virtual void Clear() {}
    bool IsEmpty() { return areas_.IsEmpty(); }

    ButcherProjectLines &Lines() { return lines_; }
    ButcherProjectAreas &Areas() { return areas_; }

    ButcherProjectMask *GetMask() { return mask_; }
    ButcherProjectArea::areaclass_t GetAreaClass() { return areaclass_; }
    ButcherProjectArea::areaclass_t GetContainedAreaClass();
    bool IsAreaSelect(ButcherProjectArea::areaselect_t areas);
    ButcherProjectArea::areaflags_t GetFlags();

    virtual void LoadMetadata(ButcherMetadataData *metadata);
    virtual void SaveMetadata(ButcherMetadataData *metadata);

    virtual void ProjectEvent(ButcherProjectEventNotify &event) {
        ButcherProjectBaseModify::ProjectEvent(event);

		for (ButcherProjectLines::iterator i=lines_.begin(); i!=lines_.end(); i++)
			i->ProjectEvent(event);
		for (ButcherProjectAreas::iterator i=areas_.begin(); i!=areas_.end(); i++)
			i->ProjectEvent(event);
    }
protected:
    virtual void SetMarginLeft(long pos) {}
    virtual void SetMarginTop(long pos) {}
    virtual void SetMarginRight(long pos)  {}
    virtual void SetMarginBottom(long pos)  {}

    virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
    virtual ButcherProjectArea::areaflags_t InternalGetFlags(ButcherProjectArea::areaflags_t curflags = 0);


    ButcherProjectLines lines_;
    ButcherProjectAreas areas_;

    friend class ButcherProjectLineStart;
    friend class ButcherProjectLineEnd;
private:
    ButcherProjectMask *mask_;
    ButcherProjectArea::areaclass_t areaclass_;
};

#endif // __BPROJECT_BUTCHERPROJECTMASKCONTAINER_H__
