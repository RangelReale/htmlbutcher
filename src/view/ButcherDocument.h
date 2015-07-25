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

#include "ButcherDocumentDrawEvent.h"
#include "ButcherDocumentMouseEvent.h"
#include "ButcherDocumentKeyEvent.h"

#include <wx/wx.h>

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>

class ButcherView;

/**
 * @class ButcherDocument
 *
 * @brief document control
 */

#ifdef QT_HIDE_FROM

DECLARE_EVENT_TYPE( wxEVT_BUTCHERDOCUMENTKEYBOARD_ACTION, -1 )


#define EVT_BUTCHERDOCUMENTKEYBOARD(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERDOCUMENTKEYBOARD_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxKeyEventFunction, & fn ), (wxObject *) NULL ),

#endif // QT_HIDE_FROM


class ButcherDocument : public QWidget //wxControl 
{
	Q_OBJECT
public:
    ButcherDocument(ButcherView* view, QWidget *parent/*, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxBORDER_NONE, const wxString& name = wxT("ButcherDocument")*/);

    ButcherView *GetView() { return view_; }

    //void DrawGrid(wxDC *dc);
Q_SIGNALS:
	void documentDrawEvent(const ButcherDocumentDrawEvent &event);
	void documentMouseEvent(const ButcherDocumentMouseEvent &event);
	void documentKeyEvent(const ButcherDocumentKeyEvent &event);
private:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

#ifdef QT_HIDE_FROM
	void OnPaint(wxPaintEvent &event);
	void OnEraseBackground(wxEraseEvent &event);
    void OnMouse(wxMouseEvent &event);
    void OnKeyboard(wxKeyEvent &event);
	void OnChildFocus(wxChildFocusEvent &event);
#endif // QT_HIDE_FROM

    ButcherView* view_;

    //DECLARE_EVENT_TABLE()
};

#endif // __BVIEW_BUTCHERDOCUMENT_H__
