//=============================================================================
/**
 *  @file    DialogViewEdit.cpp
 *
 *  $Id: DialogViewEdit.h,v 1.17 2008/02/12 00:40:37 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-03
 */
//=============================================================================
#ifndef __DIALOGVIEWEDIT_H__
#define __DIALOGVIEWEDIT_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>
#include <cppcomp/wxccucontrol.h>
#include "BControls.h"

using namespace std;
using namespace cppcomp;

/**
 * @class HTMLButcherViewEditDialog
 *
 * @brief List wxClientData
 */
class HTMLButcherViewEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherViewEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("View"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(BLID_t viewid) { Load(GetProject()->Views().Get(viewid)); }
    void Save(BLID_t viewid) { Save(GetProject()->Views().Get(viewid)); }

    void Load(ButcherProjectView *view);
    void Save(ButcherProjectView *view);

    const wxString &GetViewName() { return viewname_; }
    unsigned int GetFileId() { return fileid_; }
    unsigned int GetMaskId() { return maskid_; }
    unsigned int GetMaskConfigId() { return maskconfigid_; }
protected:
    void OnBtnCSS(wxCommandEvent &event);

    void OnCtrlChanged(wxCommandEvent &event);

    wxccu_Control_ItemMap<ButcherHTMLConsts::encoding_t> encodingimap_;
    wxccu_Control_ItemMap<ButcherProjectAreaConfig::bgrepeat_t> bgrepeatimap_;
	wxccu_Control_ItemMap<ButcherProjectConsts::layouttype_t> layouttypeimap_;

    enum {
        ID_NAME = 1000,
		ID_LAYOUTTYPE,
        ID_FILE,
        ID_MASK,
        ID_IMAGEDIR,
        ID_ITEM_HTML,
        ID_ITEM_IMAGES,
        ID_ITEM_IMAGESGLOBAL,
        ID_FILENAME,
        ID_CSSFILE,
        ID_BODYTAGADD,
        ID_TABLETAGADD,
        ID_TITLE,
        ID_HEAD,
        ID_BODYPREPEND,
        ID_BODYAPPEND,
        ID_ENCODING,
        ID_ASSORTEDFILEGROUPS,
        ID_BGIMAGE,
        ID_BGREPEAT,
        ID_ISBGCOLOR,
        ID_ISSTRETCH,
        ID_BGCOLOR,
		ID_STYLESEPARATED,

        ID_BTN_CSS,

        ID_FILEALTERNATE // must be last
    };

    //DialogEncodingMap encmap_;

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    void UpdateAppState();

	wxString viewname_;
	ButcherProjectConsts::layouttype_t layouttype_;
    BLID_t fileid_, filealternateid_[BUTCHERCONST_VIEW_MAXALTERNATE], maskid_,
        maskconfigid_, imagefilepathid_, cssfileid_;
    int items_;
    wxString filename_, bodytagadd_, tabletagadd_, title_, head_, bodyprepend_, bodyappend_;
    ButcherHTMLConsts::encoding_t encoding_;
    ButcherIDList assortedfilegroups_;
    ButcherProjectFileLink bgimage_;
    ButcherProjectAreaConfig::bgrepeat_t bgrepeat_;
    bool isbgcolor_, isstretch_, styleseparated_;
    wxColor bgcolor_;
};

#endif //__DIALOGVIEWEDIT_H__
