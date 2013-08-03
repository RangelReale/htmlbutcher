/***************************************************************
 * Name:      MetadataFileViewerMain.cpp
 * Purpose:   Code for Application Frame
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

#include <wx/numdlg.h>

#include "MetadataFileViewerMain.h"
#include "ButcherFileDefs.h"

wxString ViewerButcherMetadataFile::GetMDDescription(ButcherMetadataID_t id)
{
    switch (id)
    {
    case BFILE_MD_COLORSCHEME: return _("COLOR SCHEME");
    case BFILE_MD_IMAGEFORMAT: return _("IMAGE FORMAT");
    case BFILE_MD_FILE: return _("FILE");
    case BFILE_MD_MASK: return _("MASK");
    case BFILE_MD_VIEW: return _("VIEW");
    case BFILE_MD_ASSORTEDFILE: return _("ASSORTED FILE");
    case BFILE_MD_ASSORTEDFILEGROUP: return _("ASSORTED FILE GROUP");
    case BFILE_MD_FILEPATH: return _("FILE PATH");
    case BFILE_MD_CSSFILE: return _("CSS FILE");

    case BFILE_MD_COLORSCHEME_ITEM: return _("COLOR SCHEME ITEM");
    case BFILE_MD_CONFIG: return _("CONFIG");
    case BFILE_MD_CONTAINER: return _("CONTAINER");
    case BFILE_MD_LINE: return _("LINE");
    case BFILE_MD_AREA: return _("AREA");
    case BFILE_MD_FILEALTERNATE: return _("FILE ALTERNATE");
    case BFILE_MD_ALTERNATEIMAGEINFO: return _("ALTERNATE IMAGE INFO");
    case BFILE_MD_TRANSPARENTCOLOR: return _("TRANSPARENT COLOR");
    default: return wxString::Format(wxT("%d"), id);
    }
}
wxString ViewerButcherMetadataFile::GetMIDescription(ButcherMetadataID_t id)
{
    switch (id)
    {
    case BFILE_MDI_ID: return _("ID");
    case BFILE_MDI_NAME: return _("NAME");
    case BFILE_MDI_COLOR: return _("COLOR");
    case BFILE_MDI_FORMAT: return _("FORMAT");
    case BFILE_MDI_FLAGS: return _("FLAGS");
    case BFILE_MDI_FILENAME: return _("FILENAME");
    case BFILE_MDI_WIDTH: return _("WIDTH");
    case BFILE_MDI_HEIGHT: return _("HEIGHT");
    case BFILE_MDI_FILE: return _("FILE");
    case BFILE_MDI_PARENT: return _("PARENT");
    case BFILE_MDI_MARGINLEFT: return _("MARGIN LEFT");
    case BFILE_MDI_MARGINTOP: return _("MARGIN TOP");
    case BFILE_MDI_MARGINRIGHT: return _("MARGIN RIGHT");
    case BFILE_MDI_MARGINBOTTOM: return _("MARGIN BOTTOM");
    case BFILE_MDI_LEFT: return _("LEFT");
    case BFILE_MDI_TOP: return _("TOP");
    case BFILE_MDI_RIGHT: return _("RIGHT");
    case BFILE_MDI_BOTTOM: return _("BOTTOM");
    case BFILE_MDI_LINES: return _("LINES");
    case BFILE_MDI_AREAS: return _("AREAS");
    case BFILE_MDI_START: return _("START");
    case BFILE_MDI_END: return _("END");
    case BFILE_MDI_ORIENTATION: return _("ORIENTATION");
    case BFILE_MDI_POSITION: return _("POSITION");
    case BFILE_MDI_MASK: return _("MASK");
    case BFILE_MDI_MASKCONFIG: return _("MASKCONFIG");
    case BFILE_MDI_IMAGEDIR: return _("IMAGEDIR");
    case BFILE_MDI_ITEMS: return _("ITEMS");
    case BFILE_MDI_BODYTAGADD: return _("BODYTAGADD");
    case BFILE_MDI_TITLE: return _("TITLE");
    case BFILE_MDI_HEAD: return _("HEAD");
    case BFILE_MDI_AREAKIND: return _("AREAKIND");
    case BFILE_MDI_IMAGEFORMAT: return _("IMAGEFORMAT");
    case BFILE_MDI_BACKGROUND: return _("BACKGROUND");
    case BFILE_MDI_FILENAMEPREPEND: return _("FILENAMEPREPEND");
    case BFILE_MDI_ALIGN: return _("ALIGN");
    case BFILE_MDI_VALIGN: return _("VALIGN");
    case BFILE_MDI_BGREPEAT: return _("BGREPEAT");
    case BFILE_MDI_ISBGCOLOR: return _("ISBGCOLOR");
    case BFILE_MDI_BGCOLOR: return _("BGCOLOR");
    case BFILE_MDI_HAVEMAP: return _("HAVEMAP");
    case BFILE_MDI_CONTENT: return _("CONTENT");
    case BFILE_MDI_CELLTAGAPPEND: return _("CELLTAGAPPEND");
    case BFILE_MDI_IMAGEURL: return _("IMAGEURL");
    case BFILE_MDI_IMAGEURLTAGAPPEND: return _("IMAGEURLTAGAPPEND");
    case BFILE_MDI_IMAGETAGAPPEND: return _("IMAGE TAG APPEND");
    case BFILE_MDI_IMAGEMAPNAME: return _("IMAGEMAPNAME");
    case BFILE_MDI_TABLETAGADD: return _("TABLE TAG ADD");
    case BFILE_MDI_BODYPREPEND: return _("BODY PREPEND");
    case BFILE_MDI_BODYAPPEND: return _("BODY APPEND");
    case BFILE_MDI_ENCODING: return _("ENCODING");
    case BFILE_MDI_FILEALTERNATE: return _("FILE ALTERNATE");
    case BFILE_MDI_INNERSCROLLABLE: return _("INNER SCROLLABLE");
    case BFILE_MDI_ASSORTEDFILEGROUP: return _("ASSORTED FILE GROUP");
    case BFILE_MDI_PATH: return _("PATH");
    case BFILE_MDI_FILEPATH: return _("FILE PATH");
    case BFILE_MDI_FILETYPE: return _("FILE TYPE");
    case BFILE_MDI_ALTERNATEIMAGEINFO: return _("ALTERNATE IMAGE INFO");
    case BFILE_MDI_TRANSPARENTCOLORS: return _("TRANSPARENT COLORS");
    case BFILE_MDI_ENABLED: return _("ENABLED");
    case BFILE_MDI_SAVEFILENAME: return _("SAVE FILE NAME");
    case BFILE_MDI_IMAGESOURCE: return _("IMAGE SOURCe");
    case BFILE_MDI_IMAGEFILELINK: return _("IMAGE FILE LINK");
    case BFILE_MDI_BGIMAGE: return _("BGIMAGE");
    case BFILE_MDI_TEXT: return _("TEXT");
    case BFILE_MDI_CSS: return _("CSS");

    case BFILE_MDI_COLORITEM: return _("COLORITEM");
    case BFILE_MDI_CONFIGS: return _("CONFIGS");
    case BFILE_MDI_CONTAINERS: return _("CONTAINERS");

    case BFILE_MDI_AREACLASS: return _("AREACLASS");
    default: return wxString::Format(wxT("%d"), id);
    }
}

BEGIN_EVENT_TABLE(MetadataFileViewerFrame, wxFrame)
    EVT_CLOSE(MetadataFileViewerFrame::OnClose)
    EVT_MENU(idMenuOpen, MetadataFileViewerFrame::OnOpen)
    EVT_MENU(idMenuQuit, MetadataFileViewerFrame::OnQuit)
    EVT_MENU(idMenuAbout, MetadataFileViewerFrame::OnAbout)
END_EVENT_TABLE()

MetadataFileViewerFrame::MetadataFileViewerFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuOpen, _("&Open\tCtrl-O"), _("Open file"));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);

    // Tree
    /*wxTreeCtrl *mdtree =*/ (void) new wxTreeCtrl(this, idMDTree, wxDefaultPosition, wxDefaultSize);
}


