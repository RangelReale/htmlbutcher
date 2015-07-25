//=============================================================================
/**
 *  @file    ButcherRuler.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERRULER_H__
#define __BVIEW_BUTCHERRULER_H__

#include <wx/wx.h>

#include <QWidget>

class ButcherView;

/**
 * @class ButcherRuler
 *
 * @brief Ruler control
 */
class ButcherRuler : public QWidget // wxControl 
{
	Q_OBJECT
public:
    enum rulerkind_t { BRK_TOP, BRK_LEFT, BRK_BOTTOM, BRK_RIGHT };

    ButcherRuler(ButcherView* parent, rulerkind_t kind/*, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxBORDER_NONE, const wxString& name = wxT("ButcherRuler")*/);

    void SetStart(unsigned int start) { start_=start; update(); }
    void SetSelection(long position);
private:
    void OnPaint(wxPaintEvent &event);
    void OnEraseBackground(wxEraseEvent &event);

    wxRect PosRect(long position, long spacing = 0);

    ButcherView* view_;
    rulerkind_t kind_;
    int linesmallpct_, linebigpct_;
    int start_;
    long position_;

    //DECLARE_EVENT_TABLE()
};

#endif // __BVIEW_BUTCHERRULER_H__