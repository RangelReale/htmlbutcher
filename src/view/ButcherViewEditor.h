//=============================================================================
/**
 *  @file    ButcherViewEditor.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERVIEWEDITOR_H__
#define __BVIEW_BUTCHERVIEWEDITOR_H__

#include <wx/wx.h>
#include "ButcherDocumentMouseEvent.h"
#include "ButcherDocumentKeyEvent.h"
#include "ButcherViewSelectEvent.h"
#include "ButcherProjectEvent.h"
#include "ButcherProjectMaskSelect.h"
#include "ButcherView.h"
#include "ButcherStatusEvent.h"

#include <QWidget>

/**
 * @class ButcherViewEditor
 *
 * @brief Butcher main editor class
 */
class ButcherViewEditor : public ButcherView 
{
	Q_OBJECT
public:
    enum mode_t { MODE_DEFAULT, MODE_SELECTPOSITION, MODE_SELECTLINE,
        MODE_SELECTHLINE, MODE_SELECTVLINE, MODE_SELECTAREA, MODE_SELECTAREAGLOBAL, MODE_SELECTAREAMAP,
        MODE_SELECTHLINE_OPERATION, MODE_SELECTVLINE_OPERATION };
    enum modeevent_t { ME_MOUSELDOWN, ME_MOUSELMOVE, ME_MOUSELUP, ME_MOUSEHOVER, ME_MOUSEMDOWN, ME_MOUSERDOWN };
    enum operation_t { OP_NONE, OP_INSERTVLINE, OP_INSERTHLINE, OP_DELETELINE, OP_MOVELINE,
        OP_INSERTAREAGLOBAL, OP_DELETEAREAGLOBAL, OP_INSERTAREAMAP, OP_DELETEAREAMAP};

    ButcherViewEditor(QWidget* parent/*, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL,
        const wxString& name = wxT("ButcherViewEditor")*/);
    virtual ~ButcherViewEditor();

    virtual ButcherProject *GetProject() { return project_; }
    void SetProject(ButcherProject *project);

    virtual BLID_t GetProjectViewId() { return projectviewid_; }
    void SetProjectViewId(BLID_t projectviewid);

    virtual unsigned short GetZoom() { return zoom_; }
    void SetZoom(unsigned short z);

    virtual unsigned int GetDesignWidth() { return designwidth_; }
    void SetDesignWidth(unsigned int w);
    virtual unsigned int GetDesignHeight() { return designheight_; }
    void SetDesignHeight(unsigned int h);
    void SetDesignSize(unsigned int w, unsigned int h);

    virtual bool GetShowRulers() { return showrulers_; }
    void SetShowRulers(bool s) { showrulers_=s; RepositionView(); }

    virtual unsigned int GetRulerSize() { return rulersize_; }

    virtual bool GetShowGrid() { return showgrid_; }
    void SetShowGrid(bool s) { showgrid_=s; update(); }
    virtual int GetGridSize() { return gridsize_; }
    void SetGridSize(int gs) { gridsize_=gs; if (showgrid_) update(); }

    virtual bool GetShowPreview() { return showpreview_; }
    void SetShowPreview(bool s);

    virtual bool GetShowBorders() { return showborders_; }
    virtual bool GetShowBordersCurrent() { if (operation_!=OP_NONE) return true; return showborders_; }
    void SetShowBorders(bool s) { showborders_=s; update(); }

    virtual areaview_t GetAreaView() { return areaview_; }
    virtual areaview_t GetAreaViewCurrent();
    void SetAreaView(areaview_t av) { areaview_=av; DoDrawHoverClear(); update(); }

    bool GetProjectViewShowAreas();
    bool GetProjectViewShowAreasGlobal();

    mode_t GetMode() { if (mode_==MODE_DEFAULT) return defaultmode_; return mode_; }

    mode_t GetDefaultMode() { return defaultmode_; }
    void SetDefaultMode(mode_t m) { defaultmode_=m; if (mode_==MODE_DEFAULT) { SelectionClear(); update(); } }

    //void HoverClear() { DoDrawHoverClear(); }

