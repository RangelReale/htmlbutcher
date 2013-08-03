/***************************************************************
 * Name:      MetadataFileViewerApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2008-01-14
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "MetadataFileViewerApp.h"
#include "MetadataFileViewerMain.h"
#include "BExcept.h"

IMPLEMENT_APP(MetadataFileViewerApp);

bool MetadataFileViewerApp::OnInit()
{
    MetadataFileViewerFrame* frame = new MetadataFileViewerFrame(0L, _("Metadata File Viewer"));
#ifdef __WXMSW__
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
#endif
    frame->Show();

    return true;
}

bool MetadataFileViewerApp::OnExceptionInMainLoop()
{
    // this shows how we may let some exception propagate uncaught
    try
    {
        throw;
    }
    catch ( ButcherException& e )
    {
        wxMessageBox(e.what(),
                     _("Metadata File Viewer"), wxOK | wxICON_ERROR);
    }
    catch ( ... )
    {
        wxMessageBox(_("Unhandled exception caught, program will terminate."),
                     _("Metadata File Viewer"), wxOK | wxICON_ERROR);
    }
    return true;
}

void MetadataFileViewerApp::OnUnhandledException()
{
    // this shows how we may let some exception propagate uncaught
    try
    {
        throw;
    }
    catch ( ButcherException& e )
    {
        wxMessageBox(e.what(),
                     _("Metadata File Viewer"), wxOK | wxICON_ERROR);
    }
    catch ( ... )
    {
        wxMessageBox(_("Unhandled exception caught, program will terminate."),
                     _("Metadata File Viewer"), wxOK | wxICON_ERROR);
    }

}
