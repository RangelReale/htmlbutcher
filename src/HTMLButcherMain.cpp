//=============================================================================
/**
 *  @file    HTMLButcherMain.cpp
 *
 *  $Id: HTMLButcherMain.cpp,v 1.102 2008/02/16 12:52:50 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-11-29
 */
//=============================================================================

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/dcbuffer.h>
#include <wx/numdlg.h>
#include <wx/filename.h>
#include <wx/confbase.h>
#include <wx/xrc/xmlres.h>
#include <cppcomp/wxccucontrol.h>

#include "HTMLButcherMain.h"
#include "ButcherViewEditor.h"
#include "ButcherViewDisplay.h"


#include "DialogFileEdit.h"
#include "DialogMaskEdit.h"
#include "DialogListEdit.h"
#include "DialogViewEdit.h"
#include "DialogFileLink.h"
#include "DialogMaskAreaListEdit.h"
#include "DialogImageFormatEdit.h"
#include "DialogAbout.h"
#include "DialogOptions.h"
#include "BProc.h"
#include "HTMLButcherVersion.h"
#include "WizNewView.h"
#include "DialogProjectOptions.h"

#ifdef HTMLBUTCHER_DEMO
#include "DialogDemo.h"
#endif

#include <QMenuBar>

/*
BEGIN_EVENT_TABLE(HTMLButcherFrame, wxFrame)
    EVT_CLOSE(HTMLButcherFrame::OnClose)
    EVT_MENU(idMenuNew, HTMLButcherFrame::OnMenuNew)
    EVT_MENU(idMenuClose, HTMLButcherFrame::OnMenuClose)
#ifndef HTMLBUTCHER_DEMO
    EVT_MENU(idMenuOpen, HTMLButcherFrame::OnMenuOpen)
	EVT_MENU(idMenuSave, HTMLButcherFrame::OnMenuSave)
    EVT_MENU(idMenuSaveAs, HTMLButcherFrame::OnMenuSaveAs)
#endif
    EVT_MENU(idMenuSaveForWeb, HTMLButcherFrame::OnMenuSaveForWeb)
#ifndef HTMLBUTCHER_DEMO
    EVT_MENU(idMenuSaveForWebMultiple, HTMLButcherFrame::OnMenuSaveForWebMultiple)
#endif
    EVT_MENU(wxID_PREFERENCES, HTMLButcherFrame::OnMenuOptions)
    EVT_MENU(wxID_EXIT, HTMLButcherFrame::OnMenuQuit)
#ifdef BUTCHER_USE_HELP
    EVT_MENU(wxID_HELP, HTMLButcherFrame::OnHelpHelp)
#endif
	EVT_MENU(wxID_ABOUT, HTMLButcherFrame::OnHelpAbout)
#ifdef HTMLBUTCHER_DEBUG
	EVT_MENU(idMenuHelpTest, HTMLButcherFrame::OnHelpTest)
#endif
    EVT_MENU(idMenuFiles, HTMLButcherFrame::OnMenuFiles)
    EVT_MENU(idMenuMasks, HTMLButcherFrame::OnMenuMasks)
    EVT_MENU(idMenuViews, HTMLButcherFrame::OnMenuViews)
    EVT_MENU(idMenuFilePaths, HTMLButcherFrame::OnMenuFilePaths)
    EVT_MENU(idMenuAssortedFileGroups, HTMLButcherFrame::OnMenuAssortedFileGroups)
    EVT_MENU(idMenuAssortedFiles, HTMLButcherFrame::OnMenuAssortedFiles)
    EVT_MENU(idMenuCSSFiles, HTMLButcherFrame::OnMenuCSSFiles)
    EVT_MENU(idMenuImageFormats, HTMLButcherFrame::OnMenuImageFormats)
    EVT_MENU(idMenuMaskCurrent, HTMLButcherFrame::OnMenuMaskCurrent)
    EVT_MENU(idMenuViewCurrent, HTMLButcherFrame::OnMenuViewCurrent)
    EVT_MENU(idMenuAreasCurrent, HTMLButcherFrame::OnMenuAreasCurrent)
    EVT_MENU(idMenuWizNewView, HTMLButcherFrame::OnMenuWizNewView)
    EVT_MENU(idMenuProjectOptions, HTMLButcherFrame::OnMenuProjectOptions)
    EVT_BUTCHERPROJECT(wxID_ANY, HTMLButcherFrame::OnProjectEvent)
    EVT_COMBOBOX(idViewList, HTMLButcherFrame::OnViewSelect)
    EVT_COMBOBOX(idModeList, HTMLButcherFrame::OnModeSelect)
    EVT_COMBOBOX(idFileAlternateList, HTMLButcherFrame::OnFileAlternateSelect)
    EVT_MENU_RANGE(idMenuZoomIn, idMenuZoom, HTMLButcherFrame::OnMenuZoom)
    EVT_MENU(idMenuShowPreview, HTMLButcherFrame::OnMenuShowPreview)
    EVT_MENU(idMenuShowBorders, HTMLButcherFrame::OnMenuShowBorders)
    EVT_MENU(idMenuShowAreas, HTMLButcherFrame::OnMenuShowAreas)
    EVT_MENU(idMenuShowAreasGlobal, HTMLButcherFrame::OnMenuShowAreasGlobal)
    EVT_MENU(idMenuShowAreasMap, HTMLButcherFrame::OnMenuShowAreasMap)
    EVT_MENU(idMenuGrid, HTMLButcherFrame::OnMenuGrid)
    EVT_MENU(idMenuGridSize, HTMLButcherFrame::OnMenuGridSize)
#if NEED_CHOOSELANG_UI
    EVT_MENU(idMenuLanguage, HTMLButcherFrame::OnMenuLanguage)
#endif
    EVT_MENU_RANGE(idMenuModeNone, idMenuModeArea, HTMLButcherFrame::OnMenuMode)
    EVT_MENU_RANGE(idMenuEditModeBasic, idMenuEditModeAdvanced, HTMLButcherFrame::OnMenuEditMode)
    EVT_MENU_RANGE(idMenuInsertLineVertical, idMenuCancelOperation, HTMLButcherFrame::OnMenuOperation)
    EVT_MENU(idMenuFileAlternate, HTMLButcherFrame::OnMenuFileAlternate)
    EVT_MENU_RANGE(idMenuViewSelect, idMenuViewSelect_MAX, HTMLButcherFrame::OnMenuViewSelect)
    EVT_BUTCHERDOCUMENTMOUSE(wxID_ANY, HTMLButcherFrame::OnDocumentMouse)
    EVT_BUTCHERDOCUMENTKEY(wxID_ANY, HTMLButcherFrame::OnDocumentKey)
    EVT_BUTCHERVIEWSELECT(wxID_ANY, HTMLButcherFrame::OnBViewSelect)
    EVT_BUTCHERSTATUS(wxID_ANY, HTMLButcherFrame::OnButcherStatus)
    EVT_BUTCHEROPERATION(wxID_ANY, HTMLButcherFrame::OnButcherOperation)

#ifndef HTMLBUTCHER_DEMO
    EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, HTMLButcherFrame::OnMenuHistory)
#endif

    //EVT_UPDATE_UI_RANGE(idAreaMenuStart, idAreaMenuEnd, HTMLButcherFrame::OnUpdateUIAreaMenu)
END_EVENT_TABLE()
*/

HTMLButcherFrame::HTMLButcherFrame()
    : /*wxFrame(frame, -1, title, wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_FRAME_STYLE | wxMAXIMIZE),*/ 
		QMainWindow(),
		project_(NULL), view_(NULL), viewinfo_(NULL),
        lineselect_(NULL), areaselect_(NULL), progdlg_(NULL),
#ifndef HTMLBUTCHER_DEMO
		filehistory_(),
