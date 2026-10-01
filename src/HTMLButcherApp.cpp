//=============================================================================
/**
 *  @file    HTMLButcherApp.cpp
 *
 *  $Id: HTMLButcherApp.cpp,v 1.14 2008/06/09 14:27:27 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-11-29
 */
//=============================================================================

/*
#if defined(_MSC_VER) && !defined(_WIN64)
// visual leak detector
#include <vld.h>
#endif
*/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <stdexcept>
#include "HTMLButcherSplash.h"
#include "HTMLButcherApp.h"
#include "HTMLButcherMain.h"

#include "wxFreeImage.h"
#include "BLanguage.h"

#include <wx/wfstream.h>
#include <wx/xrc/xmlres.h>
#include <wx/stdpaths.h>
#include <wx/cmdline.h>
#include <wx/sysopt.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include <wx/fs_arc.h>


#ifdef __WXGTK__
    // XPM icon
    #include "../resources/htmlbutcher.xpm"
#endif

wxIMPLEMENT_APP(HTMLButcherApp);



bool HTMLButcherApp::OnInit()
{
	if (!wxApp::OnInit()) return false;

#if defined(__WXMSW__) || !defined(HTMLBUTCHER_DEMO)
	SetAppName("HTMLButcher");
#else
	SetAppName("HTMLButcher-Demo");
#endif

#ifdef HTMLBUTCHER_DEBUG
	wxFileName kf(wxStandardPaths::Get().GetExecutablePath());
	kf.SetFullName("default.po");
	kf.AppendDir("..");
	kf.AppendDir("..");
	kf.AppendDir("locale");
	kf.Normalize(wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE);

	locale_.AddCatalogLookupPathPrefix(kf.GetPath());
#endif
	locale_.AddCatalogLookupPathPrefix(wxStandardPaths::Get().GetDataDir() + "/share/locale");


	locale_.Init(GetUILanguage());
		
	locale_.AddCatalog("htmlbutcher");

#ifdef __WXGTK__
    wxFileName::Mkdir(wxStandardPaths::Get().GetUserDataDir(), 0777, wxPATH_MKDIR_FULL); // must create dir for wxConfig
    wxConfig::Set(new wxFileConfig(wxEmptyString, wxEmptyString,
        wxFileName(wxStandardPaths::Get().GetUserDataDir(), "config").GetFullPath(),
        wxEmptyString, wxCONFIG_USE_LOCAL_FILE));
#endif



    extern void InitXmlResource(); // defined in htmlbutcher_resources.cpp
    wxXmlResource::Get()->InitAllHandlers();
    InitXmlResource();

    wxInitAllImageHandlers();
    wxFileSystem::AddHandler(new wxArchiveFSHandler);

    //::wxHandleFatalExceptions(true);

	wxFreeImage::Initialize();

#ifdef BUTCHER_USE_HELP
	// help
    wxHelpControllerHelpProvider* provider = new wxHelpControllerHelpProvider;
    wxHelpProvider::Set(provider);
    provider->SetHelpController(&help_);
#ifdef __WXMSW__
	wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
	fn.SetFullName("htmlbutcher.chm");
#else
#ifdef HTMLBUTCHER_DEBUG
	wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
	fn.SetFullName("htmlbutcher.htb");
#else
	wxFileName fn(wxStandardPaths::Get().GetResourcesDir().Lower(), wxEmptyString);
	fn.SetFullName("htmlbutcher.htb");
    if (!fn.FileExists())
    {
        fn.AssignDir(wxString::Format("/usr/share/%s", wxTheApp->GetAppName().Lower()));
        fn.SetFullName("htmlbutcher.htb");
    }

#endif
#endif
	if (!help_.Initialize(fn.GetFullPath()))
	{
        wxLogError("Cannot initialize the help system, aborting.");
        return false;
	}
#endif


	// main frame
	HTMLButcherFrame* frame = new HTMLButcherFrame(0L, "HTMLButcher");

	// splash screen
    {
        wxLogNull logNo;
        wxBitmap rbitmap(wxXmlResource::Get()->LoadBitmap("b_splash"));

        wxImage simage(rbitmap.ConvertToImage());
        simage.ConvertAlphaToMask();
        wxBitmap bitmap(simage);
        HTMLButcherSplashScreen *splash = new HTMLButcherSplashScreen(bitmap,
                wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
                4000, frame, -1, wxDefaultPosition, wxDefaultSize,
                wxBORDER_NONE|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP|wxSPLASH_CENTRE_ON_SCREEN|wxFRAME_SHAPED);
        splash->Update();
    }


	//wxYield();

#ifdef __WXMSW__
    frame->SetIcon(wxICON(HTMLBUTCHERICON)); // To Set App Icon
#elif defined(__WXGTK__)
    frame->SetIcon(wxICON(htmlbutcher)); // To Set App Icon
#endif //__WXMSW__
    frame->Show();
    SetTopWindow(frame);
//#ifndef __WXMAC__
	frame->Maximize();
//#endif

#ifndef HTMLBUTCHER_DEMO
    if (!openfile_.IsEmpty())
        frame->DoOpenProject(openfile_);
#endif

    return true;
}



