//=============================================================================
/**
 *  @file    DialogMaskAreaImageFormat.h
 *
 *  $Id: DialogMaskAreaImageFormat.h,v 1.7 2008/02/07 20:27:27 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-28
 */
//=============================================================================
#ifndef __DIALOGMASKAREAIMAGEFORMAT_H__
#define __DIALOGMASKAREAIMAGEFORMAT_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/spinctrl.h>

#include "ButcherProject.h"
#include "ButcherProjectAreaImageInfo.h"
#include "ButcherProjectFile.h"
#include "BImage.h"
#include "BImageView.h"
#include "BControls.h"

class HTMLButcherMaskAreaImageFormatDialog: public ButcherControl_Dialog
{
    DECLARE_EVENT_TABLE()
public:
    HTMLButcherMaskAreaImageFormatDialog( wxWindow* parent,
        wxWindowID id,
        ButcherProject *project,
        const wxString& caption = _("Image Format"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    void Init();
    void CreateControls();

    void Load(ButcherProjectAreaImageInfo *imageinfo);
    void Save(ButcherProjectAreaImageInfo *imageinfo);

    void SetArea(ButcherProjectFile *file, ButcherProjectArea *area) {
        file_=file; area_=area;
    }
protected:
    void OnSelectColor(ButcherImageViewColorEvent &event);
    void OnImageFormatPreview(wxCommandEvent &event);
    void OnTColorDClick(wxCommandEvent &event);
    void OnTPixelDClick(wxCommandEvent &event);

    enum {
        ID_LEFT=1000,

        ID_IMAGEFORMAT,
        ID_TRANSPARENTSELECTION,
        ID_TRANSPARENTCOLOR,
        ID_TRANSPARENTPIXEL,

        ID_PREVIEW1,
        ID_PREVIEW2,
        ID_PREVIEW2FORMAT,

        ID_PROC_HOVERCOLOR,
        ID_PROC_SELCOLOR
    };

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    void AddTColor(const wxColour &color);
    void LoadTColors();
    void SaveTColors();

    void AddTPixel(const wxPoint &pixel);
    void LoadTPixels();
    void SaveTPixels();

    void UpdateDisplay(bool isfirst=false);

    ButcherImageView *preview1ctrl_, *preview2ctrl_;

    ButcherProjectFile *file_;
    ButcherProjectArea *area_;

    BLID_t imageformat_;
    ButcherImage::transparentcolors_t tcolors_;
    ButcherImage::transparentpixels_t tpixels_;
};

#endif //__DIALOGMASKAREAIMAGEFORMAT_H__

