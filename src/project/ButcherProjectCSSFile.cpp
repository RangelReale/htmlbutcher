//=============================================================================
/**
 *  @file   ButcherProjectCSSFile.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectCSSFile.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"

#include <wx/filename.h>
#include <wx/wfstream.h>



/////////////////////////////////
// CLASS
//      ButcherProjectCSSFile
/////////////////////////////////
ButcherProjectCSSFile::ButcherProjectCSSFile(ButcherProject *project, const wxString &name) :
    ButcherProjectBaseBLId(project), ButcherOutputFile(), name_(name), csstext_(wxEmptyString),
    filepathid_(project->FilePaths().DefaultId()), savefilename_(wxEmptyString)
{

}




ButcherProjectCSSFile::ButcherProjectCSSFile(ButcherProject *project) :
    ButcherProjectBaseBLId(project), ButcherOutputFile(), name_(wxEmptyString), csstext_(wxEmptyString),
    filepathid_(project->FilePaths().DefaultId()), savefilename_(wxEmptyString)
{

}




ButcherProjectCSSFile::~ButcherProjectCSSFile()
{
    // don't delete filedata_
}



wxString ButcherProjectCSSFile::GetOutputFileURL(BLID_t id)
{
    wxString p=(filepathid_>0?GetProject()->FilePaths().Get(filepathid_)->GetFormattedPath():wxT(""));
    wxString fn;
    if (!savefilename_.IsEmpty())
    {
        wxFileName tmp(savefilename_);
        if (tmp.GetExt().IsEmpty())
        {
			tmp.SetExt(wxT("css"));
        }
		fn=tmp.GetFullName();
    }
    else
        fn=wxString::Format(wxT("%s.css"), name_.c_str());
    return p+fn;
}




wxString ButcherProjectCSSFile::GetOutputFilename(const wxString &basepath, BLID_t id)
{
    wxString p=(filepathid_>0?GetProject()->FilePaths().Get(filepathid_)->GetFormattedPath(basepath):wxT(""));
    wxString fn;
    if (!savefilename_.IsEmpty())
    {
        wxFileName tmp(savefilename_);
        if (tmp.GetExt().IsEmpty())
        {
			tmp.SetExt(wxT("css"));
        }
		fn=tmp.GetFullName();
    }
    else
        fn=wxString::Format(wxT("%s.css"), name_.c_str());
    return p+fn;

/*
	wxFileName fn;
    fn.Assign(basepath, GetOutputFileURL(id));
    return fn.GetFullPath();
*/
}




void ButcherProjectCSSFile::SaveOutputFile(const wxString &filename, BLID_t id)
{
    wxFileOutputStream sfile(filename);
    SaveFile(sfile);
}




void ButcherProjectCSSFile::SaveFile(const wxString &filename)
{
    wxFileOutputStream f(filename);
    SaveFile(f);
}



void ButcherProjectCSSFile::SaveFileToPath(const wxString &path)
{
    SaveFile(GetOutputFilename(path));
}




void ButcherProjectCSSFile::SaveFile(wxOutputStream &ostream)
{
    ostream.SeekO(0, wxFromStart);
    wxStringInputStream s(csstext_);
    ostream.Write(s);
}




void ButcherProjectCSSFile::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseBLId::ProjectEvent(event);

    switch (event.GetProjectEvent().GetEvent())
    {
    case ButcherProjectEvent::BPE_FILEPATHDELETED:
        // File path
        if (event.GetProjectEvent().GetEId()==0||event.GetProjectEvent().GetEId()==filepathid_)
        {
            event.AddChange();
            if (event.GetIsExecute())
                filepathid_=0; // no default for filepath
        }
        break;
    default:
        break;
    }
}



void ButcherProjectCSSFile::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING)) ||
        (!metadata->Exists(BFILE_MDI_TEXT, ButcherMetadataDataItem::DT_STRING)))
        throw ButcherException(_("Invalid metadata for CSS file"));

    SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    name_=metadata->Get(BFILE_MDI_NAME).GetString();
    csstext_=metadata->Get(BFILE_MDI_TEXT).GetString();

    if (metadata->Exists(BFILE_MDI_FILEPATH, ButcherMetadataDataItem::DT_INTEGER))
        filepathid_=metadata->Get(BFILE_MDI_FILEPATH).GetInteger();
    if (metadata->Exists(BFILE_MDI_SAVEFILENAME, ButcherMetadataDataItem::DT_STRING))
        savefilename_=metadata->Get(BFILE_MDI_SAVEFILENAME).GetString();
}




void ButcherProjectCSSFile::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_TEXT, csstext_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FILEPATH, filepathid_));

    if (!savefilename_.IsEmpty())
        metadata->
            MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_SAVEFILENAME, savefilename_));
}