MetadataFileViewerFrame::~MetadataFileViewerFrame()
{
}

void MetadataFileViewerFrame::OnOpen(wxCommandEvent& event)
{
    long headersize;

    wxFileDialog d(this);
    if (d.ShowModal()==wxID_OK)
    {
        headersize=wxGetNumberFromUser(_("Custom Header size (bytes)"), _("Size"), _("Open file"), 5, 0, 100000, this);
        if (headersize >= 0)
        {
            LoadMDFile(d.GetPath(), headersize);
        }
    }
}

void MetadataFileViewerFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void MetadataFileViewerFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void MetadataFileViewerFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(_("Metadata File Viewer 1.0"), _("Metadata File Viewer"));
}

void MetadataFileViewerFrame::LoadMDFile(const wxString &filename, long headersize)
{
    wxTreeCtrl *mdtree=(wxTreeCtrl *)FindWindow(idMDTree);

    ViewerButcherMetadataFile fl(headersize);
    fl.Load(filename);

    mdtree->DeleteAllItems();

    wxTreeItemId root=mdtree->AddRoot(_("ROOT"));

    LoadMDItem(mdtree, &root, &fl);

    //mdtree->ExpandAll();
    mdtree->Expand(mdtree->GetRootItem());
}

void MetadataFileViewerFrame::LoadMDItem(wxTreeCtrl *mdtree, wxTreeItemId *parent, ButcherMetadataStorage *storage)
{
    wxTreeItemId newid, newid2, newid3;
    ButcherMetadataData::data_t::const_iterator dt;

    for (ViewerButcherMetadataFile::listorder_t::const_iterator i=storage->GetListOrder().begin();
        i!=storage->GetListOrder().end(); i++)
    {
        newid=mdtree->AppendItem(*parent, ViewerButcherMetadataFile::GetMDDescription(*i));

        for (unsigned long ct=0; ct<storage->Get(*i).Count(); ct++)
        {
            newid2=mdtree->AppendItem(newid, wxString::Format(wxT("%d"), ct));

            for (dt=storage->Get(*i).Get(ct).GetData().begin();
                    dt!=storage->Get(*i).Get(ct).GetData().end(); dt++)
            {
                newid3=mdtree->AppendItem(newid2,
                    wxString::Format(wxT("%s: %s     [st: %d - sz: %d]"),
                        ViewerButcherMetadataFile::GetMIDescription(dt->first).c_str(),
                        storage->Get(*i).Get(ct).Get(dt->first).GetValueDescription().c_str(),
                        storage->Get(*i).Get(ct).Get(dt->first).GetPosition(),
                        storage->Get(*i).Get(ct).Get(dt->first).GetSize()
                    ));

                if (storage->Get(*i).Get(ct).Get(dt->first).GetDataType()==ButcherMetadataDataItem::DT_METADATA)
                    LoadMDItem(mdtree, &newid3, &storage->Get(*i).Get(ct).Get(dt->first).GetMetadata());
            }
        }
    }
}

