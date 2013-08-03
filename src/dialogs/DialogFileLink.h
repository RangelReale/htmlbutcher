//=============================================================================
/**
 *  @file    DialogFileLink.h
 *
 *  $Id: DialogFileLink.h,v 1.7 2008/02/09 00:15:51 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-31
 */
//=============================================================================
#ifndef __DIALOGFILELINK_H__
#define __DIALOGFILELINK_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/treectrl.h>

#include "ButcherProject.h"
#include "ButcherProjectFileLink.h"
#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

// forward
class FileLinkTreeItemData;


class HTMLButcherFileLinkDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    enum seltype_t { ST_IMAGEONLY = 1, ST_ALLOWNONE = 2 };
    typedef int seltypes_t;

    HTMLButcherFileLinkDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("File Link"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    const ButcherProjectFileLink &GetFileLink() { return link_; }
    void SetLink(const ButcherProjectFileLink &link) { link_.SetBURL(link.GetBURL()); }
    void SetSelTypes(seltypes_t st) { seltypes_=st; }

    void ExpandLink(const ButcherProjectFileLink &link);
protected:
    void OnLinksExpanding(wxTreeEvent &event);
    void OnLinksChanged(wxTreeEvent &event);
    void OnButtonNone(wxCommandEvent &event);
    void OnPreviewEnable(wxCommandEvent &event);

    enum {
        ID_LINKS=1000,
        ID_PREVIEWPANEL,
        ID_PREVIEWENABLE,
        ID_PREVIEWFILENAME,

        ID_BTN_NONE
    };

    void UpdatePreview(const wxString &burl);
    void UpdatePreview(const ButcherProjectFileLink &link);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    void AddSubArea(wxTreeItemId parentitem, BLID_t viewid, ButcherProjectMaskContainer *container, bool isglobal = false);
    void ExpandItem(wxTreeItemId item);

    ButcherImageView *imageview_;

    wxTreeItemId FindTreeData(FileLinkTreeItemData *data);

    wxTreeItemId FindTreeData(wxTreeItemId root, FileLinkTreeItemData *data);

    ButcherProjectFileLink link_;
    seltypes_t seltypes_;

    wxString lastburl_;
};

#endif //__DIALOGFILELINK_H__