#endif
        options_()
{
#ifdef HTMLBUTCHER_DEBUG
    //(void) new wxLogWindow(this, wxT("log"));
#endif //__WXDEBUG__

    // notify wxAUI which frame to use
    //mgr_.SetManagedWindow(this);

    options_.Load();

    // create a menu bar
    //wxMenuBar* mbar = new wxMenuBar();
    QMenu* fileMenu = new QMenu(tr("&File"), this);
    //fileMenu->Append(idMenuNew, _("&New\tCtrl-N"), _("Create new project"));
	menuNew_ = new QAction(tr("&New\tCtrl-N"), this); menuNew_->setStatusTip(tr("Create new project")); fileMenu->addAction(menuNew_);
	connect(menuNew_, SIGNAL(triggered()), this, SLOT(OnMenuNew()));
	/*
#ifndef HTMLBUTCHER_DEMO
    fileMenu->Append(idMenuOpen, _("&Open...\tCtrl-O"), _("Open project"));
#endif
    fileMenu->Append(idMenuClose, _("C&lose"), _("Close project"));
#ifndef HTMLBUTCHER_DEMO
    fileMenu->Append(idMenuSave, _("&Save\tCtrl-S"), _("Save project"));
    fileMenu->Append(idMenuSaveAs, _("Save &as..."), _("Save project with a different file name"));
#endif
    fileMenu->AppendSeparator();
    fileMenu->Append(idMenuSaveForWeb, _("Save for &Web...\tCtrl-W"), _("Export HTML"));
#ifndef HTMLBUTCHER_DEMO
    fileMenu->Append(idMenuSaveForWebMultiple, _("Save all for We&b..."), _("Export multiple views HTML"));
#endif
	fileMenu->AppendSeparator();
    fileMenu->Append(wxID_PREFERENCES, _("&Preferences..."), _("Program preferences"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _("E&xit\tAlt-F4"), _("Exit the application"));

#ifndef HTMLBUTCHER_DEMO
    filehistory_.UseMenu(fileMenu);
#endif
    wxConfigBase::Get(true)->SetPath(wxT("/"));
#ifndef HTMLBUTCHER_DEMO
    filehistory_.Load(*wxConfigBase::Get(true));
#endif
	*/

    //mbar->Append(fileMenu, _("&File"));
	menuBar()->addMenu(fileMenu);

    QMenu* viewMenu = new QMenu(tr("&View"), this);

	/*
    //wxMenu* viewviewMenu = new wxMenu(_T(""));
    //viewMenu->Append(idMenuViewSelect, _("&View"), _("View"));
    //viewMenu->Append(idMenuViewSelect, wxT("&View"), viewviewMenu, wxT("View"));
	viewMenu->Append(idMenuViewSelect, _("&View"), _("View"));

    viewMenu->AppendSeparator();
    viewMenu->Append(idMenuZoomOut, _("Zoom &Out\t-"), _("Zoom out"));
    viewMenu->Append(idMenuZoomIn, _("Zoom &In\t+"), _("Zoom in"));
    viewMenu->Append(idMenuZoomNormal, _("Zoom &Normal\t/"), _("Zoom normal"));
    viewMenu->Append(idMenuZoom, _("&Zoom...\tCtrl-Z"), _("Zoom"));
    viewMenu->AppendSeparator();
    viewMenu->AppendCheckItem(idMenuShowPreview, _("&Preview\tCTRL-V"), _("Show/hide preview"));
    viewMenu->AppendCheckItem(idMenuShowBorders, _("&Borders\tCTRL-B"), _("Show/hide borders"));
    viewMenu->AppendCheckItem(idMenuShowAreas, _("&Areas\tCTRL-A"), _("Show/hide areas"));
    viewMenu->AppendCheckItem(idMenuShowAreasGlobal, _("G&lobal Areas\tCTRL-L"), _("Show/hide global areas"));
    viewMenu->AppendCheckItem(idMenuShowAreasMap, _("&Map Areas\tCTRL-M"), _("Show/hide map areas"));
    viewMenu->AppendCheckItem(idMenuGrid, _("&Grid\tCTRL-G"), _("Show/hide grid"));
    viewMenu->Append(idMenuGridSize, _("Grid &size..."), _("Grid size"));
    viewMenu->AppendSeparator();
    viewMenu->AppendCheckItem(idMenuFileAlternate, _("Alterna&te File\tF12"), _("Show alternate file"));
#if NEED_CHOOSELANG_UI
    viewMenu->AppendSeparator();
    viewMenu->AppendCheckItem(idMenuLanguage, _("Lang&uage..."), _("Select language"));
#endif
	*/
    //mbar->Append(viewMenu, _("&View"));
	menuBar()->addMenu(viewMenu);

    QMenu* modeMenu = new QMenu(tr("&Mode"), this);
	/*
    modeMenu->AppendRadioItem(idMenuModeNone, _("&None"), _("No selection mode"));
    modeMenu->AppendRadioItem(idMenuModeLine, _("&Line\tF2"), _("Line selection mode"));
    modeMenu->AppendRadioItem(idMenuModeArea, _("&Area\tF3"), _("Area selection mode"));
    wxMenu* editmodeMenu = new wxMenu(wxEmptyString);
    editmodeMenu->AppendRadioItem(idMenuEditModeBasic, _("&Basic"), _("Basic edit mode"));
    editmodeMenu->AppendRadioItem(idMenuEditModeNormal, _("&Normal"), _("Normal edit mode"));
    editmodeMenu->AppendRadioItem(idMenuEditModeAdvanced, _("&Advanced"), _("Advanced edit mode"));
    modeMenu->AppendSeparator();
    modeMenu->Append(wxID_STATIC, _("&Edit mode"), editmodeMenu, _("Edit mode"));
	*/
    //mbar->Append(modeMenu, _("&Mode"));
	menuBar()->addMenu(modeMenu);

    QMenu* dataMenu = new QMenu(tr("&Data"), this);
	/*
    dataMenu->Append(idMenuFiles, _("&Files...\tF5"), _("Project files"));
    dataMenu->Append(idMenuMasks, _("&Masks...\tF6"), _("Project masks"));
    dataMenu->Append(idMenuViews, _("&Views...\tF7"), _("Project views"));
    dataMenu->AppendSeparator();
    dataMenu->Append(idMenuFilePaths, _("File &paths..."), _("File paths"));
    dataMenu->Append(idMenuAssortedFileGroups, _("Assorted file &groups..."), _("Assorted file groups"));
    dataMenu->Append(idMenuAssortedFiles, _("&Assorted files...\tCtrl-D"), _("Assorted files"));
    dataMenu->Append(idMenuCSSFiles, _("&CSS files...\tF9"), _("CSS files"));
    dataMenu->AppendSeparator();
    dataMenu->Append(idMenuImageFormats, _("&Image Formats..."), _("Image formats"));
    dataMenu->AppendSeparator();
    dataMenu->Append(idMenuMaskCurrent, _("Current mas&k...\tCtrl-F6"), _("Edit current mask"));
    dataMenu->Append(idMenuViewCurrent, _("&Current view...\tCtrl-F7"), _("Edit current view"));
    dataMenu->Append(idMenuAreasCurrent, _("Current a&reas...\tCtrl-Shift-F7"), _("Edit current view's areas"));
    dataMenu->AppendSeparator();
    dataMenu->Append(idMenuWizNewView, _("New vie&w wizard"), _("Shows the new view wizard"));
    dataMenu->AppendSeparator();
    dataMenu->Append(idMenuProjectOptions, _("Project &options..."), _("Project options"));
	*/
    //mbar->Append(dataMenu, _("&Data"));
	menuBar()->addMenu(dataMenu);

    QMenu* helpMenu = new QMenu(tr("&Help"), this);
	/*
#ifdef BUTCHER_USE_HELP
	helpMenu->Append(wxID_HELP, _("&Contents\tF1"), _("Help contents"));
    helpMenu->AppendSeparator();
#endif
    helpMenu->Append(wxID_ABOUT, _("&About"), _("Show info about this application"));
#ifdef HTMLBUTCHER_DEBUG
	helpMenu->Append(idMenuHelpTest, _("&Test"), _("Test"));
#endif
	*/
    //mbar->Append(helpMenu, _("&Help"));
	menuBar()->addMenu(helpMenu);

    //SetMenuBar(mbar);

    // create a status bar with some information about the used wxWidgets version
	/*
    wxStatusBar *sbar = new wxStatusBar(this);
    int sbarwidths[5] = { 100, 100, 250, 300, 100 };
    sbar->SetFieldsCount(5, sbarwidths);

    SetStatusBar(sbar);
    SetStatusBarPane(3);
	*/

	statusBar();

#ifdef QT_HIDE_FROM
    wxBitmap bmp_base = wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(16,16));

    // TOOLBAR: File