    void SelectionClear(bool refresh = true, bool hoveronly = false) { DoHoverClear(); ButcherView::SelectionClear(refresh, hoveronly); }

    operation_t GetOperation() { return operation_; }
    void StartOperation(operation_t operation);
    void StopOperation();

    // advanced
    void DoSelectEvent(ButcherViewSelectEvent::select_t sel, long x=0, long y=0,
        ButcherProjectMaskLineSelect *lineselect = NULL, ButcherProjectMaskAreaSelect *areaselect = NULL);

    ButcherProjectLine* DoMenuSelectLine(ButcherProjectMaskLineSelect *lines);
    ButcherProjectArea* DoMenuSelectArea(ButcherProjectMaskAreaSelect *areas);
protected:
    virtual void DoAfterDraw(ButcherDocumentDrawEvent& event);

    ButcherProjectMaskDrawSelection *GetSelection();
Q_SIGNALS:
	void OnViewDocumentMouse(const ButcherDocumentMouseEvent& event);
	void OnViewDocumentKey(const ButcherDocumentKeyEvent& event);
	void OnViewBViewSelect(const ButcherViewSelectEvent& event);
	void OnViewStatus(const ButcherStatusEvent &event);
private Q_SLOTS:
	void OnDocumentMouse(const ButcherDocumentMouseEvent& event);
	void OnDocumentKey(const ButcherDocumentKeyEvent& event);
	void OnProjectEvent(const ButcherProjectEvent& event);
private:
    void OnDWindowLeave(wxMouseEvent &event);

    void OnBViewSelect(const ButcherViewSelectEvent& event);

    virtual void ProjectChanged();
    void ProjectViewChanged();

    void SetMode(mode_t m) { mode_=m; update(); }

    void DoModeEvent(modeevent_t me, long x, long y);

    void DoModeEvent_SelectLine(modeevent_t me, long x, long y);
    void DoModeEvent_SelectArea(modeevent_t me, long x, long y);

    void DoStatusEvent(ButcherStatusEvent::status_t st, const wxString &message);
    void DoOperationEvent();

    void DoOperationStart(operation_t operation);

    // find AreaMap compatible selected area
    ButcherProjectArea* DoFindAreaMapSelectArea(ButcherProjectMaskAreaSelect *areas);

    void DoDrawBand(wxRect bandrect, bool clear = false);
    void InternalDrawBand(wxDC &dc, wxRect bandrect);
    void DoClearBand();
    void DoResetBand();

    void DrawHover(ButcherProjectMaskLineSelect *lineshover,
        ButcherProjectMaskLineSelect *newlineshover, bool hover,
        bool checkredraw = true);
    void DrawHover(ButcherProjectMaskAreaSelect *areashover,
        ButcherProjectMaskAreaSelect *newareashover, bool hover,
        bool checkredraw = true);

    void DoDrawHover(ButcherProjectMaskLineSelect *lineshover);
    void DoDrawHover(ButcherProjectMaskAreaSelect *areashover);

    void DoDrawHoverClear();
    void DoDrawHoverClear(ButcherProjectMaskLineSelect *lineshover);
    void DoDrawHoverClear(ButcherProjectMaskAreaSelect *areahover);
    void DoHoverClear();

    unsigned short zoom_;
    int rulersize_;
    unsigned int designwidth_, designheight_;
    bool showrulers_, showgrid_, showpreview_, showborders_, showareasglobal_, showareas_;
    int gridsize_;

    ButcherProject *project_;
    BLID_t projectviewid_;
    mode_t mode_, defaultmode_;
    operation_t operation_;

    BLID_t operationid_, operationid2_;
    int operationpos_, operationpos2_;
    wxPoint operationpt_;
    ButcherProjectLine *operationline_;
    ButcherProjectArea *operationarea_;
    ButcherProjectMaskLineSelect *lineshover_;
    ButcherProjectMaskAreaSelect *areashover_;
    wxRect band_;
    areaview_t areaview_;

    //DECLARE_EVENT_TABLE()
};

#endif // __BVIEW_BUTCHERVIEWEDITOR_H__
