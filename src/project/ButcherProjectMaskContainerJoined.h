//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerJoined.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKCONTAINERJOINED_H__
#define __BPROJECT_BUTCHERPROJECTMASKCONTAINERJOINED_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectMaskContainer.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectLine.h"
#include "ButcherMetadataFile.h"

class ButcherProjectMask;

/**
 * @class ButcherProjectMaskContainerJoined
 *
 * @brief Project mask container joined
 */
class ButcherProjectMaskContainerJoined : public ButcherProjectMaskContainer {
public:
    typedef deque< ButcherProjectArea* > ButcherProjectModifyAreas;

    ButcherProjectMaskContainerJoined(ButcherProjectMask *mask,
        ButcherProjectArea::areaclass_t areaclass);
    ~ButcherProjectMaskContainerJoined();

    ButcherProjectLine *GetLineTop() { return line_top_; }
    ButcherProjectLine *GetLineLeft() { return line_left_; }
    ButcherProjectLine *GetLineBottom() { return line_bottom_; }
    ButcherProjectLine *GetLineRight() { return line_right_; }

    ButcherProjectLine *GetStart(ButcherProjectLine::orientation_t orientation);
    ButcherProjectLine *GetEnd(ButcherProjectLine::orientation_t orientation);

    virtual int GetMarginLeft() { return marginleft_; }
    virtual int GetMarginTop() { return margintop_; }
    virtual int GetMarginRight() { return marginright_; }
    virtual int GetMarginBottom() { return marginbottom_; }

    virtual long GetStartMinMaxPos(ButcherProjectLine::orientation_t orientation);
    virtual long GetEndMinMaxPos(ButcherProjectLine::orientation_t orientation);

    ButcherProjectArea *FirstArea();

    virtual ButcherProjectLine *AddLine(ButcherProjectLine *start, ButcherProjectLine *end,
        ButcherProjectLine::orientation_t orientation, unsigned long position,
        BLID_t id = 0);
    virtual bool DeleteLine(ButcherProjectLine *line);

    bool Check(ButcherProjectLine *start, ButcherProjectLine *end,
        ButcherProjectLine::orientation_t orientation, unsigned long position);
    static bool CheckConnect(ButcherProjectLine *start, ButcherProjectLine *end,
        ButcherProjectLine::orientation_t orientation, unsigned long position);

    virtual void Clear();

    bool LineAreas(ButcherProjectModifyAreas &areas, ButcherProjectLine *start,
        ButcherProjectLine *end,
        ButcherProjectLine::orientation_t orientation, long position);

    virtual void LoadMetadata(ButcherMetadataData *metadata);
    virtual void SaveMetadata(ButcherMetadataData *metadata);
protected:
    BLID_t InternalAddLine(ButcherProjectLine *line, BLID_t id = 0);
    BLID_t InternalRemoveLine(ButcherProjectLine *line, bool doremove = true);

    virtual void SetMarginLeft(long pos) { marginleft_=pos; }
    virtual void SetMarginTop(long pos) { margintop_=pos; }
    virtual void SetMarginRight(long pos) { marginright_=pos; }
    virtual void SetMarginBottom(long pos) { marginbottom_=pos; }

    void LoadMetadataLines(ButcherMetadataDataList *metadatalist);
    void LoadMetadataAreas(ButcherMetadataDataList *metadatalist);
    void SaveMetadataLines(ButcherMetadataDataList *metadatalist);
    void SaveMetadataAreas(ButcherMetadataDataList *metadatalist);
private:
    void Initialize();

    ButcherProjectLine *line_top_, *line_left_, *line_bottom_, *line_right_;
    int marginleft_, margintop_, marginright_, marginbottom_;
};


#endif // __BPROJECT_BUTCHERPROJECTMASKCONTAINERJOINED_H__