#if defined(__WXMAC__)
    wxToolBar* tbFile = CreateToolBar(wxTB_FLAT | wxTB_NODIVIDER, idTBFile);
#else
    wxToolBar* tbFile = new wxToolBar(this, idTBFile, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER);
#endif
    tbFile->SetToolBitmapSize(wxSize(16,16));
    tbFile->AddTool(idMenuNew, _("New Project"), butil_loadxmlrcbitmap(wxT("ico_new")),
        _("New Project"));

#ifndef HTMLBUTCHER_DEMO
    tbFile->AddTool(idMenuOpen, _("Open Project"), butil_loadxmlrcbitmap(wxT("ico_open")),
        _("Open Project"));
    tbFile->AddTool(idMenuSave, _("Save Project"), butil_loadxmlrcbitmap(wxT("ico_save")),
        _("Save Project"));
#endif

    tbFile->Realize();

    // TOOLBAR: Tools

    wxToolBar* tbTools = new wxToolBar(this, idTBTools, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER | wxTB_VERTICAL);
    tbTools->SetToolBitmapSize(wxSize(32,32));
    tbTools->AddTool(idMenuInsertLineHorizontal, _("Insert Horizontal Line"), butil_loadxmlrcbitmap(wxT("ico_insert_hline")),
        _("Insert Horizontal Line"));
    tbTools->AddTool(idMenuInsertLineVertical, _("Insert Vertical Line"), butil_loadxmlrcbitmap(wxT("ico_insert_vline")),
        _("Insert Vertical Line"));
    tbTools->AddTool(idMenuMoveLine, _("Move line"), butil_loadxmlrcbitmap(wxT("ico_move_line")),
        _("Move line"));
    tbTools->AddTool(idMenuDeleteLine, _("Delete line"), butil_loadxmlrcbitmap(wxT("ico_delete_line")),
        _("Delete line"));
    tbTools->AddTool(idMenuInsertAreaGlobal, _("Insert global area"), butil_loadxmlrcbitmap(wxT("ico_insert_garea")),
        _("Insert global area"));
    tbTools->AddTool(idMenuDeleteAreaGlobal, _("Delete global area"), butil_loadxmlrcbitmap(wxT("ico_delete_garea")),
        _("Delete global area"));
    tbTools->AddTool(idMenuInsertAreaMap, _("Insert map area"), butil_loadxmlrcbitmap(wxT("ico_insert_marea")),
        _("Insert map area"));
    tbTools->AddTool(idMenuDeleteAreaMap, _("Delete map area"), butil_loadxmlrcbitmap(wxT("ico_delete_marea")),
        _("Delete map area"));
    tbTools->AddTool(idMenuCancelOperation, _("Cancel operation"), butil_loadxmlrcbitmap(wxT("ico_cancel")),
		_("Cancel operation"));
    tbTools->Realize();

    // TOOLBAR: DETAIL
/*
    wxToolBar* tbDetail = new wxToolBar(this, idTBDetail, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER | wxTB_VERTICAL);
    tbDetail->SetToolBitmapSize(wxSize(200,300));

    viewzoom_=new ButcherViewDisplay(tbDetail, wxID_ANY, wxDefaultPosition, wxSize(200, 300));
    tbDetail->AddControl(viewzoom_);

    tbDetail->Realize();
*/

    // TOOLBAR: SELECTION
/*
	wxToolBar* tbSelection = new wxToolBar(this, idTBSelection, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER | wxTB_VERTICAL);

	wxBoxSizer *selsizer = new wxBoxSizer(wxVERTICAL);

	tbSelection->SetToolBitmapSize(wxSize(200,300));

	viewinfo_ = new ButcherControl_SelectionDisplay(tbSelection, idViewInfo, wxDefaultPosition, wxSize(200, 300));
	selsizer->Add(viewinfo_, 1, wxEXPAND);
    tbSelection->AddControl(viewinfo_);

    tbSelection->Realize();


	tbSelection->SetSizer(selsizer);
	selsizer->SetSizeHints(tbSelection);
*/

    // TOOLBAR: View
#if defined(__WXMAC__)
    wxToolBar* tbView = tbFile;
    tbView->AddSeparator();
#else
    wxToolBar* tbView = new wxToolBar(this, idTBView, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER);
#endif
    tbView->SetToolBitmapSize(wxSize(16,16));
    tbView->AddControl(new wxStaticText(tbView, wxID_ANY, _("View:"), wxDefaultPosition, wxDefaultSize, 0));
    tbView->AddControl(new wxComboBox(tbView, idViewList, wxEmptyString, wxDefaultPosition, wxSize(200, -1),
        wxArrayString(), wxCB_READONLY|wxCB_SORT));
    tbView->AddTool(idMenuZoomOut, _("Zoom Out"), butil_loadxmlrcbitmap(wxT("ico_zoom_out")),
        _("Zoom Out"));
    tbView->AddTool(idMenuZoomIn, _("Zoom In"), butil_loadxmlrcbitmap(wxT("ico_zoom_in")),
        _("Zoom In"));
    tbView->AddTool(idMenuZoomNormal, _("Zoom Normal"), butil_loadxmlrcbitmap(wxT("ico_zoom_normal")),
        _("Zoom Normal"));
    tbView->AddSeparator();
    tbView->AddControl(new wxStaticText(tbView, wxID_ANY, _("Mode:"), wxDefaultPosition, wxDefaultSize, 0));
    tbView->AddControl(new wxComboBox(tbView, idModeList, wxEmptyString, wxDefaultPosition, wxSize(80, -1),
        wxArrayString(), wxCB_READONLY));
    tbView->AddSeparator();
    tbView->AddControl(new wxStaticText(tbView, wxID_ANY, _("Alternate File:"), wxDefaultPosition, wxDefaultSize, 0));
    tbView->AddControl(new wxComboBox(tbView, idFileAlternateList, wxEmptyString, wxDefaultPosition, wxSize(160, -1),
        wxArrayString(), wxCB_READONLY));
    tbView->Realize();

    // add the toolbars to the manager
    mgr_.AddPane(tbFile, wxAuiPaneInfo().
                  Name(wxT("tbFile")).Caption(_("Main")).
                  ToolbarPane().Top().
                  LeftDockable(false).RightDockable(false));

    mgr_.AddPane(tbView, wxAuiPaneInfo().
                  Name(wxT("tbView")).Caption(_("View")).
                  ToolbarPane().Top().Position(2).
                  LeftDockable(false).RightDockable(false));

    mgr_.AddPane(tbTools, wxAuiPaneInfo().
                  Name(wxT("tbTools")).Caption(_("Tools")).
                  ToolbarPane().Left().
                  GripperTop().
                  TopDockable(false).BottomDockable(false));

/*
    mgr_.AddPane(tbDetail, wxAuiPaneInfo().
                  Name(wxT("tbDetail")).Caption(_("Detail")).
                  ToolbarPane().Left().
                  GripperTop().
                  TopDockable(false).BottomDockable(false));
*/

/*
	mgr_.AddPane(tbSelection, wxAuiPaneInfo().
                  Name(wxT("tbSelection")).Caption(_("Selection")).
                  ToolbarPane().Right().
                  GripperTop().
                  TopDockable(false).BottomDockable(false));
*/
#endif QT_HIDE_FROM

    // main area
    //panBase_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	panBase_ = new QWidget(this);
	/*
    mgr_.AddPane(panBase_, wxAuiPaneInfo().
                  Name(wxT("panBase")).Caption(_("Base")).
                  CentrePane().PaneBorder(false));
	*/
	setCentralWidget(panBase_);


#ifdef QT_HIDE_FROM

    szBase_ = new wxBoxSizer( wxVERTICAL );

    //view_ = new ButcherView(panBase, wxID_ANY, wxPoint(10, 10), wxSize(100, 100), wxBORDER_SUNKEN);
    view_ = new ButcherViewEditor(panBase_, idView, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE );
    //view_->SetBackgroundColour(*wxBLUE);

/*
    viewzoom_->SetZoom(180);
    viewzoom_->SetParentView(view_);
*/

    szBase_->Add(
        view_,
        1,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        0 );         // set border width to 10

    view_->SetDesignWidth(201);
    view_->SetDesignHeight(201);

	//viewinfo_ = new ButcherControl_SelectionDisplay(panBase_, idViewInfo, wxDefaultPosition, wxSize(-1, 150));
	//szBase_->Add(viewinfo_, 0, wxEXPAND|wxALL, 0);

    szBase_->Hide(view_);
    //szBase_->Hide(viewinfo_);

    panBase_->SetSizer( szBase_ );
