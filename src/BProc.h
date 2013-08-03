//=============================================================================
/**
 *  @file    BProc.h
 *
 *  $Id: BProc.h,v 1.2 2008/02/16 12:52:50 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-10
 */
//=============================================================================
#ifndef __BPROC_H__
#define __BPROC_H__

#include <wx/wx.h>
#include "ButcherProjectBase.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectLine.h"
#include "ButcherProjectMaskSelect.h"
#include "ButcherProjectView.h"

class ButcherProjectProc_AreaMenu : public ButcherProjectBase
{
public:
    ButcherProjectProc_AreaMenu(ButcherProject *project,
        wxWindow *window, ButcherProjectMaskAreaSelect *select,
        ButcherProjectView *view, bool showpreview = false) :
        ButcherProjectBase(project), window_(window), select_(select),
        view_(view), showpreview_(showpreview) {}

    enum {
        ID_AREAMENU_MIN = wxID_HIGHEST+1,
        ID_AREAMENU_CONFIG,
		ID_AREAMENU_COPY,
		ID_AREAMENU_COPY_ITEMBEGIN,
		ID_AREAMENU_COPY_ITEMEND = ID_AREAMENU_COPY_ITEMBEGIN+500,
        ID_AREAMENU_KINDNONE,
        ID_AREAMENU_KINDIMAGE,
        ID_AREAMENU_KINDMASK,
        ID_AREAMENU_BACKGROUND,
        ID_AREAMENU_IMAGEFORMAT,
        ID_AREAMENU_IMAGEFORMAT_ITEMBEGIN,
        ID_AREAMENU_IMAGEFORMAT_ITEMEND = ID_AREAMENU_IMAGEFORMAT_ITEMBEGIN+100,
        ID_AREAMENU_IMAGEFORMAT_SELECT,
        ID_AREAMENU_DELETE,
        ID_AREAMENU_SIZE,
        ID_AREAMENU_FILEALTERNATE,
        ID_AREAMENU_SLICE,
        ID_AREAMENU_SAVERAWIMAGEAS,
        ID_AREAMENU_SAVEIMAGEAS,
        ID_AREAMENU_RENAME,
        ID_AREAMENU_PROPERTIES,
        ID_AREAMENU_MAX
    };

    void ShowAreaMenu();
protected:
    wxMenu* BuildAreaMenu(wxMenu *rootmenu, int index, ButcherProjectArea *area);
    void ProcessAreaMenu(int menuid);
private:
    wxWindow *window_;
    ButcherProjectMaskAreaSelect *select_;
    ButcherProjectView *view_;
    bool showpreview_;
};

class ButcherProjectProc_LineMenu : public ButcherProjectBase
{
public:
    ButcherProjectProc_LineMenu(ButcherProject *project,
        wxWindow *window, ButcherProjectMaskLineSelect *select,
        ButcherProjectView *view, bool showpreview = false) :
        ButcherProjectBase(project), window_(window), select_(select),
        view_(view), showpreview_(showpreview) {}

    enum {
        ID_LINEMENU_MIN = wxID_HIGHEST+1,
        ID_LINEMENU_DELETE,
        ID_LINEMENU_PROPERTIES,
        ID_LINEMENU_MAX
    };

    void ShowLineMenu();
protected:
    wxMenu* BuildLineMenu(wxMenu *rootmenu, int index, ButcherProjectLine *line);
    void ProcessLineMenu(int menuid);
private:
    wxWindow *window_;
    ButcherProjectMaskLineSelect *select_;
    ButcherProjectView *view_;
    bool showpreview_;
};

#endif //__BPROC_H__
