//=============================================================================
/**
 *  @file    HTMLButcherMain.h
 *
 *  $Id: HTMLButcherMain.h,v 1.54 2008/02/16 12:52:50 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-11-29
 */
//=============================================================================
#ifndef HTMLBUTCHERMAIN_H
#define HTMLBUTCHERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "wx/aui/aui.h"
#include <wx/docview.h>

#include "HTMLButcherApp.h"
#include "ButcherView.h"
#include "ButcherDocumentMouseEvent.h"
#include "ButcherDocumentKeyEvent.h"
#include "ButcherViewSelectEvent.h"
#include "ButcherStatusEvent.h"
#include "wxpopupmenu.h"
#include "DialogProgress.h"
#include "BUtil.h"
#include "BControls.h"
#include "BLanguage.h"

class ButcherViewEditor;
class ButcherViewDisplay;

class HTMLButcherFrame: public wxFrame
{
public:
    HTMLButcherFrame(wxFrame *frame, const wxString& title);
    ~HTMLButcherFrame();

#ifndef HTMLBUTCHER_DEMO
    void DoOpenProject(const wxString &filename);
#endif
    bool DoCloseProject();
private:
    enum
    {
        idMenuNew = wxID_HIGHEST+1000,
        idMenuOpen,
        idMenuClose,
        idMenuSave,
        idMenuSaveAs,
        idMenuSaveForWeb,
        idMenuSaveForWebMultiple,
        //idMenuQuit,

        idMenuModeNone,
        idMenuModeLine,
        idMenuModeArea,

        idMenuEditModeBasic,
        idMenuEditModeNormal,
        idMenuEditModeAdvanced,

        idMenuFiles,
        idMenuMasks,
        idMenuViews,
        idMenuFilePaths,
        idMenuAssortedFileGroups,
        idMenuAssortedFiles,
        idMenuCSSFiles,
        idMenuImageFormats,
        idMenuMaskCurrent,
        idMenuViewCurrent,
        idMenuAreasCurrent,
		idMenuWizNewView,
		idMenuProjectOptions,

        idMenuFileAlternate,

        idMenuViewSelect,
        idMenuViewSelect_MAX = idMenuViewSelect + 100,
        idMenuZoomIn,
        idMenuZoomOut,
        idMenuZoomNormal,
        idMenuZoom,
        idMenuShowPreview,
        idMenuShowBorders,
        idMenuShowAreas,
        idMenuShowAreasGlobal,
        idMenuShowAreasMap,
        idMenuGrid,
        idMenuGridSize,

#if NEED_CHOOSELANG_UI
		idMenuLanguage,
#endif

        //idMenuOptions,

        //idMenuHelpAbout,

#ifdef HTMLBUTCHER_DEBUG
		idMenuHelpTest,
#endif

        idViewList,
        idModeList,
        idFileAlternateList,

        idMenuInsertLineVertical,
        idMenuInsertLineHorizontal,
        idMenuDeleteLine,
        idMenuMoveLine,

        idMenuInsertAreaGlobal,
        idMenuDeleteAreaGlobal,

        idMenuInsertAreaMap,
        idMenuDeleteAreaMap,

        idMenuCancelOperation,

        idTBFile,
        idTBTools,
        idTBDetail,
        idTBView,
		idTBSelection,

        idView,
		idViewZoom,
		idViewInfo

    };

    enum {
        ID_AREAMENU_MIN = wxID_HIGHEST+1,
        ID_AREAMENU_CONFIG,
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
        ID_AREAMENU_PROPERTIES,
        ID_AREAMENU_MAX
    };

    enum {
        ID_LINEMENU_MIN = wxID_HIGHEST+1,
        ID_LINEMENU_DELETE,
        ID_LINEMENU_PROPERTIES,
        ID_LINEMENU_MAX
    };

    void OnMenuNew(wxCommandEvent& event);
#ifndef HTMLBUTCHER_DEMO
    void OnMenuOpen(wxCommandEvent& event);
#endif
    void OnMenuClose(wxCommandEvent& event);
#ifndef HTMLBUTCHER_DEMO
    void OnMenuSave(wxCommandEvent& event);
    void OnMenuSaveAs(wxCommandEvent& event);
#endif
    void OnMenuSaveForWeb(wxCommandEvent& event);
#ifndef HTMLBUTCHER_DEMO
    void OnMenuSaveForWebMultiple(wxCommandEvent& event);
#endif
    void OnMenuQuit(wxCommandEvent& event);

#ifndef HTMLBUTCHER_DEMO
    void OnMenuHistory(wxCommandEvent &event);
#endif

