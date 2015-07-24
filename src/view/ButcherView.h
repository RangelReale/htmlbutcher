//=============================================================================
/**
 *  @file    ButcherView.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERVIEW_H__
#define __BVIEW_BUTCHERVIEW_H__

#include <wx/wx.h>
#include "ButcherViewBase.h"
#include "BList.h"
#include "ButcherViewChangedEvent.h"
#include "ButcherDocumentDrawEvent.h"
#include "ButcherProjectEvent.h"
#include "ButcherProjectArea.h"

#include <QWidget>

class ButcherView;
class ButcherRuler;
class ButcherProject;
class ButcherProjectView;
class ButcherProjectFile;
class ButcherProjectMaskDrawSelection;


/**
 * @class ButcherView
 *
 * @brief Base scrolling window
 */

#ifdef QT_HIDE_FROM

DECLARE_EVENT_TYPE( wxEVT_BUTCHEROPERATION_ACTION, -1 )

#define EVT_BUTCHEROPERATION(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHEROPERATION_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxCommandEventFunction, & fn ), (wxObject *) NULL ),

DECLARE_EVENT_TYPE( wxEVT_BUTCHERSCROLL_ACTION, -1 )

#define EVT_BUTCHERSCROLL(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_BUTCHERSCROLL_ACTION, id, -1, \
    (wxObjectEventFunction) (wxEventFunction) \
    wxStaticCastEvent( wxScrollWinEventFunction, & fn ), (wxObject *) NULL ),

#endif // QT_HIDE_FROM

class ButcherView : public QWidget, public ButcherViewBase {
	Q_OBJECT
public:
    enum areaviewitem_t { AV_AREA=1, AV_AREAGLOBAL=2, AV_AREAMAP=4 };
    typedef unsigned short areaview_t;

	/*
    ButcherView(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxBORDER_NONE,
        const wxString& name = wxT("ButcherView"));
	*/
	ButcherView(QWidget *parent);
    virtual ~ButcherView();

    virtual ButcherProject *GetProject() = 0;

    virtual BLID_t GetProjectViewId() = 0;
    ButcherProjectView *GetProjectView();
    ButcherProjectFile *GetProjectFile();

    virtual unsigned short GetZoom() { return 100; }

    virtual unsigned int GetDesignWidth() { return 0; }
    virtual unsigned int GetDesignHeight() { return 0; }

    virtual bool GetShowRulers() { return false; }
    virtual unsigned int GetRulerSize() { return 0; }

    virtual bool GetShowGrid() = 0;
    virtual int GetGridSize() = 0;

    virtual bool GetShowPreview() = 0;

    virtual bool GetShowBorders() = 0;
    virtual bool GetShowBordersCurrent() { return GetShowBorders(); }

    virtual areaview_t GetAreaView() = 0;
    virtual areaview_t GetAreaViewCurrent() { return GetAreaView(); }

    bool IsActive() { return GetProject() && GetProjectView(); }

    bool GetFileAlternate() { return filealternate_; }
    void SetFileAlternate(bool s, bool refresh = true);

    int GetFileAlternateId() { return filealternateid_; }
    void SetFileAlternateId(int s, bool refresh = true) { filealternateid_=s; if (refresh) Refresh(); }

    virtual void SelectionClear(bool refresh = true, bool hoveronly = false);

/*
    virtual wxCoord PosToClient(wxCoord p);
    virtual wxCoord ClientToPos(wxCoord p);
    virtual wxPoint PosToClient(wxPoint p, wxPoint add = wxPoint(0, 0));
    virtual wxPoint ClientToPos(wxPoint p, wxPoint add = wxPoint(0, 0));
    virtual wxRect PosToClient(wxRect r, wxPoint add = wxPoint(0, 0), wxSize addsize = wxSize(0, 0));
    virtual wxRect ClientToPos(wxRect r, wxPoint add = wxPoint(0, 0), wxSize addsize = wxSize(0, 0));
*/
    virtual float GetPixelSize();

    void ViewCenter(long x, long y);

    virtual void SetFocus();
protected:
    void RepositionView();
    void RepositionScroll(int orientation = wxBOTH);
    void RulerPosition(long x, long y);

    void DoChangedEvent(ButcherViewChangedEvent::change_t change, long movex = 0, long movey = 0);

    virtual void DoAfterDraw(ButcherDocumentDrawEvent& event) {}
    virtual void DoBeforeScroll() {}
    virtual void DoAfterScroll() {}

    ButcherDocument *designer_;
    wxScrolledWindow *dwindow_;
    ButcherProjectMaskDrawSelection *selection_;

    virtual void ProjectChanged();
private:

    void OnSize(wxSizeEvent &event);
    void OnDWindowWinScroll(wxScrollWinEvent &event);
    void OnDWindowRealWinScroll(wxScrollWinEvent &event);
    void OnSetFocus(wxFocusEvent &event);

    void OnProjectEvent(ButcherProjectEvent& event);
    void OnDrawDocument(ButcherDocumentDrawEvent& event);

    ButcherRuler *ruler_left_, *ruler_top_, *ruler_right_, *ruler_bottom_;
    bool filealternate_;
    int filealternateid_;

    //DECLARE_EVENT_TABLE()
};

ButcherProjectArea::areaselect_t ButcherView_AreaSelect(bool showareas, bool showareasglobal);

ButcherProjectArea::areaselect_t ButcherView_AreaSelect(ButcherView::areaview_t areaview);


#endif // __BVIEW_BUTCHERVIEW_H__
