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

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QLabel>

class ButcherViewEditor;
class ButcherViewDisplay;

class HTMLButcherFrame: public QMainWindow //wxFrame
{
	Q_OBJECT
public:
    HTMLButcherFrame();
    ~HTMLButcherFrame();

#ifndef HTMLBUTCHER_DEMO
    void DoOpenProject(const wxString &filename);
#endif
    bool DoCloseProject();
public Q_SLOTS:
	void OnProjectEvent(const ButcherProjectEvent& event);
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

private Q_SLOTS:
    void OnMenuNew();


#ifndef HTMLBUTCHER_DEMO
    void OnMenuOpen();
#endif

    void OnMenuClose();

#ifndef HTMLBUTCHER_DEMO
    void OnMenuSave();
    void OnMenuSaveAs();
#endif

    void OnMenuSaveForWeb();

#ifndef HTMLBUTCHER_DEMO
    void OnMenuSaveForWebMultiple();
#endif

	void OnMenuOptions();
	void OnMenuQuit();

#ifdef QT_HIDE_FROM
#ifndef HTMLBUTCHER_DEMO
    void OnMenuHistory(wxCommandEvent &event);
#endif
#endif // QT_HIDE_FROM

	void OnMenuViewSelect();
	void OnMenuZoom(QAction *action);

	void OnMenuShowPreview(bool checked);
	void OnMenuShowBorders(bool checked);
	void OnMenuShowAreas(bool checked);
	void OnMenuShowAreasGlobal(bool checked);
	void OnMenuShowAreasMap(bool checked);
	void OnMenuGrid(bool checked);
	void OnMenuGridSize();
	void OnMenuFileAlternate(bool checked);

#if NEED_CHOOSELANG_UI
	void OnMenuLanguage();
#endif

    void OnMenuMode(QAction *action);
    void OnMenuEditMode(QAction *action);

	void OnMenuFiles();
    void OnMenuMasks();
    void OnMenuViews();

    void OnMenuFilePaths();
    void OnMenuAssortedFileGroups();
    void OnMenuAssortedFiles();
    void OnMenuCSSFiles();

	void OnMenuImageFormats();

	void OnMenuMaskCurrent();
    void OnMenuViewCurrent();
    void OnMenuAreasCurrent();

    void OnMenuWizNewView();

    void OnMenuProjectOptions();

	void OnHelpHelp();
	void OnHelpAbout();
#ifdef HTMLBUTCHER_DEBUG
	void OnHelpTest();
#endif

    void OnMenuOperation(QAction *action);

#ifdef QT_HIDE_FROM

	void OnClose(wxCloseEvent& event);

#endif // QT_HIDE_FROM

private:

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


	/*
    wxAuiManager mgr_;
    wxPanel* panBase_;
    wxBoxSizer *szBase_;
	*/
	QWidget *panBase_;

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

	QAction *menuNew_, *menuOpen_, *menuClose_, *menuSave_, *menuSaveAs_, *menuSaveForWeb_, *menuSaveForWebMultiple_, *menuOptions_, *menuQuit_;
	QAction *menuViewSelect_, *menuZoomOut_, *menuZoomIn_, *menuZoomNormal_, *menuZoom_;
	QAction *menuShowPreview_, *menuShowBorders_, *menuShowAreas_, *menuShowAreasGlobal_, *menuShowAreasMap_, *menuGrid_, *menuGridSize_;
	QAction *menuFileAlternate_, *menuLanguage_;
	QAction *menuModeNone_, *menuModeLine_, *menuModeArea_, *menuEditModeBasic_, *menuEditModeNormal_, *menuEditModeAdvanced_;
	QAction *menuFiles_, *menuMasks_, *menuViews_, *menuFilePaths_, *menuAssortedFileGroups_, *menuAssortedFiles_, *menuCSSFiles_;
	QAction *menuImageFormats_, *menuMaskCurrent_, *menuViewCurrent_, *menuAreasCurrent_, *menuWizNewView_, *menuProjectOptions_;
	QAction *menuHelp_, *menuAbout_, *menuHelpTest_;

	QAction *menuInsertLineHorizontal_, *menuInsertLineVertical_, *menuMoveLine_, *menuDeleteLine_, *menuInsertAreaGlobal_, *menuDeleteAreaGlobal_, *menuInsertAreaMap_, *menuDeleteAreaMap_, *menuCancelOperation_;

	QLabel  *status0_, *status1_, *status2_, *status3_, *status4_;

    //DECLARE_EVENT_TABLE()
};


#endif // HTMLBUTCHERMAIN_H