#endif QT_HIDE_FROM

    // "commit" all changes made to wxAuiManager
    //mgr_.Update();

    project_=new ButcherProject;
    project_->SetOptions(&options_);

	/*
    project_->Connect(wxID_ANY, wxEVT_BUTCHERPROJECT_ACTION,
        ButcherProjectEventHandler(HTMLButcherFrame::OnProjectEvent),
        NULL, this);
	*/
	connect(project_, SIGNAL(projectEvent(ButcherProjectEvent &)), this, SLOT(OnProjectEvent(ButcherProjectEvent &)));

    UpdateAppState();

    LoadViews();
    LoadModes();
    LoadFileAlternate();

    view_->SetProject(project_);

#ifdef HTMLBUTCHER_DEMO
	HTMLButcherDEMODialog demod(this, wxID_ANY);
	demod.ShowModal();
#endif
}



HTMLButcherFrame::~HTMLButcherFrame()
{
    if (progdlg_) {
        progdlg_->Destroy();
        progdlg_=NULL;
    }

    project_->Close();

	/*
    project_->Disconnect(wxID_ANY, wxEVT_BUTCHERPROJECT_ACTION,
        ButcherProjectEventHandler(HTMLButcherFrame::OnProjectEvent),
        NULL, this);
	*/
	disconnect(project_, SIGNAL(projectEvent(ButcherProjectEvent &)), this, SLOT(projectEvent(ButcherProjectEvent &)));

    view_->SetProject(NULL);

    delete project_;

    if (lineselect_) delete lineselect_;
    if (areaselect_) delete areaselect_;

    //mgr_.UnInit();
}


#ifdef QT_HIDE_FROM

void HTMLButcherFrame::OnClose(wxCloseEvent &event)
{
    if (event.CanVeto())
        if (!DoCloseProject()) {
            event.Veto();
            return;
        }

#ifdef HTMLBUTCHER_DEMO
	HTMLButcherDEMODialog demod(this, wxID_ANY);
	demod.ShowModal();
#endif

    Destroy();
}

#endif // QT_HIDE_FROM




void HTMLButcherFrame::OnMenuNew()
{
	if (!DoCloseProject()) return;



	project_->New();
    UpdateAppState();

	// Run new view wizard
#ifdef QT_HIDE_FROM
	HTMLButcherNewViewWizard nvwizard(this, wxID_ANY, project_);
	if (nvwizard.RunWizard(nvwizard.GetFirstPage()))
		SetView(nvwizard.GetViewId());
#endif QT_HIDE_FROM
}



#ifdef QT_HIDE_FROM

#ifndef HTMLBUTCHER_DEMO
void HTMLButcherFrame::OnMenuOpen(wxCommandEvent& event)
{

	if (!DoCloseProject()) return;

    wxFileDialog d(this, _("Load project"), wxEmptyString, wxEmptyString,
        _("HTMLButcher Project (*.hbp)|*.hbp|All files|*.*"), wxFD_OPEN  | wxFD_FILE_MUST_EXIST);
    if (d.ShowModal() == wxID_OK) {


        DoOpenProject(d.GetPath());
    }
}
#endif




void HTMLButcherFrame::OnMenuClose(wxCommandEvent& event)
{


	DoCloseProject();

    UpdateAppState();
}




#ifndef HTMLBUTCHER_DEMO
void HTMLButcherFrame::OnMenuSave(wxCommandEvent& event)
{
	if (!project_->IsOpen()) return;



    if (project_->GetFilename().IsEmpty())
    {
        OnMenuSaveAs(event);
        return;
    }
    project_->Save();

    UpdateAppState();
}
#endif




#ifndef HTMLBUTCHER_DEMO
void HTMLButcherFrame::OnMenuSaveAs(wxCommandEvent& event)
{
	if (!project_->IsOpen()) return;

    wxFileDialog d(this, _("Save project as"), wxEmptyString, _("project.hbp"),
        _("HTMLButcher Project (*.hbp)|*.hbp|All files|*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (d.ShowModal() == wxID_OK) {


        project_->SaveAs(d.GetPath());
        filehistory_.AddFileToHistory(d.GetPath());
        filehistory_.Save(*wxConfigBase::Get());
        UpdateAppState();
    }
}
#endif




void HTMLButcherFrame::OnMenuSaveForWeb(wxCommandEvent& event)
{
    if (!view_->GetProjectView()) return;

    wxFileDialog d(this, _("Save for web"), wxEmptyString, view_->GetProjectView()->GetDefaultFilename(),
        _("HTML files (*.html;*;htm)|*.html;*.htm|All files (*.*)|*.*"),
        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (d.ShowModal()==wxID_OK)
    {


		SetCursor(*wxHOURGLASS_CURSOR);
        try {
            view_->GetProjectView()->GenerateHTML(d.GetPath());
            //view_->GetProjectView()->GenerateHTML(wxT("c:\\temp\\work\\h.html"));

        } catch(...) {
            SetCursor(*wxSTANDARD_CURSOR);
            throw;
        }
        SetCursor(*wxSTANDARD_CURSOR);
    }
}



#ifndef HTMLBUTCHER_DEMO
void HTMLButcherFrame::OnMenuSaveForWebMultiple(wxCommandEvent& event)
{
    if (!project_->IsOpen()) return;

    wxFileName p(project_->GetFilename());
    wxDirDialog d(this, _("Select output directory"), p.GetPath());
    if (d.ShowModal()!=wxID_OK) return;



    //wxString dir=wxDirSelector(_("Select output directory"), wxEmptyString, 0, wxDefaultPosition, this);
    wxString dir=d.GetPath()+wxFileName::GetPathSeparator();
    if (dir.IsEmpty()) return;

    wxArrayString views;
    wxArrayInt sel, ids;

	for (ButcherProjectViews::iterator i=project_->Views().begin(); i!=project_->Views().end(); i++)
	{
        sel.Add(static_cast<unsigned long>(views.Add(i->GetName())));
        ids.Add(i.first());
	}

    if (wxGetMultipleChoices(sel, _("Select views to save:"), _("Save all for Web"), views, this)>0 &&
        sel.GetCount()>0)
    {
        wxBusyCursor bc;
        ButcherProjectBaseAutoProgress prog(project_, _("Generating HTML..."), ButcherProject::UF_GROUP);

        for (unsigned i=0; i<sel.GetCount(); i++)
            project_->Views().Get(ids[sel[i]])->GenerateHTML(dir+project_->Views().Get(ids[sel[i]])->GetDefaultFilename());
    }
}
#endif





void HTMLButcherFrame::OnMenuQuit(wxCommandEvent &event)
{
    Close();
}




#ifndef HTMLBUTCHER_DEMO
void HTMLButcherFrame::OnMenuHistory(wxCommandEvent &event)
{
    wxString f(filehistory_.GetHistoryFile(event.GetId() - wxID_FILE1));
    if (!f.empty())
    {
        if (!DoCloseProject()) return;

        project_->Open(f);
        UpdateAppState();
    }
}
#endif




void HTMLButcherFrame::OnHelpHelp(wxCommandEvent& event)
{
#ifdef BUTCHER_USE_HELP
	if (!static_cast<wxHelpControllerHelpProvider*>(wxHelpProvider::Get())->GetHelpController()->DisplayContents())
	{
		wxMessageBox(_("No help"));
	}
#endif
}




void HTMLButcherFrame::OnHelpAbout(wxCommandEvent& event)
{
	HTMLButcherAboutDialog d(this);
	d.ShowModal();
}

#ifdef HTMLBUTCHER_DEBUG
void HTMLButcherFrame::OnHelpTest(wxCommandEvent& event)
{
	//ButcherHTMLAttributeGenerator a(_("Maria=\"1\" jose'2 e demais' joaQUIm=3434"));
	ButcherHTMLAttributeGenerator a(wxT("maria: \"1 px\"; jose: '2 e demais'; joaquim: 3434 13px"), ButcherHTMLAttributeGenerator::KIND_STYLE);

	wxMessageBox(a.Generate(ButcherHTMLAttributeGenerator::KIND_TAG, wxT("ID")));
	wxMessageBox(a.Generate(ButcherHTMLAttributeGenerator::KIND_STYLE, wxT("ID")));
}
#endif





void HTMLButcherFrame::OnMenuMode(wxCommandEvent& event)
{
    wxComboBox* modesctrl=(wxComboBox*)FindWindow(idModeList);

    switch (event.GetId())
    {
    case idMenuModeNone:
        view_->SetDefaultMode(ButcherViewEditor::MODE_DEFAULT);
        //modesctrl->SetSelection(0);
        modesctrl->SetSelection(wxccu_control_getindex(modesctrl, ButcherViewEditor::MODE_DEFAULT));
        break;
    case idMenuModeLine:
        view_->SetDefaultMode(ButcherViewEditor::MODE_SELECTLINE);
        //modesctrl->SetSelection(1);
        modesctrl->SetSelection(wxccu_control_getindex(modesctrl, ButcherViewEditor::MODE_SELECTLINE));
        break;
    case idMenuModeArea:
        view_->SetDefaultMode(ButcherViewEditor::MODE_SELECTAREA);
        //modesctrl->SetSelection(2);
        modesctrl->SetSelection(wxccu_control_getindex(modesctrl, ButcherViewEditor::MODE_SELECTAREA));
        break;
    }
    UpdateAppState();
    //event.Skip();
}




void HTMLButcherFrame::OnMenuEditMode(wxCommandEvent& event)
{
    switch (event.GetId())
    {
    case idMenuEditModeBasic:
        options_.SetEditMode(ButcherOptions::EM_BASIC);
        break;
    case idMenuEditModeNormal:
        options_.SetEditMode(ButcherOptions::EM_NORMAL);
        break;
    case idMenuEditModeAdvanced:
        options_.SetEditMode(ButcherOptions::EM_ADVANCED);
        break;
    }
    options_.Save();

    UpdateAppState();
    //event.Skip();
}




void HTMLButcherFrame::OnMenuFiles(wxCommandEvent& event)
{


	HTMLButcherFilesDialog d(this, wxID_ANY, project_);
    d.ShowModal();
}




void HTMLButcherFrame::OnMenuMasks(wxCommandEvent& event)
{


	HTMLButcherMasksDialog d(this, wxID_ANY, project_);
    d.ShowModal();
}




void HTMLButcherFrame::OnMenuViews(wxCommandEvent& event)
{


	HTMLButcherViewsDialog d(this, wxID_ANY, project_);
    d.ShowModal();
}




void HTMLButcherFrame::OnMenuFilePaths(wxCommandEvent& event)
{


	HTMLButcherFilePathsDialog d(this, wxID_ANY, project_);
    d.ShowModal();
}




void HTMLButcherFrame::OnMenuAssortedFileGroups(wxCommandEvent& event)
{
    HTMLButcherAssortedFileGroupsDialog d(this, wxID_ANY, project_);
    d.ShowModal();
}




void HTMLButcherFrame::OnMenuAssortedFiles(wxCommandEvent& event)
{
    HTMLButcherAssortedFilesDialog d(this, wxID_ANY, project_);
    d.ShowModal();
}



void HTMLButcherFrame::OnMenuCSSFiles(wxCommandEvent& event)
{


	HTMLButcherCSSFilesDialog d(this, wxID_ANY, project_);
    d.ShowModal();
}




void HTMLButcherFrame::OnMenuViewCurrent(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;



    unsigned long vid=view_->GetProjectViewId();

    HTMLButcherViewEditDialog d(this, wxID_ANY, project_);
    d.Load(vid);
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(project_);
        d.Save(vid);
    }
}




void HTMLButcherFrame::OnMenuAreasCurrent(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;



    HTMLButcherMaskAreaListDialog d(this, wxID_ANY, project_);
    d.Load(view_->GetProjectView());
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(project_);
        d.Save(view_->GetProjectView());
    }
}

