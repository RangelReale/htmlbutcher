//=============================================================================
/**
 *  @file    ButcherViewDisplay.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================
#ifndef __BVIEW_BUTCHERVIEWDISPLAY_H__
#define __BVIEW_BUTCHERVIEWDISPLAY_H__

#include <wx/wx.h>
#include "ButcherView.h"

/**
 * @class ButcherViewDisplay
 *
 * @brief Butcher display
 */
class ButcherViewDisplay : public ButcherView {
public:
    ButcherViewDisplay(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL,
        const wxString& name = wxT("ButcherViewDisplay"));
    virtual ~ButcherViewDisplay();

    ButcherView *GetParentView() { return parentview_; }
    void SetParentView(ButcherView *pv);

    virtual ButcherProject *GetProject();

    virtual BLID_t GetProjectViewId();

    //virtual bool GetShowRulers() { return false; }
    //virtual unsigned int GetRulerSize() { return 15; }

    virtual unsigned short GetZoom() { return zoom_; }
    void SetZoom(unsigned short z);

    virtual unsigned int GetDesignWidth();
    virtual unsigned int GetDesignHeight();

    virtual bool GetShowGrid() { return false; }
    virtual int GetGridSize() { return 0; }

    virtual bool GetShowPreview() { return false; }

    virtual areaview_t GetAreaView() { return AV_AREA|AV_AREAGLOBAL|AV_AREAMAP; }

    virtual bool GetShowBorders() { return true; }
private:
    void DrawTarget(long x, long y, wxDC *dc = NULL);
    void ParentViewChanged();

    void OnViewChanged(ButcherViewChangedEvent &event);

    ButcherView *parentview_;
    unsigned short zoom_;
    bool showrulers_, showgrid_, showpreview_, showborders_, showareasglobal_, showareas_;
    wxPoint drawtarget_;

    DECLARE_EVENT_TABLE()
};

#endif // __BVIEW_BUTCHERVIEWDISPLAY_H__
