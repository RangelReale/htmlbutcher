//=============================================================================
/**
 *  @file    BImageView.h
 *
 *  $Id: BImageView.h,v 1.9 2008/02/10 16:26:48 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-08
 */
//=============================================================================
#ifndef __BIMAGEVIEW_H__
#define __BIMAGEVIEW_H__

#include <wx/wx.h>
#include "BExcept.h"
#include "BImage.h"

// forward declarations
class ButcherImageViewControl;

/**
 * @class ButcherImageViewColorEvent
 *
 * @brief Color event
 */
class ButcherImageViewColorEvent;
wxDECLARE_EVENT(wxEVT_BUTCHERIMAGEVIEWCOLOR_ACTION, ButcherImageViewColorEvent);

class ButcherImageViewColorEvent : public wxEvent
{
public:
    enum select_t { CS_HOVER, CS_SELECT, CS_CONFIRM };

    ButcherImageViewColorEvent(const wxColor &color, select_t select,
        const wxPoint &pixel,
        int id = 0, wxEventType commandType = wxEVT_BUTCHERIMAGEVIEWCOLOR_ACTION);

    // required for sending with wxPostEvent()
    virtual wxEvent* Clone() const;

    const wxColor &GetColor() { return color_; }
    select_t GetSelect() { return select_; }
    const wxPoint GetPixel() { return pixel_; }
private:
    wxColor color_;
    select_t select_;
    wxPoint pixel_;
};

typedef void (wxEvtHandler::*ButcherImageViewColorEventFunction)(ButcherImageViewColorEvent&);

#define ButcherImageViewColorEventHandler(func) \
	wxEVENT_HANDLER_CAST(ButcherImageViewColorEventFunction, func)

#define EVT_BUTCHERIMAGEVIEW_COLOR(id, fn) \
    wx__DECLARE_EVT1(wxEVT_BUTCHERIMAGEVIEWCOLOR_ACTION, id, ButcherImageViewColorEventHandler(fn))

/**
 * @class ButcherImageViewDrawEvent
 *
 * @brief image draw event
 */
class ButcherImageViewDrawEvent;
wxDECLARE_EVENT(wxEVT_BUTCHERIMAGEVIEWAFTERPAINT_ACTION, ButcherImageViewDrawEvent);

class ButcherImageViewDrawEvent : public wxEvent
{
public:
    ButcherImageViewDrawEvent(ButcherImageViewControl *control, wxDC *dc,
        const wxRegion &updateregion,
        int id = 0, wxEventType commandType = wxEVT_BUTCHERIMAGEVIEWAFTERPAINT_ACTION);

    // required for sending with wxPostEvent()
    virtual wxEvent* Clone() const;

    ButcherImageViewControl *GetControl() { return control_; }
    wxDC *GetDC() { return dc_; }
    const wxRegion &GetUpdateRegion() { return updateregion_; }
private:
    ButcherImageViewControl *control_;
    wxDC *dc_;
    wxRegion updateregion_;
};

typedef void (wxEvtHandler::*ButcherImageViewDrawEventFunction)(ButcherImageViewDrawEvent&);

#define ButcherImageViewDrawEventHandler(func) \
	wxEVENT_HANDLER_CAST(ButcherImageViewDrawEventFunction, func)

#define EVT_BUTCHERIMAGEVIEW_AFTERPAINT(id, fn) \
    wx__DECLARE_EVT1(wxEVT_BUTCHERIMAGEVIEWAFTERPAINT_ACTION, id, ButcherImageViewDrawEventHandler(fn))


/**
 * @class ButcherImageViewControl
 *
 * @brief Image view control
 */

wxDECLARE_EVENT(wxEVT_BUTCHERIMAGEVIEWMENU_ACTION, wxCommandEvent);

#define EVT_BUTCHERIMAGEVIEW_MENU(id, fn) \
    wx__DECLARE_EVT1(wxEVT_BUTCHERIMAGEVIEWMENU_ACTION, id, wxCommandEventHandler(fn))