    void OnMenuMode(wxCommandEvent& event);
    void OnMenuEditMode(wxCommandEvent& event);

    void OnMenuFiles(wxCommandEvent& event);
    void OnMenuMasks(wxCommandEvent& event);
    void OnMenuViews(wxCommandEvent& event);

    void OnMenuFilePaths(wxCommandEvent& event);
    void OnMenuAssortedFileGroups(wxCommandEvent& event);
    void OnMenuAssortedFiles(wxCommandEvent& event);
    void OnMenuCSSFiles(wxCommandEvent& event);

    void OnMenuMaskCurrent(wxCommandEvent& event);
    void OnMenuViewCurrent(wxCommandEvent& event);
    void OnMenuAreasCurrent(wxCommandEvent& event);

    void OnMenuWizNewView(wxCommandEvent& event);

    void OnMenuProjectOptions(wxCommandEvent& event);

    void OnMenuImageFormats(wxCommandEvent& event);

    void OnMenuShowPreview(wxCommandEvent& event);
    void OnMenuShowBorders(wxCommandEvent& event);
    void OnMenuShowAreas(wxCommandEvent& event);
    void OnMenuShowAreasGlobal(wxCommandEvent& event);
    void OnMenuShowAreasMap(wxCommandEvent& event);
    void OnMenuZoom(wxCommandEvent& event);
    void OnMenuViewSelect(wxCommandEvent& event);
    void OnMenuGrid(wxCommandEvent& event);
    void OnMenuGridSize(wxCommandEvent& event);
    void OnMenuFileAlternate(wxCommandEvent& event);

#if NEED_CHOOSELANG_UI
	void OnMenuLanguage(wxCommandEvent& event);
#endif

    void OnMenuOptions(wxCommandEvent& event);

    void OnMenuOperation(wxCommandEvent& event);

    void OnClose(wxCloseEvent& event);
    void OnHelpHelp(wxCommandEvent& event);
    void OnHelpAbout(wxCommandEvent& event);
#ifdef HTMLBUTCHER_DEBUG
	void OnHelpTest(wxCommandEvent& event);
#endif

    void OnAbout(wxCommandEvent& event);
    void OnAbout2(wxCommandEvent& event);
    void OnAbout3(wxCommandEvent& event);
    void OnAbout4(wxCommandEvent& event);
    void OnAbout5(wxCommandEvent& event);

    void OnProjectEvent(ButcherProjectEvent& event);
    void OnDocumentMouse(ButcherDocumentMouseEvent& event);
    void OnDocumentKey(ButcherDocumentKeyEvent& event);
    void OnBViewSelect(ButcherViewSelectEvent& event);
    void OnButcherStatus(ButcherStatusEvent& event);
    void OnButcherOperation(wxCommandEvent& event);

    void OnViewSelect(wxCommandEvent& event);
    void OnModeSelect(wxCommandEvent& event);
    void OnFileAlternateSelect(wxCommandEvent& event);

    void UpdateAppState();

    void UpdateProgress();

    void LoadViews(bool check = false);
    void SetView(unsigned long id);
    void LoadModes();
    void LoadFileAlternate();

/*
    wxMenu* BuildAreaMenu(wxMenu *rootmenu, int index, ButcherProjectArea *area);
    void ShowAreaMenu(ButcherProjectMaskAreaSelect *select);
    void ProcessAreaMenu(int menuid, ButcherProjectMaskAreaSelect *select);

    wxMenu* BuildLineMenu(wxMenu *rootmenu, int index, ButcherProjectLine *line);
    void ShowLineMenu(ButcherProjectMaskLineSelect *select);
    void ProcessLineMenu(int menuid, ButcherProjectMaskLineSelect *select);
*/

    void FileAlternateSelect(int index, bool refreshlist = true);


    wxAuiManager mgr_;
    wxPanel* panBase_;
    wxBoxSizer *szBase_;

    ButcherProject* project_;
    ButcherViewEditor* view_;
	ButcherControl_SelectionDisplay *viewinfo_;
    ButcherViewDisplay* viewzoom_;

    ButcherProjectMaskLineSelect *lineselect_;
    ButcherProjectMaskAreaSelect *areaselect_;

    unsigned long menuid_;
    HTMLButcherProgressDialog *progdlg_;
#ifndef HTMLBUTCHER_DEMO
    wxFileHistory filehistory_;
#endif
    ButcherOptions options_;

    DECLARE_EVENT_TABLE()
};


#endif // HTMLBUTCHERMAIN_H
