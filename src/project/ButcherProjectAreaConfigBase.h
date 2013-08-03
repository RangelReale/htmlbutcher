//=============================================================================
/**
 *  @file   ButcherProjectAreaConfigBase.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTAREACONFIGBASE_H__
#define __BPROJECT_BUTCHERPROJECTAREACONFIGBASE_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherProjectConsts.h"
#include "BUtil.h"
#include "ButcherProjectFileLink.h"
#include "ButcherProjectAreaImageInfo.h"
#include "ButcherProjectAreaConfigPreview.h"
#include "ButcherProjectEventNotify.h"
#include "ButcherMetadataFile.h"

class ButcherProjectArea;

/**
 * @class ButcherProjectAreaConfigBase
 *
 * @brief Project area config base
 */
class ButcherProjectAreaConfigBase : public ButcherProjectBaseModify {
public:
    enum areakind_t { AK_NONE=0, AK_IMAGE, AK_MASK };
    enum align_t { AA_NONE=0, AA_LEFT, AA_CENTER, AA_RIGHT };
    enum valign_t { AV_NONE=0, AV_TOP, AV_MIDDLE, AV_BOTTOM };
    enum bgrepeat_t { BR_NONE=0, BR_HORIZONTAL, BR_VERTICAL, BR_ALL };
    enum imagesource_t { IS_AREA, IS_LINK };

    ButcherProjectAreaConfigBase(ButcherProjectArea *area, BLID_t id = 0);
    virtual ~ButcherProjectAreaConfigBase();

	bool GetIsDefault();

    areakind_t GetAreaKind() { return areakind_; }
    virtual void SetAreaKind(areakind_t k) { areakind_=k; ProjectModified(); }

	ButcherProjectConsts::layouttype_t GetLayoutType() { return layouttype_; }
	void SetLayoutType(ButcherProjectConsts::layouttype_t l) { layouttype_=l; ProjectModified(); }

    wxString GetAreaKindID();
    static wxString GetAreaKindID(areakind_t areakind);

    bool GetBackground() { return background_; }
    void SetBackground(bool b) { background_=b; ProjectModified(); }

    const wxString &GetContent() { return content_; }
    void SetContent(const wxString &c) { content_=c; ProjectModified(); }

    const wxString &GetCellTagAppend() { return celltagappend_; }
    void SetCellTagAppend(const wxString &c) { celltagappend_=c; ProjectModified(); }

    const wxString &GetImageURL() { return imageurl_; }
    void SetImageURL(const wxString &c) { imageurl_=c; ProjectModified(); }

    const wxString &GetImageURLTagAppend() { return imageurltagappend_; }
    void SetImageURLTagAppend(const wxString &c) { imageurltagappend_=c; ProjectModified(); }

    const wxString &GetImageTagAppend() { return imagetagappend_; }
    void SetImageTagAppend(const wxString &c) { imagetagappend_=c; ProjectModified(); }

    align_t GetAlign() { return align_; }
    void SetAlign(align_t a) { align_=a; ProjectModified(); }

    valign_t GetVAlign() { return valign_; }
    void SetVAlign(valign_t a) { valign_=a; ProjectModified(); }

    bgrepeat_t GetBGRepeat() { return bgrepeat_; }
    void SetBGRepeat(bgrepeat_t r) { bgrepeat_=r; ProjectModified(); }

    bool GetIsBGColor() { return isbgcolor_; }
    void SetIsBGColor(bool s) { isbgcolor_=s; ProjectModified(); }

    const wxColor &GetBGColor() { return bgcolor_; }
    void SetBGColor(const wxColor &c) { bgcolor_=c; ProjectModified(); }

    bool GetInnerScrollable() { return innerscrollable_; }
    void SetInnerScrollable(bool f) { innerscrollable_=f; ProjectModified(); }

    bool GetVariableSize() { return variablesize_; }
    void SetVariableSize(bool v) { variablesize_=v; ProjectModified(); }

    bool GetHaveMap() { return havemap_; }
    virtual void SetHaveMap(bool s) { havemap_=s; ProjectModified(); }

    const wxString &GetImageMapName() { return imagemapname_; }
    void SetImageMapName(const wxString &c) { imagemapname_=c; ProjectModified(); }

    int GetAlternateFile() { return alternatefile_; }
    void SetAlternateFile(int alternatefile) { alternatefile_=alternatefile; ProjectModified(); }

    imagesource_t GetImageSource() { return imagesource_; }
    void SetImageSource(imagesource_t is) { imagesource_=is; ProjectModified(); }

	const ButcherMargin &GetMargins() { return margins_; }
	ButcherMargin &GetMarginsModify() { return margins_; ProjectModified(); }

    ButcherProjectFileLink &ImageLink() { return imagelink_; }

    ButcherProjectArea *GetArea() { return area_; }

	wxRect GetGlobalRect();

    ButcherProjectAreaImageInfo &ImageInfo() { return imageinfo_; }
    ButcherProjectAreaImageInfo &AlternateImageInfo(int index) { if (index==-1) return imageinfo_; return *altimageinfo_[index]; }
    ButcherProjectAreaImageInfo &GetValidImageInfo(int alternatefile = -1);
    ButcherProjectAreaImageInfo &CurrentImageInfo();

    bool HaveAlternate();
    bool IsOutputImage();

    bool Compare(const ButcherProjectAreaConfigBase &other);
	virtual void Copy(const ButcherProjectAreaConfigBase &other);

    ButcherProjectAreaConfigPreview *GetPreview(BLID_t viewid);

    virtual wxString GetFlagsDescription();

    BLID_t GetBLId() { return blid_; }

    virtual void ProjectEvent(ButcherProjectEventNotify &event);

    virtual void LoadMetadata(ButcherMetadataData *metadata);
    virtual void SaveMetadata(ButcherMetadataData *metadata);
protected:
    void SetBLId(BLID_t b) { blid_=b; }

	virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);

    friend class ButcherProjectAreaConfigs;
private:
    void LoadMetadataAlternateImageInfo(ButcherMetadataDataList *metadatalist);
    void SaveMetadataAlternateImageInfo(ButcherMetadataDataList *metadatalist);

    ButcherProjectArea *area_;
    BLID_t blid_;
    areakind_t areakind_;
	ButcherProjectConsts::layouttype_t layouttype_;
    bool background_;
    wxString content_, celltagappend_, imageurl_, imageurltagappend_, imagetagappend_, imagemapname_;
    align_t align_;
    valign_t valign_;
    bgrepeat_t bgrepeat_;
    bool isbgcolor_;
    wxColor bgcolor_;
    bool innerscrollable_, variablesize_;
    bool havemap_;
    int alternatefile_;
    imagesource_t imagesource_;
    ButcherProjectFileLink imagelink_;
    ButcherProjectAreaImageInfo imageinfo_;
    deque< linked_ptr<ButcherProjectAreaImageInfo> > altimageinfo_;
	ButcherMargin margins_;
};

#endif // __BPROJECT_BUTCHERPROJECTAREACONFIGBASE_H__
