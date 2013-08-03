//=============================================================================
/**
 *  @file   ButcherProjectOptions.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTOPTIONS_H__
#define __BPROJECT_BUTCHERPROJECTOPTIONS_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectConsts.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"

class ButcherProject;

/**
 * @class ButcherProjectOptions
 *
 * @brief Project Options
 */
class ButcherProjectOptions : public ButcherProjectBaseModify
{
public:
    ButcherProjectOptions(ButcherProject *project);

	ButcherProjectConsts::layouttype_t GetLayoutType() { return layouttype_; }
	void SetLayoutType(ButcherProjectConsts::layouttype_t l) { layouttype_=l; ProjectModified(); }

	void Reset();

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);
protected:
	virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_OPTIONSMODIFIED; }

	void do_reset();
private:
	ButcherProjectConsts::layouttype_t layouttype_;
};

#endif // __BPROJECT_BUTCHERPROJECTOPTIONS_H__