void HTMLButcherFrame::OnMenuWizNewView(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;

	// Run new view wizard
	HTMLButcherNewViewWizard nvwizard(this, wxID_ANY, project_);
	if (nvwizard.RunWizard(nvwizard.GetFirstPage()))
		SetView(nvwizard.GetViewId());
}

void HTMLButcherFrame::OnMenuProjectOptions(wxCommandEvent& event)
{


	HTMLButcherProjectOptionsDialog d(this, wxID_ANY, project_);
	d.Load();
	if (d.ShowModal()==wxID_OK)
		d.Save();
}




void HTMLButcherFrame::OnMenuMaskCurrent(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;



    unsigned long mid=view_->GetProjectView()->GetMaskId();

    HTMLButcherMaskEditDialog d(this, wxID_ANY, project_);
    //d.SetProject(project_);
    d.SetMaskName(view_->GetProjectView()->GetMask()->GetName());
    d.SetMaskWidth(view_->GetProjectView()->GetMask()->GetWidth());
    d.SetMaskHeight(view_->GetProjectView()->GetMask()->GetHeight());
    d.Load(view_->GetProjectView()->GetMask());
    if (d.ShowModal() == wxID_OK) {
        ButcherProjectBaseAutoUpdate upd(project_);

        project_->Masks().Edit(mid, d.GetMaskName(), d.GetMaskWidth(),
            d.GetMaskHeight());
        d.Save(view_->GetProjectView()->GetMask());
    }
}




void HTMLButcherFrame::OnMenuImageFormats(wxCommandEvent& event)
{


	HTMLButcherImageFormatListEditDialog d(this, wxID_ANY, project_);
    if (view_->GetProjectView()!=NULL)
        d.SetPreview(view_->GetProjectFile()->CreateImage());
    d.ShowModal();
}




void HTMLButcherFrame::OnMenuZoom(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;

    long z;

    switch (event.GetId()) {
    case idMenuZoomIn:
        view_->SetZoom(view_->GetZoom() + 10);
        break;
    case idMenuZoomOut:
        if (view_->GetZoom() > 10)
            view_->SetZoom(view_->GetZoom() - 10);
        break;
    case idMenuZoomNormal:
        view_->SetZoom(100);
        break;
    case idMenuZoom:
        z=wxGetNumberFromUser(_("Enter zoom"), _("Zoom: 1-500%"), _("Zoom"),
            view_->GetZoom(), 1, 500, this);
        if (z>1)
            view_->SetZoom(z);
        break;
    }
}



void HTMLButcherFrame::OnMenuViewSelect(wxCommandEvent& event)
{
    SetView(event.GetId()-idMenuViewSelect);
}




void HTMLButcherFrame::OnMenuShowPreview(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;
    view_->SetShowPreview(!view_->GetShowPreview());
    //event.Skip();
}




void HTMLButcherFrame::OnMenuShowBorders(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;
    view_->SetShowBorders(!view_->GetShowBorders());
    //event.Skip();
}




void HTMLButcherFrame::OnMenuShowAreas(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;
    view_->SetAreaView(view_->GetAreaView() ^ ButcherView::AV_AREA); // XOR
    //event.Skip();
}




void HTMLButcherFrame::OnMenuShowAreasGlobal(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;
    view_->SetAreaView(view_->GetAreaView() ^ ButcherView::AV_AREAGLOBAL); // XOR
    //event.Skip();
}




void HTMLButcherFrame::OnMenuShowAreasMap(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;
    view_->SetAreaView(view_->GetAreaView() ^ ButcherView::AV_AREAMAP); // XOR
    //event.Skip();
}




void HTMLButcherFrame::OnMenuGrid(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;
    view_->SetShowGrid(!view_->GetShowGrid());
    //event.Skip();
}




void HTMLButcherFrame::OnMenuGridSize(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;

    wxTextEntryDialog d(this, _("Enter grid size"), _("Grid Size"), wxString::Format(wxT("%d"), view_->GetGridSize()));
    if (d.ShowModal() == wxID_OK) {
        int a=wxAtoi(d.GetValue());
        if (a>0)
            view_->SetGridSize(a);
    }
    //event.Skip();
}

#if NEED_CHOOSELANG_UI
void HTMLButcherFrame::OnMenuLanguage(wxCommandEvent& event)
{
	ChangeUILanguage();
}
#endif




void HTMLButcherFrame::OnMenuFileAlternate(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;
    view_->SetFileAlternate(!view_->GetFileAlternate());
    LoadFileAlternate();
    //event.Skip();
}




