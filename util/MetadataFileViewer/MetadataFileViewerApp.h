/***************************************************************
 * Name:      MetadataFileViewerApp.h
 * Purpose:   Defines Application Class
 * Author:     ()
 * Created:   2008-01-14
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef METADATAFILEVIEWERAPP_H
#define METADATAFILEVIEWERAPP_H

#include <wx/app.h>

class MetadataFileViewerApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual bool OnExceptionInMainLoop();
    virtual void OnUnhandledException();
};

#endif // METADATAFILEVIEWERAPP_H
