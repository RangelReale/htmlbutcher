//=============================================================================
/**
 *  @file   ButcherProjectView.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTVIEW_H__
#define __BPROJECT_BUTCHERPROJECTVIEW_H__

#include <wx/wx.h>
#include "BConsts.h"
#include "BList.h"
#include "ButcherProjectBaseBLId.h"
#include "ButcherProjectFile.h"
#include "ButcherProjectMask.h"
#include "BImage.h"
#include "ButcherProjectConsts.h"
#include "ButcherProjectFileLink.h"
#include "ButcherProjectAreaConfig.h"
#include "ButcherProjectBaseIDList.h"
#include "ButcherViewBase.h"
#include "ButcherProjectViewFileCheck.h"
#include "ButcherProjectArea.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"
#include "ButcherProjectBaseAutoProgress.h"
#include "ButcherProjectViews.h"
#include "BHTML.h"

#include <memory>

class ButcherProject;

using namespace std;

/**
 * @class ButcherProjectView
 *
 * @brief Project view
 */
class ButcherProjectView : public ButcherProjectBaseBLId {
public:
    enum items_t { ITEM_HTML = 1, ITEM_IMAGES = 2, ITEM_IMAGESGLOBAL = 4 };

    ButcherProjectView(ButcherProject *project, const wxString &name,
        BLID_t fileid, BLID_t maskid);
    virtual ~ButcherProjectView();

    bool HaveFileAlternate();

    ButcherProjectFile *GetFile();
    ButcherProjectFile *GetFileAlternate(int i);
    ButcherProjectFile *GetFileCurrent(int alternatefile = -1);
    ButcherProjectMask *GetMask();
    //ButcherProjectMaskConfig *GetMaskConfig();

    ButcherImage *CreateAreaImage(ButcherProjectArea *area, int alternate = -1);
    ButcherImage *CreateAreaImageDefault(ButcherProjectArea *area, int alternate = -1);
    ButcherImage *CreateAreaErrorImage(ButcherProjectArea *area);

    void Set(const wxString &name,
        BLID_t fileid, BLID_t maskid, int items)
        { name_=name; fileid_=fileid; maskid_=maskid; items_=items;
            ProjectModified(); }

    const wxString &GetName() { return name_; }
    void SetName(const wxString &n) { name_=n; ProjectModified(); }

	ButcherProjectConsts::layouttype_t GetLayoutType() { return layouttype_; }
	void SetLayoutType(ButcherProjectConsts::layouttype_t l) { layouttype_=l; ProjectModified(); }

    BLID_t GetFileId() { return fileid_; }
    void SetFileId(BLID_t i) { fileid_=i; ProjectModified(); }

    BLID_t GetMaskId() { return maskid_; }
    void SetMaskId(BLID_t i) { maskid_=i; ProjectModified(); }

    BLID_t GetCOMPATMaskConfigId() { return COMPATmaskconfigid_; }
    //void SetMaskConfigId(BLID_t i) { maskconfigid_=i; ProjectModified(); }

    int GetItems() { return items_;}
    void SetItems(int i) { items_=i; ProjectModified(); }

    BLID_t GetImageFilePathId() { return imagefilepathid_; }
    void SetImageFilePathId(BLID_t i) { imagefilepathid_=i; ProjectModified(); }

    wxString GetImageDir(const wxString &basepath = wxEmptyString, bool formatted = false);

    const wxString &GetFilename() { return filename_; }
    void SetFilename(const wxString &f) { filename_=f; ProjectModified(); }

    BLID_t GetCSSFileId() { return cssfileid_; }
    void SetCSSFileId(BLID_t i) { cssfileid_=i; ProjectModified(); }

    const wxString &GetBodyTagAdd() { return bodytagadd_; }
    void SetBodyTagAdd(const wxString &f) { bodytagadd_=f; ProjectModified(); }

    const wxString &GetTableTagAdd() { return tabletagadd_; }
    void SetTableTagAdd(const wxString &f) { tabletagadd_=f; ProjectModified(); }

    const wxString &GetTitle() { return title_; }
    void SetTitle(const wxString &f) { title_=f; ProjectModified(); }

