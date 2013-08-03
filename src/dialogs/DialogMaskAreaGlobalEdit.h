//=============================================================================
/**
 *  @file    DialogMaskAreaGlobalEdit.h
 *
 *  $Id: DialogMaskAreaGlobalEdit.h,v 1.7 2008/02/06 16:40:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-08
 */
//=============================================================================
#ifndef __DIALOGMASKAERAGLOBALEDIT_H__
#define __DIALOGMASKAREAGLOBALEDIT_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

class HTMLButcherMaskAreaGlobalEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMaskAreaGlobalEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Area"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );
    virtual ~HTMLButcherMaskAreaGlobalEditDialog();

    void Init();
    void CreateControls();

    //void Load(ButcherProjectArea *loadarea, ButcherProjectAreaConfigBase *loadconfig);
    //void Save(ButcherProjectArea *loadarea, ButcherProjectAreaConfigBase *loadconfig);

    void Load(ButcherProjectView *view, ButcherProjectArea *area);
    void Save(ButcherProjectView *view, ButcherProjectArea *area);

    const wxString &GetAreaName() { return areaname_; }
    void SetAreaName(const wxString &areaname) { areaname_=areaname; }

    ButcherProjectAreaConfig::areakind_t GetAreaKind() { return areakind_; }
    void SetAreaKind(ButcherProjectAreaConfig::areakind_t areakind) { areakind_=areakind; }

    BLID_t GetImageFormat() { return imageformat_; }
    void SetImageFormat(BLID_t imageformat) { imageformat_=imageformat; }

    //void SetPreview(ButcherImage *preview) { previewctrl_->SetImage(preview); }

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    typedef map< ButcherProjectAreaConfig::areakind_t, int > DialogAreaMap;
    ButcherImageView *previewctrl_;

    void OnImageFormat(wxCommandEvent &event);
    void OnAlternateFileChanged(wxCommandEvent &event);

    void imageformats_load(BLID_t selid);
	void preview_load();

    DialogAreaMap areamap_;

    enum {
        ID_NAME = 1000,
        ID_KIND,
        ID_IMAGEFORMAT,
        ID_IMAGEFORMATDIALOG,
        ID_FILENAME,
        ID_FILENAMEPREPEND,
        ID_FILEALTERNATE
    };
private:
    wxString areaname_;
    ButcherProjectAreaConfig::areakind_t areakind_;
    BLID_t imageformat_;
    wxString filename_;
    bool filename_prepend_;
    int alternatefile_;

	ButcherProjectView *loadview_;
	ButcherProjectArea *loadarea_;
};

#endif //__DIALOGMASKAERAEDIT_H__