int HTMLButcherApp::OnExit()
{
    wxFreeImage::Finalize();

    return wxApp::OnExit();
}



void HTMLButcherApp::OnFatalException()
{
    wxMessageBox(_("Fatal error - please restart the application"),
                 "HTMLButcher", wxOK | wxICON_ERROR);

}

bool HTMLButcherApp::OnExceptionInMainLoop()
{
    // this shows how we may let some exception propagate uncaught
    try
    {
        throw;
    }
    catch ( ButcherException& e )
    {
        wxMessageBox(e.what(),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( wxFreeImageException& e )
    {
        wxMessageBox(e.what(),
                     "HTMLButcher*", wxOK | wxICON_ERROR);
    }
    catch ( std::runtime_error& e )
    {
        wxMessageBox(wxString(e.what(), wxConvUTF8),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( ... )
    {
        wxMessageBox(_("Unhandled exception caught, program will terminate."),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    return true;
}




void HTMLButcherApp::OnUnhandledException()
{
    // this shows how we may let some exception propagate uncaught
    try
    {
        throw;
    }
    catch ( ButcherException& e )
    {
        wxMessageBox(e.what(),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( wxFreeImageException& e )
    {
        wxMessageBox(e.what(),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( std::runtime_error& e )
    {
        wxMessageBox(wxString(e.what(), wxConvUTF8),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( ... )
    {
        wxMessageBox(_("Unhandled exception caught, program will terminate."),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }

}

int HTMLButcherApp::OnRun()
{
    try {
        wxApp::OnRun();
    }
    catch ( ButcherException& e )
    {
        wxMessageBox(e.what(),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( wxFreeImageException& e )
    {
        wxMessageBox(e.what(),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( std::runtime_error& e )
    {
        wxMessageBox(wxString(e.what(), wxConvUTF8),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( ... )
    {
        wxMessageBox(_("Unhandled exception caught, program will terminate."),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }

    return 0;
}

#if defined(__WXGTK__) || defined(__WXMAC__)
void HTMLButcherApp::HandleEvent(wxEvtHandler *handler,
    wxEventFunction func,
    wxEvent& event) const
{
    try {
        wxApp::HandleEvent(handler, func, event);
    }
    catch ( ButcherException& e )
    {
        wxMessageBox(e.what(),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( wxFreeImageException& e )
    {
        wxMessageBox(e.what(),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( std::runtime_error& e )
    {
        wxMessageBox(wxString(e.what(), wxConvUTF8),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
    catch ( ... )
    {
        wxMessageBox(_("Unhandled exception caught, program will terminate."),
                     "HTMLButcher", wxOK | wxICON_ERROR);
    }
}
#endif



static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_SWITCH, "h", "help", _("displays help on the command line parameters"),
          wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
	 { wxCMD_LINE_OPTION, "l", "license-file",   "license file" },
     { wxCMD_LINE_PARAM,  NULL, NULL, _("input project file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
     { wxCMD_LINE_NONE }
};




void HTMLButcherApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars ("-");
}




bool HTMLButcherApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    //silent_mode = parser.Found("s");
	if (!parser.Found("l", &licfile_))
		licfile_.clear();

    // open file name
    if (parser.GetParamCount()>0)
        openfile_=parser.GetParam(0);

    return true;
}



