//=============================================================================
/**
 *  @file    DialogMaskAreaEdit.h
 *
 *  $Id: DialogMaskAreaEdit.h,v 1.28 2008/02/12 00:40:37 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-12
 */
//=============================================================================
#ifndef __DIALOGMASKAERAEDIT_H__
#define __DIALOGMASKAREAEDIT_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cppcomp/wxccucontrol.h>
#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

using namespace cppcomp;

class HTMLButcherMaskAreaEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMaskAreaEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Area"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherProjectView *view, ButcherProjectArea *area);
    void Save(ButcherProjectView *view, ButcherProjectArea *area);

    const wxString &GetAreaName() { return areaname_; }

    //void SetPreview(ButcherImage *preview) { previewctrl_->SetImage(preview); }

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    ButcherImageView *previewctrl_;

    void OnImageFormat(wxCommandEvent &event);
    void OnSelectColor(ButcherImageViewColorEvent &event);

    void OnCtrlChanged(wxCommandEvent &event);
    void OnAlternateFileChanged(wxCommandEvent &event);

    void imageformats_load(BLID_t selid);
	void preview_load();

    wxccu_Control_ItemMap<ButcherProjectAreaConfig::areakind_t> areaimap_;
    wxccu_Control_ItemMap<ButcherProjectAreaConfig::align_t> alignimap_;
    wxccu_Control_ItemMap<ButcherProjectAreaConfig::valign_t> valignimap_;
    wxccu_Control_ItemMap<ButcherProjectAreaConfig::bgrepeat_t> bgrepeatimap_;
    wxccu_Control_ItemMap<ButcherProjectAreaConfig::imagesource_t> imagesourceimap_;
	wxccu_Control_ItemMap<ButcherProjectConsts::layouttype_t> layouttypeimap_;

    enum {
        ID_NAME = 1000,
        ID_KIND,
        ID_BACKGROUND,
		ID_LAYOUTTYPE,
        ID_IMAGEFORMAT,
        ID_IMAGEFORMATDIALOG,
        ID_HAVEIMAGEMAP,
        ID_IMAGEMAPNAME,
        ID_FILENAME,
        ID_FILENAMEPREPEND,

        ID_CONTENT,
        ID_CELLTAGAPPEND,
        ID_IMAGEURL,
        ID_IMAGEURLTAGAPPEND,
        ID_IMAGETAGAPPEND,
        ID_ALIGN,
        ID_VALIGN,
        ID_BGREPEAT,
        ID_ISBGCOLOR,
        ID_BGCOLOR,
        ID_TABLETAGAPPEND,
        ID_INNERSCROLLABLE,
        ID_VARIABLESIZE,
        ID_IMAGESOURCE,
        ID_IMAGELINK,
        ID_FILEALTERNATE,

        ID_MARGINLEFT,
        ID_MARGINTOP,
        ID_MARGINRIGHT,
        ID_MARGINBOTTOM,

        ID_PROC_HOVERCOLOR,
        ID_PROC_SELCOLOR
    };
private:
    void UpdateAppState();

    ButcherProject *project_;

    wxString areaname_;
    ButcherProjectAreaConfig::areakind_t areakind_;
    BLID_t imageformat_;
    bool background_;
	ButcherProjectConsts::layouttype_t layouttype_;
    wxString imagemapname_;
    bool haveimagemap_;
    wxString filename_;
    bool filename_prepend_;

    wxString content_, celltagappend_, imageurl_, imageurltagappend_, imagetagappend_;
    ButcherProjectAreaConfig::align_t align_;
    ButcherProjectAreaConfig::valign_t valign_;
    ButcherProjectAreaConfig::bgrepeat_t bgrepeat_;
    bool isbgcolor_;
    wxColor bgcolor_;
    wxString tabletagappend_;
    bool innerscrollable_, variablesize_;
    int alternatefile_;
    ButcherProjectAreaConfig::imagesource_t imagesource_;
    ButcherProjectFileLink imagelink_;
	ButcherMargin margins_;

	ButcherProjectView *loadview_;
	ButcherProjectArea *loadarea_;
};

#endif //__DIALOGMASKAERAEDIT_H__

