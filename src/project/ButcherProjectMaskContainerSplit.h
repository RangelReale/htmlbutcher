//=============================================================================
/**
 *  @file   ButcherProjectMaskContainerSplit.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTMASKCONTAINERSPLIT_H__
#define __BPROJECT_BUTCHERPROJECTMASKCONTAINERSPLIT_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectMaskContainer.h"
#include "ButcherProjectArea.h"
#include "ButcherMetadataFile.h"

class ButcherProjectMask;

/**
 * @class ButcherProjectMaskContainerSplit
 *
 * @brief Project mask container split
 */
class ButcherProjectMaskContainerSplit : public ButcherProjectMaskContainer {
public:
    ButcherProjectMaskContainerSplit(ButcherProjectMask *mask,
        ButcherProjectArea::areaclass_t areaclass);

    virtual ButcherProjectArea *AddArea(long left, long top, long right, long bottom, BLID_t id = 0);
    virtual void DeleteArea(ButcherProjectArea *area);

    virtual void Clear();

    virtual void LoadMetadata(ButcherMetadataData *metadata);
    virtual void SaveMetadata(ButcherMetadataData *metadata);
protected:
    void LoadMetadataAreas(ButcherMetadataDataList *metadatalist);
    void SaveMetadataAreas(ButcherMetadataDataList *metadatalist);
};

#endif // __BPROJECT_BUTCHERPROJECTMASKCONTAINERSPLIT_H__