void HTMLButcherFrame::OnMenuOptions(wxCommandEvent& event)
{
    HTMLButcherOptionsDialog d(this);
    d.Load(&options_);
    if (d.ShowModal()==wxID_OK)
    {
        d.Save(&options_);
        options_.Save();
    }
}




void HTMLButcherFrame::OnMenuOperation(wxCommandEvent& event)
{
    if (view_->GetProjectView() == NULL) return;



    if (event.GetId() == idMenuInsertLineVertical)
        view_->StartOperation(ButcherViewEditor::OP_INSERTVLINE);
    else if (event.GetId() == idMenuInsertLineHorizontal)
        view_->StartOperation(ButcherViewEditor::OP_INSERTHLINE);
    else if (event.GetId() == idMenuMoveLine)
        view_->StartOperation(ButcherViewEditor::OP_MOVELINE);
    else if (event.GetId() == idMenuDeleteLine)
        view_->StartOperation(ButcherViewEditor::OP_DELETELINE);
    else if (event.GetId() == idMenuInsertAreaGlobal)
        view_->StartOperation(ButcherViewEditor::OP_INSERTAREAGLOBAL);
    else if (event.GetId() == idMenuDeleteAreaGlobal)
        view_->StartOperation(ButcherViewEditor::OP_DELETEAREAGLOBAL);
    else if (event.GetId() == idMenuInsertAreaMap)
        view_->StartOperation(ButcherViewEditor::OP_INSERTAREAMAP);
    else if (event.GetId() == idMenuDeleteAreaMap)
        view_->StartOperation(ButcherViewEditor::OP_DELETEAREAMAP);
    else if (event.GetId() == idMenuCancelOperation)
        view_->StopOperation();
    //event.Skip();
}

#endif // QT_HIDE_FROM


#ifndef HTMLBUTCHER_DEMO
void HTMLButcherFrame::DoOpenProject(const wxString &filename)
{
    if (!DoCloseProject()) return;

    project_->Open(filename);
    filehistory_.AddFileToHistory(filename);
    filehistory_.Save(*wxConfigBase::Get());
    UpdateAppState();
}
#endif