    const wxString &GetHead() { return head_; }
    void SetHead(const wxString &f) { head_=f; ProjectModified(); }

    const wxString &GetBodyPrepend() { return bodyprepend_; }
    void SetBodyPrepend(const wxString &f) { bodyprepend_=f; ProjectModified(); }

    const wxString &GetBodyAppend() { return bodyappend_; }
    void SetBodyAppend(const wxString &f) { bodyappend_=f; ProjectModified(); }

    ButcherHTMLConsts::encoding_t GetEncoding() { return encoding_; }
    void SetEncoding(ButcherHTMLConsts::encoding_t e) { encoding_=e; ProjectModified(); }

    ButcherProjectFileLink &BGImage() { return bgimage_; }

    ButcherProjectAreaConfig::bgrepeat_t GetBGRepeat() { return bgrepeat_; }
    void SetBGRepeat(ButcherProjectAreaConfig::bgrepeat_t r) { bgrepeat_=r; ProjectModified(); }

    bool GetIsBGColor() { return isbgcolor_; }
    void SetIsBGColor(bool s) { isbgcolor_=s; ProjectModified(); }

    const wxColor &GetBGColor() { return bgcolor_; }
    void SetBGColor(const wxColor &c) { bgcolor_=c; ProjectModified(); }

    void SetFileAlternateId(int i, BLID_t filealternateid) { filealternateid_[i]=filealternateid; ProjectModified(); }
    BLID_t GetFileAlternateId(int i) { return filealternateid_[i]; }

    bool GetIsStretch() { return isstretch_; }
    void SetIsStretch(bool s) { isstretch_=s; ProjectModified(); }

    bool GetStyleSeparated() { return styleseparated_; }
    void SetStyleSeparated(bool s) { styleseparated_=s; ProjectModified(); }

    ButcherProjectBaseIDList &AssortedFileGroups() { return assortedfilegroups_; }

    wxString GetBaseFilename();
    wxString GetDefaultFilename();

    void DrawPreview(ButcherViewBase *view, wxDC *dc);

    void CheckFile(ButcherProjectViewFileCheck &check);
    void GenerateHTML(const wxString &filename);
    void GenerateImages(const wxString &dirname, ButcherProjectMask* mask,
        ButcherProjectArea::areaselect_t areas, ButcherProjectArea::
        areaselect_t areasinner = ButcherProjectArea::AS_ALL);

    virtual void ProjectEvent(ButcherProjectEventNotify &event);

    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
protected:
    ButcherProjectView(ButcherProject *project);

    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_VIEWMODIFIED; }

    void do_genhtml(int level, ButcherHTMLGenerator &htmlgenerator, const wxString &dirname,
        ButcherProjectMask *mask, ButcherProjectBaseAutoProgress *progress = NULL);
    void do_checkfile(int level, ButcherProjectViewFileCheck &check, ButcherProjectMask *mask);
    void do_checkimages(ButcherProjectViewFileCheck &check, ButcherProjectMask* mask,
        ButcherProjectArea::areaselect_t areas, ButcherProjectArea::
        areaselect_t areasinner = ButcherProjectArea::AS_ALL);

    friend class ButcherProjectViews;
    friend class ButcherList_1<ButcherProjectView>;
private:
    wxString name_;
	ButcherProjectConsts::layouttype_t layouttype_;
    BLID_t fileid_, filealternateid_[BUTCHERCONST_VIEW_MAXALTERNATE];
    BLID_t maskid_, COMPATmaskconfigid_, imagefilepathid_, cssfileid_;
    int items_;
    wxString filename_, bodytagadd_, tabletagadd_, title_, head_, bodyprepend_, bodyappend_;
    ButcherHTMLConsts::encoding_t encoding_;
    ButcherProjectBaseIDList assortedfilegroups_;
    ButcherProjectFileLink bgimage_, previewbgimage_;
    ButcherProjectAreaConfig::bgrepeat_t bgrepeat_;
    bool isbgcolor_, isstretch_, styleseparated_;
    wxColor bgcolor_;

    auto_ptr<ButcherImage> previewbgimagedraw_;
};

#endif // __BPROJECT_BUTCHERPROJECTVIEW_H__
