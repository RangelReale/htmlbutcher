//=============================================================================
/**
 *  @file    DialogImageFormatEdit.h
 *
 *  $Id: DialogImageFormatEdit.h,v 1.7 2008/02/07 12:35:34 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-19
 */
//=============================================================================
#ifndef __DIALOGIMAGEFORMATEDIT_H__
#define __DIALOGIMAGEFORMATEDIT_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <map>
#include <cppcomp/wxccucontrol.h>
#include "BImage.h"
#include "BImageView.h"
#include "DialogListEdit.h"
#include "BControls.h"

using namespace std;
using namespace cppcomp;

/**
 * @class HTMLButcherImageFormatListEditDialog
 *
 * @brief Image format list
 */
class HTMLButcherImageFormatListEditDialog: public HTMLButcherListEditDialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherImageFormatListEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Image Formats"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX ):
        HTMLButcherListEditDialog(parent, id, project, caption, pos, size, style),
            preview_(NULL), previewformat_(NULL) {}
    virtual ~HTMLButcherImageFormatListEditDialog();

    void SetPreview(ButcherImage *preview);
protected:
    enum {
        ID_AUTOUPDATE = ID_DIALOGLISTMAX+1
    };

    virtual void do_load();

    virtual void do_select(BLID_t id);

    virtual void do_add();
    virtual void do_edit(unsigned long id);
    virtual void do_remove(unsigned long id);

    void do_createcontrols_preview();

    void UpdatePreview();
    void UpdatePreview(BLID_t id);

    void OnAutoUpdate(wxCommandEvent &event);
private:
    ButcherImage *preview_, *previewformat_;
    wxStaticText *previewlabel_;
    ButcherImageView *previewctrl_;
    wxSizer *previewsizer_;
};

/**
 * @class HTMLButcherImageFormatEditDialog
 *
 * @brief Image format
 */
class HTMLButcherImageFormatEditDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherImageFormatEditDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Image Format"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );
    virtual ~HTMLButcherImageFormatEditDialog();

    void Init();
    void CreateControls();

    void SetPreview(ButcherImage *preview);

    void SetFormatName(const wxString &formatname) { formatname_=formatname; }
    const wxString &GetFormatName() { return formatname_; }

    void SetFormat(ButcherImage::format_t format) { format_=format; }
    ButcherImage::format_t GetFormat() { return format_; }

    void SetFlags(int flags) { flags_=flags; }
    int GetFlags() { return flags_; }

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    void UpdatePreview();

    ButcherImage::format_t DialogFormat();
    int DialogFlags();

    void do_createcontrols_preview();

    void OnAutoUpdate(wxCommandEvent &event);
    void OnFormatChange(wxCommandEvent &event);

    void OnCtrlChanged(wxCommandEvent &event);
    void UpdateAppState();
private:
    wxccu_Control_ItemMap<ButcherImage::format_t> imageformatimap_;
    wxccu_Control_ItemMap<ButcherImage::flags_t> jpegcompressionimap_;
    wxccu_Control_ItemMap<ButcherImage::flags_t> quantizationimap_;
    wxccu_Control_ItemMap<ButcherImage::flags_t> forcebppimap_;

    enum {
        ID_NAME = 1000,
        ID_FORMAT,
        ID_FLAGS,

        ID_JPEGCOMPRESSION,
        ID_JPEGPROGRESSIVE,
        ID_QUANTIZATON,
        ID_FORCEBPP,

        ID_AUTOUPDATE
    };


    wxString formatname_;
    ButcherImage::format_t format_;
    int flags_;

    ButcherImage *preview_, *previewformat_;
    ButcherProject *project_;
    wxStaticText *previewlabel_;
    ButcherImageView *previewctrl_;

    wxSizer *previewsizer_, *bodysizer_;
};

#endif //__DIALOGIMAGEFORMATEDIT_H__