bool HTMLButcherFrame::DoCloseProject()
{
    if (!project_->IsOpen()) return true;

#ifdef QT_HIDE_FROM

#ifndef HTMLBUTCHER_DEMO
	if (project_->GetModified()) {
        wxMessageDialog d(this, _("Current project is not saved, close anyway?"),
            _("Warning"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
        if (d.ShowModal() != wxID_YES) return false;
    }
#endif

#endif // QT_HIDE_FROM

    project_->Close();

    UpdateAppState();
    return true;
}




void HTMLButcherFrame::UpdateAppState()
{
#ifdef QT_HIDE_FROM

    wxMenuBar *menu = GetMenuBar();
    wxToolBar *tbFile = (wxToolBar*)FindWindow(idTBFile);
#if defined(__WXMAC__)
    wxToolBar *tbView = tbFile;
#else
    wxToolBar *tbView = (wxToolBar*)FindWindow(idTBView);
#endif

    wxToolBar *tbTools = (wxToolBar*)FindWindow(idTBTools);
    wxComboBox* viewsctrl=(wxComboBox*)FindWindow(idViewList);
    wxComboBox* modesctrl=(wxComboBox*)FindWindow(idModeList);
    wxComboBox* filealternatectrl=(wxComboBox*)FindWindow(idFileAlternateList);

    bool isactive=project_->IsOpen();
    bool isview=isactive && view_->GetProjectViewId()!=0;
    bool isoperation=view_->GetOperation()!=ButcherViewEditor::OP_NONE;

    menu->Enable(idMenuNew, true);
    menu->Enable(idMenuClose, isactive);
#ifndef HTMLBUTCHER_DEMO
    menu->Enable(idMenuOpen, true);
    menu->Enable(idMenuSave, isactive);
    menu->Enable(idMenuSaveAs, isactive);
#endif
    menu->Enable(idMenuSaveForWeb, isactive && isview);
#ifndef HTMLBUTCHER_DEMO
    menu->Enable(idMenuSaveForWebMultiple, isactive);
#endif

    menu->Enable(idMenuModeNone, isactive && isview);
    menu->Enable(idMenuModeLine, isactive && isview);
    menu->Enable(idMenuModeArea, isactive && isview);
    menu->Check(idMenuModeNone, isactive && isview && view_->GetDefaultMode() == ButcherViewEditor::MODE_DEFAULT);
    menu->Check(idMenuModeLine, isactive && isview && view_->GetDefaultMode() == ButcherViewEditor::MODE_SELECTLINE);
    menu->Check(idMenuModeArea, isactive && isview && view_->GetDefaultMode() == ButcherViewEditor::MODE_SELECTAREA);

    menu->Check(idMenuEditModeBasic, options_.GetEditMode() == ButcherOptions::EM_BASIC);
    menu->Check(idMenuEditModeNormal, options_.GetEditMode() == ButcherOptions::EM_NORMAL);
    menu->Check(idMenuEditModeAdvanced, options_.GetEditMode() == ButcherOptions::EM_ADVANCED);

    menu->Enable(idMenuFiles, isactive);
    menu->Enable(idMenuMasks, isactive);
    menu->Enable(idMenuViews, isactive);
    menu->Enable(idMenuFilePaths, isactive);
    menu->Enable(idMenuAssortedFileGroups, isactive);
    menu->Enable(idMenuAssortedFiles, isactive);
    menu->Enable(idMenuCSSFiles, isactive);

    menu->Enable(idMenuImageFormats, isactive);
    menu->Enable(idMenuMaskCurrent, isactive && isview);
    menu->Enable(idMenuViewCurrent, isactive && isview);
    menu->Enable(idMenuAreasCurrent, isactive && isview);

	menu->Enable(idMenuWizNewView, isactive &&
		(!project_->Files().IsFull() &&
		 !project_->Masks().IsFull() &&
		 !project_->Views().IsFull()));

    menu->Enable(idMenuProjectOptions, isactive);

    menu->Enable(idMenuViewSelect, isactive);
    menu->Enable(idMenuZoomIn, isactive && isview);
    menu->Enable(idMenuZoomOut, isactive && isview);
    menu->Enable(idMenuZoomNormal, isactive && isview);
    menu->Enable(idMenuZoom, isactive && isview);

    menu->Enable(idMenuFileAlternate, isactive && isview && view_->GetProjectView()->HaveFileAlternate());

    menu->Check(idMenuFileAlternate, isactive && isview && view_->GetProjectView()->HaveFileAlternate() && view_->GetFileAlternate());

    menu->Enable(idMenuShowPreview, isactive && isview);
    menu->Enable(idMenuShowBorders, isactive && isview);
    menu->Enable(idMenuShowAreas, isactive && isview && view_->GetProjectViewShowAreas());
    menu->Enable(idMenuShowAreasGlobal, isactive && isview && view_->GetProjectViewShowAreasGlobal());
    menu->Enable(idMenuShowAreasMap, isactive && isview && view_->GetProjectViewShowAreas());
    menu->Enable(idMenuGrid, isactive && isview);
    menu->Enable(idMenuGridSize, isactive && isview);
    menu->Check(idMenuGrid, isactive && isview && view_->GetShowGrid());
    menu->Check(idMenuShowPreview, isactive && isview && view_->GetShowPreview());
    menu->Check(idMenuShowBorders, isactive && isview && view_->GetShowBorders());
    //menu->Check(idMenuShowAreas, isactive && isview && view_->GetShowAreas());
    //menu->Check(idMenuShowAreasGlobal, isactive && isview && view_->GetShowAreasGlobal());
    menu->Check(idMenuShowAreas, isactive && isview && ((view_->GetAreaView()&ButcherView::AV_AREA)==ButcherView::AV_AREA));
    menu->Check(idMenuShowAreasGlobal, isactive && isview && ((view_->GetAreaView()&ButcherView::AV_AREAGLOBAL)==ButcherView::AV_AREAGLOBAL));
    menu->Check(idMenuShowAreasMap, isactive && isview && ((view_->GetAreaView()&ButcherView::AV_AREAMAP)==ButcherView::AV_AREAMAP));

    viewsctrl->Enable(isactive);
    modesctrl->Enable(isview && !isoperation);
    filealternatectrl->Enable(isview);

#ifndef HTMLBUTCHER_DEMO
    tbFile->EnableTool(idMenuSave, isactive);
#endif
    tbView->EnableTool(idMenuZoomIn, isactive && isview);
    tbView->EnableTool(idMenuZoomOut, isactive && isview);
    tbView->EnableTool(idMenuZoomNormal, isactive && isview);

    tbTools->EnableTool(idMenuInsertLineHorizontal, isview && !isoperation);
    tbTools->EnableTool(idMenuInsertLineVertical, isview && !isoperation);
    tbTools->EnableTool(idMenuDeleteLine, isview && !isoperation);
    tbTools->EnableTool(idMenuMoveLine, isview && !isoperation);
    tbTools->EnableTool(idMenuInsertAreaGlobal, isview && !isoperation && view_->GetProjectViewShowAreasGlobal());
    tbTools->EnableTool(idMenuDeleteAreaGlobal, isview && !isoperation && view_->GetProjectViewShowAreasGlobal());
    tbTools->EnableTool(idMenuInsertAreaMap, isview && !isoperation);
    tbTools->EnableTool(idMenuDeleteAreaMap, isview && !isoperation);
    tbTools->EnableTool(idMenuCancelOperation, isview && isoperation);

#ifdef HTMLBUTCHER_DEMO
	wxString atit=wxString::Format(wxT("HTMLButcher DEMO %d.%d"), HTMLBUTCHERVERSION_1, HTMLBUTCHERVERSION_2);
#else
	wxString atit=wxString::Format(wxT("HTMLButcher %d.%d"), HTMLBUTCHERVERSION_1, HTMLBUTCHERVERSION_2);
#endif

    if (isactive)
    {
        wxString fn=project_->GetFilename();
        if (fn.IsEmpty())
            fn=_("[New Project]");
        else
        {
            wxFileName f(fn);
            fn=f.GetFullName();
        }

        SetTitle(wxString::Format(wxT("%s - %s%s"), atit.c_str(), fn.c_str(), (project_->GetModified()?wxT("*"):wxEmptyString)));
    }
    else
    {
        SetTitle(atit);
    }

    if (isactive) {
        SetStatusText((project_->GetModified()?_("Modified"):wxT("")), 0);
    }

    if (!isactive) SetStatusText(wxEmptyString, 0);
    if (!isview) SetStatusText(wxEmptyString, 1);
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::OnProjectEvent(ButcherProjectEvent& event)
{
    //wxMessageBox(_("Project event"), _("Event"), wxOK | wxICON_INFORMATION);

    switch (event.GetEvent()) {
    case ButcherProjectEvent::BPE_PROJECTOPEN:
    case ButcherProjectEvent::BPE_VIEWMODIFIED:
        LoadViews(true); // reload the views toolbar
        LoadFileAlternate(); // reload the file alternate toolbar
        UpdateAppState(); // items can have been changed
        break;
    case ButcherProjectEvent::BPE_FILEMODIFIED:
    case ButcherProjectEvent::BPE_MASKMODIFIED:
        LoadViews(true); // reload the views toolbar
        UpdateAppState(); // items can have been changed
        break;
    case ButcherProjectEvent::BPE_VIEWDELETED:
        if (event.GetEId()==view_->GetProjectViewId())
			SetView(0);
		LoadViews(false); // reload the views toolbar
        break;
    case ButcherProjectEvent::BPE_PROJECTCLOSE:
        LoadViews(); // clean the views toolbar
        LoadFileAlternate();
        break;
    case ButcherProjectEvent::BPE_COLORSCHEMEMODIFIED:
    case ButcherProjectEvent::BPE_COLORSCHEMEDELETED:
    case ButcherProjectEvent::BPE_ASSORTEDFILEMODIFIED:
    case ButcherProjectEvent::BPE_ASSORTEDFILEDELETED:
    case ButcherProjectEvent::BPE_ASSORTEDFILEGROUPMODIFIED:
    case ButcherProjectEvent::BPE_ASSORTEDFILEGROUPDELETED:
        UpdateAppState(); // items can have been changed
        break;
    case ButcherProjectEvent::BPE_PROGRESS:
        UpdateProgress();
        break;
    default:
        break;
    }
    //event.Skip(); // IMPORTANT!
}



void HTMLButcherFrame::LoadViews(bool check)
{
#ifdef QT_HIDE_FROM
    wxComboBox* viewsctrl=(wxComboBox*)FindWindow(idViewList);
    wxMenuItem *mitem=GetMenuBar()->FindItem(idMenuViewSelect), *newmitem;

    wxString desc;
    wxMenu *viewmenu=NULL;
    wxMenuItem *newmenu;

    if (!check)
    {
        viewmenu=new wxMenu(wxEmptyString);

        viewsctrl->Clear();

        wxccu_control_append(viewsctrl, _("- Select view -"), 0);
    }

    if (project_->IsOpen()) {
		for (ButcherProjectViews::iterator i=project_->Views().begin(); i!=project_->Views().end(); i++)
		{
            desc=wxString::Format(wxT("%s [%s]"),
                    i->GetName().c_str(),
                    i->GetMask()->GetName().c_str());
                    //i->GetMask()->Configs().Get(i->GetMaskConfigId())->GetName().c_str());

            if (!check)
            {
                wxccu_control_append_select(viewsctrl, desc, i.first(), view_->GetProjectViewId());

                newmenu=viewmenu->AppendRadioItem(idMenuViewSelect+(i.first()), desc, desc);
            }
            else
            {
               if (wxccu_control_getindex(viewsctrl, i.first()==wxNOT_FOUND) ||
                    viewsctrl->GetString(wxccu_control_getindex(viewsctrl, i.first()))!=desc)
                {
                    LoadViews(false);
                    return;
                }
            }

		}
    }

    if (!check)
    {
        newmitem=new wxMenuItem(mitem->GetMenu(), idMenuViewSelect, _("&View"), wxEmptyString, wxITEM_NORMAL, viewmenu);
        wxccu_control_menu_replace(mitem, newmitem);


        if (project_->IsOpen()) {
            if (view_->GetProjectViewId()>0)
                newmitem->GetSubMenu()->Check(idMenuViewSelect+view_->GetProjectViewId(), true);
        }

        if (viewsctrl->GetSelection() == wxNOT_FOUND) {
            viewsctrl->SetSelection(0);
            SetView(0);
        }
    }
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::SetView(unsigned long id)
{
#ifdef QT_HIDE_FROM
    if (project_->IsOpen() && id>0) {
        if (view_->GetProjectViewId() == id) return;

		if (viewinfo_) viewinfo_->ClearSelectEvent();
		view_->SetProjectViewId(id);

        wxMenuItem *mitem=GetMenuBar()->FindItem(idMenuViewSelect);
        //GetMenuBar()->Check(idMenuViewSelect+view_->GetProjectViewId()+1, true);
        mitem->GetSubMenu()->Check(idMenuViewSelect+id, true);

        szBase_->Show(view_);
        //szBase_->Show(viewinfo_);
        szBase_->Layout();
        view_->Refresh();

        view_->SetFocus();
    } else {
		if (viewinfo_) viewinfo_->ClearSelectEvent();
        szBase_->Hide(view_);
        //szBase_->Hide(viewinfo_);

        view_->SetProjectViewId(id);
    }
	// set combo value
	wxComboBox *viewsctrl=(wxComboBox *)FindWindow(idViewList);
    if (viewsctrl->GetSelection() == wxNOT_FOUND || viewsctrl->GetSelection() != wxccu_control_getindex(viewsctrl, id)) {
        viewsctrl->SetSelection(wxccu_control_getindex(viewsctrl, id));
    }

    LoadFileAlternate();

    UpdateAppState();
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::LoadModes()
{
#ifdef QT_HIDE_FROM
    wxComboBox* modesctrl=(wxComboBox*)FindWindow(idModeList);

    modesctrl->Clear();

    wxccu_control_append(modesctrl, _("- NONE -"), ButcherViewEditor::MODE_DEFAULT);
    wxccu_control_append(modesctrl, _("Line"), ButcherViewEditor::MODE_SELECTLINE);
    wxccu_control_append(modesctrl, _("Area"), ButcherViewEditor::MODE_SELECTAREA);

    view_->SetDefaultMode(ButcherViewEditor::MODE_SELECTAREA);
    modesctrl->SetSelection(2);
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::LoadFileAlternate()
{
#ifdef QT_HIDE_FROM
	wxComboBox* factrl = (wxComboBox*)FindWindow(idFileAlternateList);

    factrl->Clear();

    wxString dfile;
    if (view_->GetProjectViewId()!=0)
        dfile=wxString::Format(wxT("* - %s"), view_->GetProjectView()->GetFile()->GetDisplayName().c_str());
    else
        dfile=_("- DEFAULT -");

    wxccu_control_append(factrl, dfile, 0);

    if (project_->IsOpen() && view_->GetProjectView() && view_->GetProjectView()->HaveFileAlternate())
    {
        for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        {
            if (view_->GetProjectView()->GetFileAlternate(i))
            {
                wxccu_control_append_select(factrl, wxString::Format(wxT("%d - %s"),
                        i+1,
                        view_->GetProjectView()->GetFileAlternate(i)->GetDisplayName().c_str()),
                        i+1, (view_->GetFileAlternate()?view_->GetFileAlternateId()+1:0) );
            }
        }
    }

    if (factrl->GetSelection() == wxNOT_FOUND) {
        factrl->SetSelection(0);
    }
#endif // QT_HIDE_FROM
}



void HTMLButcherFrame::OnViewSelect(wxCommandEvent& event)
{
#ifdef QT_HIDE_FROM
	wxComboBox* viewsctrl = (wxComboBox*)event.GetEventObject();

    if (viewsctrl->GetSelection() != wxNOT_FOUND) {
        view_->SetFocus();
        SetView(wxccu_control_getselectedid(viewsctrl, 0));
    }
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::OnModeSelect(wxCommandEvent& event)
{
#ifdef QT_HIDE_FROM
	wxComboBox* modesctrl = (wxComboBox*)event.GetEventObject();

    if (modesctrl->GetSelection() != wxNOT_FOUND) {
        view_->SetDefaultMode(wxccu_control_getselectedid(modesctrl, ButcherViewEditor::MODE_DEFAULT));
        view_->Refresh();
        view_->SetFocus();

        UpdateAppState();
    }
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::OnFileAlternateSelect(wxCommandEvent& event)
{
#ifdef QT_HIDE_FROM
	wxComboBox* factrl = (wxComboBox*)event.GetEventObject();

    if (factrl->GetSelection() != wxNOT_FOUND) {
        FileAlternateSelect(wxccu_control_getselectedid(factrl, 0));
    }
    view_->SetFocus();
#endif // QT_HIDE_FROM
}





void HTMLButcherFrame::OnDocumentMouse(ButcherDocumentMouseEvent& event)
{
#ifdef QT_HIDE_FROM
	if (event.GetOriginEventType() == wxEVT_MOTION)
        SetStatusText(wxString::Format(_("X: %d Y: %d"), event.GetX(), event.GetY()), 1);
    event.Skip();
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::OnDocumentKey(ButcherDocumentKeyEvent& event)
{
    if (event.GetOriginEventType() == wxEVT_KEY_DOWN) {
        switch (event.GetKeyCode()) {
        case WXK_ADD:
            view_->SetZoom(view_->GetZoom()+10);
            break;
        case WXK_SUBTRACT:
            view_->SetZoom(view_->GetZoom()-10);
            break;
        case WXK_DIVIDE:
            view_->SetZoom(100);
            break;
        default:
            break;
        }
        if (event.ControlDown())
        {
            for (int i=0; i<=BUTCHERCONST_VIEW_MAXALTERNATE; i++)
            {
                if (event.GetKeyCode()==i+48)
                {
                    FileAlternateSelect(i);
                }
            }
        }
    }
    event.Skip();
}




void HTMLButcherFrame::OnBViewSelect(ButcherViewSelectEvent& event)
{
#ifdef QT_HIDE_FROM
	wxString msg = wxEmptyString;
    wxString temp;

    if (event.GetSelect() == ButcherViewSelectEvent::SEL_LINEHOVER) {
		if (viewinfo_) viewinfo_->SetSelectEvent(event);
        //ButcherProjectMask *ms;

        for (unsigned int i=0; i<event.GetLineSelect()->GetCount(); i++)
        {
            msg+=wxString::Format(wxT("[%s]"), event.GetLineSelect()->GetItem(i)->GetFullDescription().c_str());
        }
        SetStatusText(msg, 3);

        //SetStatusText(wxString::Format(_("Line %d"), view_->GetProjectView()->GetMask()->Lines().Get(event.GetSelItem())->GetBLId()), 3);
    } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_AREAHOVER) {
		if (viewinfo_) viewinfo_->SetSelectEvent(event);
        //ButcherProjectMask *ms;

        for (unsigned int i=0; i<event.GetAreaSelect()->GetCount(); i++)
        {
            msg+=wxString::Format(wxT("[%s(%s)]"), event.GetAreaSelect()->GetItem(i)->GetFullDescription().c_str(),
                event.GetAreaSelect()->GetItem(i)->Configs().Get(view_->GetProjectView()->GetBLId())->GetFlagsDescription().c_str());
        }
        SetStatusText(msg, 3);


        //SetStatusText(wxString::Format(_("Area %d"), view_->GetProjectView()->GetMask()->Areas().Get(event.GetSelItem())->GetBLId()), 3);
    } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_NONE) {
		if (viewinfo_) viewinfo_->SetSelectEvent(event);
        SetStatusText(wxEmptyString, 3);
    } else if (view_->GetOperation() == ButcherViewEditor::OP_NONE && event.GetSelect() == ButcherViewSelectEvent::SEL_LINEEDIT) {
        ButcherProjectProc_LineMenu lmenu(project_, this, event.GetLineSelect(),
            view_->GetProjectView(), view_->GetShowPreview());
        lmenu.ShowLineMenu();
    } else if (view_->GetOperation() == ButcherViewEditor::OP_NONE && event.GetSelect() == ButcherViewSelectEvent::SEL_LINE) {
        // simulate line selection
        view_->StartOperation(ButcherViewEditor::OP_MOVELINE);
        view_->DoSelectEvent(ButcherViewSelectEvent::SEL_LINE, 0, 0, new ButcherProjectMaskLineSelect(*event.GetLineSelect()));
    } else if (view_->GetOperation() == ButcherViewEditor::OP_NONE && event.GetSelect() == ButcherViewSelectEvent::SEL_AREAEDIT) {
        ButcherProjectProc_AreaMenu amenu(project_, this, event.GetAreaSelect(),
            view_->GetProjectView(), view_->GetShowPreview());
        amenu.ShowAreaMenu();
    }
    event.Skip();
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::OnButcherStatus(ButcherStatusEvent& event)
{
#ifdef QT_HIDE_FROM
	switch (event.GetStatus()) {
    case ButcherStatusEvent::ST_OPERATION:
        SetStatusText(event.GetMessage(), 2);
        break;
    default:
        break;
    }
#endif // QT_HIDE_FROM
}




void HTMLButcherFrame::OnButcherOperation(wxCommandEvent& event)
{
    UpdateAppState();
}




void HTMLButcherFrame::FileAlternateSelect(int index, bool refreshlist)
{
#ifdef QT_HIDE_FROM
	if (view_->GetProjectView() == NULL) return;

    if (index==0)
    {
        view_->SetFileAlternate(false, true);
    }
    else
    {
        view_->SetFileAlternate(true, false);
        view_->SetFileAlternateId(index-1, true);
    }

    if (refreshlist)
        LoadFileAlternate();
#endif // QT_HIDE_FROM
}



void HTMLButcherFrame::UpdateProgress()
{
    if (!project_->IsOpen()) return;

    if (project_->ProgressCount()>0)
    {
        if (!progdlg_) {
            if (project_->ProgressGroupItemCount()>0)
            {
                progdlg_=new HTMLButcherProgressDialog(NULL);//this);
                progdlg_->Show();
                progdlg_->Update();

                wxBeginBusyCursor();
            }
        } else {
            progdlg_->UpdateProgress(project_);
        }
    }
    else
    {
        if (progdlg_) {
            wxEndBusyCursor();

            progdlg_->Destroy();
            progdlg_=NULL;
            view_->SetFocus();
        }
    }
}


