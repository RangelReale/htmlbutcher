//=============================================================================
/**
 *  @file   ButcherProjectAreaConfigs.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTAREACONFIGS_H__
#define __BPROJECT_BUTCHERPROJECTAREACONFIGS_H__

#include <wx/wx.h>
#include <memory>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectAreaConfig.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEventNotify.h"

class ButcherProjectArea;

/**
 * @class ButcherProjectAreaConfigs
 *
 * @brief Project area config list
 */
class ButcherProjectAreaConfigs : public ButcherProjectBaseModify
{
public:
	static const BLID_t DEFAULT_CONFIG = 0;

    ButcherProjectAreaConfigs(ButcherProjectArea *area);

    ButcherProjectAreaConfig *Get(BLID_t id);
    void Add(BLID_t id);
    void Delete(BLID_t id);
    bool Exists(BLID_t id);
	bool IsCustomization(BLID_t id) { return (id>DEFAULT_CONFIG) && (Exists(id)); }

    ButcherProjectAreaConfig* operator[] (BLID_t id) {
        return Get(id);
    }

    void LoadMetadata(ButcherMetadataDataList *metadatalist);
    void SaveMetadata(ButcherMetadataDataList *metadatalist);

    virtual void ProjectEvent(ButcherProjectEventNotify &event);
protected:
    void Clear();

    virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
private:
    typedef map< BLID_t, std::shared_ptr<ButcherProjectAreaConfig> >  list_t;
    BLID_t Add(ButcherMetadataData *metadata);

    ButcherProjectArea *area_;
    list_t list_;
};

#endif // __BPROJECT_BUTCHERPROJECTAREACONFIGS_H__
