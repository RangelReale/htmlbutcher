//=============================================================================
/**
 *  @file    DialogMaskAreaFileAlternate.h
 *
 *  $Id: DialogMaskAreaFileAlternate.h,v 1.3 2008/02/05 17:35:50 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-29
 */
//=============================================================================
#ifndef __DIALOGMASKAERAFILEALTERNATE_H__
#define __DIALOGMASKAERAFILEALTERNATE_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

class HTMLButcherMaskAreaFileAlternateDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMaskAreaFileAlternateDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Alternate Files"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherProjectView *view, ButcherProjectArea *loadarea, ButcherProjectAreaConfig *loadconfig);
    void Save(ButcherProjectView *view, ButcherProjectArea *loadarea, ButcherProjectAreaConfig *loadconfig);

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
protected:
    void OnAlternateImageFormat(wxCommandEvent &event);
    void OnEnabledClick(wxCommandEvent &event);
    void OnLoadPreview(wxCommandEvent &event);

    void ProcessEnabled();

    void imageformats_load(int afileid = -1);


    enum {
        ID_FIST = 1000,
        ID_BTN_LOADPREVIEW,

        // MUST BE LAST
        ID_ALTERNATE_BEGIN,
        ID_ALTERNATE_PREVIEW,
        ID_ALTERNATE_ENABLED,
        ID_ALTERNATE_IMAGEFORMAT,
        ID_ALTERNATE_FILENAME,
        ID_ALTERNATE_FILENAMEPREPEND,
        ID_ALTERNATE_IMAGEFORMATBUTTON,
        ID_ALTERNATE_END
    };
private:
    ButcherProjectView *view_;
    ButcherProjectArea *area_;
    ButcherProjectAreaConfig *config_;

    deque< linked_ptr<ButcherProjectAreaImageInfo> > altimageinfo_;
};

#endif //__DIALOGMASKAERAFILEALTERNATE_H__

