//=============================================================================
/**
 *  @file   ButcherProjectFilePath.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectFilePath.h"
#include "ButcherProject.h"
#include "ButcherProjectBaseAutoUpdate.h"

#include <cppcomp/wxccufile.h>
#include <wx/filename.h>
#include <wx/wfstream.h>



/////////////////////////////////
// CLASS
//      ButcherProjectFilePath
/////////////////////////////////
ButcherProjectFilePath::ButcherProjectFilePath(ButcherProject *project, const wxString &name,
    const wxString &path) :
    ButcherProjectBaseBLId(project), name_(name), path_(path)
{

}




ButcherProjectFilePath::ButcherProjectFilePath(ButcherProject *project) :
    ButcherProjectBaseBLId(project), name_(wxT("__TEMP__")), path_(wxEmptyString)
{

}





ButcherProjectFilePath::~ButcherProjectFilePath()
{

}



wxString ButcherProjectFilePath::GetFormattedPath(const wxString &path,
    const wxString &basepath, bool includetrailing)
{
    ccu_Path_wxString bpath(path);
    ccu_Path_wxString::pathformat_t f=ccu_Path_wxString::PPF_URL;
    if (!basepath.IsEmpty())
        f=ccu_Path_wxString::PPF_NATIVE;
    return bpath.GetPath(basepath, includetrailing, f);

/*
    wxString tpath=basepath;
    tpath.Replace(wxT("\\"), wxT("/"));
    if (!tpath.IsEmpty() && !tpath.EndsWith(wxT("/")))
        tpath+=wxT("/");
    tpath+=path;
    tpath.Replace(wxT("\\"), wxT("/"));
    if (includetrailing && !tpath.IsEmpty() && !tpath.EndsWith(wxT("/")))
        tpath+=wxT("/");
    return tpath;
*/
}




wxString ButcherProjectFilePath::GetFormattedPath(const wxString &basepath, bool includetrailing)
{
    return GetFormattedPath(path_, basepath, includetrailing);
}




void ButcherProjectFilePath::ForcePathCreate(const wxString &path)
{
    ccu_Path_wxString bpath(path);
    if (!bpath.IsFull())
        throw ButcherException(_("Only full paths can be created"));

    wxString curpath;
    for (unsigned i=0; i<bpath.GetPathCount(); i++)
    {
        curpath=bpath.GetPath(wxEmptyString, true, ccu_Path_wxString::PPF_NATIVE, i);
        if (!wxFileName::DirExists(curpath))
            if (!wxFileName::Mkdir(curpath))
                throw ButcherException(wxString::Format(_("Could not create dir: %s"), curpath.c_str()));

    }

/*
    wxString opath(path);
    opath.Replace(wxT("\\"), wxT("/"));
    wxString fpath=wxEmptyString;
    wxStringTokenizer pathtk(opath, wxT("/"));
    while (pathtk.HasMoreTokens())
    {
        fpath+=pathtk.GetNextToken();

        if (!wxFileName::DirExists(fpath))
            if (!wxFileName::Mkdir(fpath))
                throw ButcherException(wxString::Format(wxT("Could not create dir: %s"), fpath.c_str()));
        fpath+=wxT("/");
    }
*/
}




wxString ButcherProjectFilePath::GetValidFilename(const wxString &path)
{
    wxString ret(path), forb(wxFileName::GetForbiddenChars());
	forb+=wxT("[]()<>#@'\"");
    wxString rchar;
    for (unsigned i=0; i<forb.Len(); i++)
    {
        rchar=forb.GetChar(i);
        ret.Replace(rchar, wxT("_"));
    }
    ret.MakeLower().Replace(wxT(" "), wxT("_"));
    return ret;
}



void ButcherProjectFilePath::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING)) ||
        (!metadata->Exists(BFILE_MDI_PATH, ButcherMetadataDataItem::DT_STRING)))
        throw ButcherException(_("Invalid metadata for file path"));

    SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    name_=metadata->Get(BFILE_MDI_NAME).GetString();
    path_=metadata->Get(BFILE_MDI_PATH).GetString();
}




void ButcherProjectFilePath::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_PATH, path_));
}



