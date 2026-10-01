//=============================================================================
/**
 *  @file    ButcherDocument.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERDOCUMENT_H__
#define __BVIEW_BUTCHERDOCUMENT_H__

#include <wx/wx.h>

class ButcherView;

/**
 * @class ButcherDocument
 *
 * @brief document control
 */

wxDECLARE_EVENT(wxEVT_BUTCHERDOCUMENTKEYBOARD_ACTION, wxKeyEvent);


#define EVT_BUTCHERDOCUMENTKEYBOARD(id, fn) \
    wx__DECLARE_EVT1(wxEVT_BUTCHERDOCUMENTKEYBOARD_ACTION, id, wxKeyEventHandler(fn))


class ButcherDocument : public wxControl {
public:
    ButcherDocument(ButcherView* view, wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxBORDER_NONE, const wxString& name = "ButcherDocument");

    ButcherView *GetView() { return view_; }

    void DrawGrid(wxDC *dc);
private:
    void OnPaint(wxPaintEvent &event);
    void OnEraseBackground(wxEraseEvent &event);
    void OnMouse(wxMouseEvent &event);
    void OnKeyboard(wxKeyEvent &event);
	void OnChildFocus(wxChildFocusEvent &event);

    ButcherView* view_;

    wxDECLARE_EVENT_TABLE();
};

#endif // __BVIEW_BUTCHERDOCUMENT_H__
