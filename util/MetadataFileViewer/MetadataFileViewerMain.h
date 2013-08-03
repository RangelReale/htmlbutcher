/***************************************************************
 * Name:      MetadataFileViewerMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2008-01-14
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef METADATAFILEVIEWERMAIN_H
#define METADATAFILEVIEWERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/treectrl.h>

#include "MetadataFileViewerApp.h"
#include "BExcept.h"
#include "ButcherMetadataFile.h"

class ViewerButcherMetadataFile : public  ButcherMetadataFile
{
public:
    ViewerButcherMetadataFile(long headersize) : ButcherMetadataFile(), headersize_(headersize) {}

    static wxString GetMDDescription(ButcherMetadataID_t id);
    static wxString GetMIDescription(ButcherMetadataID_t id);
protected:
    virtual bool ReadHeader(wxFile &file) { file.Seek(headersize_, wxFromStart); return true; }
    virtual bool WriteHeader(wxFile &file) { throw ButcherException(_("Cannot save")); return false; }
private:
    long headersize_;
};

class MetadataFileViewerFrame: public wxFrame
{
public:
    MetadataFileViewerFrame(wxFrame *frame, const wxString& title);
    ~MetadataFileViewerFrame();
private:
    enum
    {
        idMenuOpen = 1000,
        idMenuQuit,
        idMenuAbout,

        idMDTree
    };

    void LoadMDFile(const wxString &filename, long headersize);

    void LoadMDItem(wxTreeCtrl *mdtree, wxTreeItemId *parent, ButcherMetadataStorage *storage);

    void OnOpen(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
};


#endif // METADATAFILEVIEWERMAIN_H