class ButcherImageViewControl : public wxControl
{
public:
    ButcherImageViewControl(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxBORDER_NONE, const wxValidator &validator = wxDefaultValidator,
        const wxString& name = wxPanelNameStr);
    ~ButcherImageViewControl();

    bool GetFit() { return fit_; }
    void SetFit(bool f) { fit_=f; ImageChanged(); }

    bool GetSelectColor() { return selectcolor_; }
    void SetSelectColor(bool s);

    unsigned short GetZoom() { return zoom_; }
    void SetZoom(unsigned short z) { zoom_=z; ImageChanged(); }

    unsigned char GetRepeat() { return repeat_; }
    void SetRepeat(unsigned char r) { repeat_=r; ImageChanged(); }

    const wxString &GetEmptyTitle() { return emptytitle_; }
    void SetEmptyTitle(const wxString &t) { emptytitle_=t; ImageChanged(); }

    void SetImage(ButcherImage *image, bool ownimage = true);
    ButcherImage *GetImage() { return image_; }

    void SetSizeFromImage(int left = 0, int top = 0);

    wxRect GetDrawRect();
    wxPoint GetImagePixel(wxPoint position);
    wxColor GetImageColor(wxPoint imagepixel);
private:
    void OnErase(wxEraseEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnMouse(wxMouseEvent &event);
	void OnChildFocus(wxChildFocusEvent &event);

    void ReleaseImage();
    void ImageChanged();

    ButcherImage *image_;
    bool ownsimage_;
    bool fit_;
    bool selectcolor_;
    unsigned short zoom_;
    unsigned char repeat_;
    wxString emptytitle_;

    wxDECLARE_EVENT_TABLE();
};

/**
 * @class ButcherImageView
 *
 * @brief Image view display
 */

wxDECLARE_EVENT(wxEVT_BUTCHERIMAGEVIEWSAVEFILE_ACTION, wxCommandEvent);

#define EVT_BUTCHERIMAGEVIEW_SAVEFILE(id, fn) \
    wx__DECLARE_EVT1(wxEVT_BUTCHERIMAGEVIEWSAVEFILE_ACTION, id, wxCommandEventHandler(fn))

class ButcherImageView : public wxControl
{
public:
    ButcherImageView(wxWindow* parent, wxWindowID id,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxBORDER_NONE, const wxValidator &validator = wxDefaultValidator,
        const wxString& name = wxPanelNameStr);
    virtual ~ButcherImageView();

    bool GetFullSize() { return fullsize_; }
    void SetFullSize(bool f) { fullsize_=f; FullsizeChanged(); }

    bool GetSelectColor() { return view_->GetSelectColor(); }
    void SetSelectColor(bool s) { view_->SetSelectColor(s); }

    unsigned short GetZoom() { return view_->GetZoom(); }
    void SetZoom(unsigned short z) { view_->SetZoom(z); RepositionControls(); }

    unsigned char GetRepeat() { return view_->GetRepeat(); }
    void SetRepeat(unsigned char r) { view_->SetRepeat(r); RepositionControls(); }

    unsigned char GetMargin() { return margin_; }
    void SetMargin(unsigned int m) { margin_=m; RepositionControls(); }

    const wxString &GetEmptyTitle() { return view_->GetEmptyTitle(); }
    void SetEmptyTitle(const wxString &t) { view_->SetEmptyTitle(t); }

    void SetImage(ButcherImage *image, bool ownimage = true);
    ButcherImage *GetImage() { return view_->GetImage(); }

    wxColor GetBGColor() { return GetBackgroundColour(); }
    void SetBGColor(const wxColor &bg);

    ButcherImageViewControl *GetView() { return view_; }

    virtual wxSize DoGetBestSize() const;
private:
    void OnSize(wxSizeEvent &event);
    void OnViewMenu(wxCommandEvent &event);
    void OnMouseRight(wxMouseEvent &event);

    void ShowMenu();

    void FullsizeChanged();
    void RepositionControls();

    ButcherImageViewControl *view_;
    wxScrolledWindow *scr_;
    bool fullsize_;
    unsigned int margin_;

    wxDECLARE_EVENT_TABLE();
};

#endif //__BIMAGEVIEW_H__
