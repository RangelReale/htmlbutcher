//=============================================================================
/**
 *  @file   ButcherProjectAssortedFile.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAssortedFile.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"
#include "ButcherProjectAssortedFiles.h"

#include <wx/filename.h>
#include <wx/wfstream.h>



/////////////////////////////////
// CLASS
//      ButcherProjectAssortedFile
/////////////////////////////////
ButcherProjectAssortedFile::ButcherProjectAssortedFile(ButcherProject *project, const wxString &filename) :
    ButcherProjectBaseBLId(project), ButcherOutputFile(), filename_(filename), groups_(this), filedata_(NULL),
        filetype_(AFT_UNKNOWN), filepath_(project->FilePaths().DefaultId()), savefilename_(wxEmptyString)
{

}




ButcherProjectAssortedFile::ButcherProjectAssortedFile(ButcherProject *project) :
    ButcherProjectBaseBLId(project), ButcherOutputFile(), filename_(wxEmptyString), groups_(this), filedata_(NULL),
        filetype_(AFT_UNKNOWN), filepath_(project->FilePaths().DefaultId()), savefilename_(wxEmptyString)
{

}




ButcherProjectAssortedFile::~ButcherProjectAssortedFile()
{
    // don't delete filedata_
}




void ButcherProjectAssortedFile::SetFilename(const wxString &f)
{
    if (filename_!=f)
    {
        if (!wxFileName::FileExists(f))
            throw ButcherException(_("File does not exists"));

        filename_=f;

        if (filedata_) filedata_=NULL;

        ProjectModified();
    }
}




wxString ButcherProjectAssortedFile::GetOutputFileURL(BLID_t id)
{
    wxString p=(filepath_>0?GetProject()->FilePaths().Get(filepath_)->GetFormattedPath():wxT(""));
    wxFileName fn;
    if (!savefilename_.IsEmpty())
    {
        wxFileName tmp(savefilename_);
        if (tmp.GetExt().IsEmpty())
        {
            tmp.Assign(GetPathFile());
            fn.Assign(p, savefilename_, tmp.GetExt());
        }
        else
            fn.Assign(p, savefilename_);
    }
    else
        fn.Assign(p, GetPathFile());
	return fn.GetFullPath(wxPATH_UNIX);
}



wxString ButcherProjectAssortedFile::GetOutputFilename(const wxString &basepath, BLID_t id)
{
    wxFileName fn;
    //fn.Assign(basepath, GetOutputFileURL(id));
	fn.Assign(GetOutputFileURL(id));
	if (!basepath.IsEmpty())
		fn.PrependDir(basepath);
    return fn.GetFullPath();

/*
    wxString p=(filepath_>0?GetProject()->FilePaths().Get(filepath_)->GetFormattedPath(basepath):basepath);
    wxFileName fn;
    if (!savefilename_.IsEmpty())
    {
        wxFileName tmp(savefilename_);
        if (tmp.GetExt().IsEmpty())
        {
            tmp.Assign(GetPathFile());
            fn.Assign(p, savefilename_, tmp.GetExt());
        }
        else
            fn.Assign(p, savefilename_);
    }
    else
        fn.Assign(p, GetPathFile());
    return fn.GetFullPath();
*/
}




void ButcherProjectAssortedFile::SaveOutputFile(const wxString &filename, BLID_t id)
{
    wxFileOutputStream sfile(filename);
    SaveFile(sfile);
}





wxString ButcherProjectAssortedFile::GetPathFile()
{
    wxFileName fn(filename_);
    return fn.GetFullName();
}



wxString ButcherProjectAssortedFile::GetPathDir()
{
    wxFileName fn(filename_);
    return fn.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
}



void ButcherProjectAssortedFile::SaveFile(const wxString &filename)
{
    wxFileOutputStream f(filename);
    SaveFile(f);
}




void ButcherProjectAssortedFile::SaveFileToPath(const wxString &path)
{
    wxString p=(filepath_>0?GetProject()->FilePaths().Get(filepath_)->GetFormattedPath(path):path);
    ButcherProjectFilePath::ForcePathCreate(p);

    wxFileName fn;
    if (!savefilename_.IsEmpty())
    {
        wxFileName tmp(savefilename_);
        if (tmp.GetExt().IsEmpty())
        {
            tmp.Assign(GetPathFile());
            fn.Assign(p, savefilename_, tmp.GetExt());
        }
        else
            fn.Assign(p, savefilename_);
    }
    else
        fn.Assign(p, GetPathFile());
    SaveFile(fn.GetFullPath());
}




void ButcherProjectAssortedFile::SaveFile(wxOutputStream &ostream)
{
    ostream.SeekO(0, wxFromStart);
    if (filedata_)
        ostream.Write(*filedata_->GetData());
    else
    {
        wxFileInputStream f(filename_);
        ostream.Write(f);
    }
}




ButcherImage *ButcherProjectAssortedFile::CreateImage()
{
    if (!IsImage())
        throw ButcherException(_("Assorted file is not an image"));

    if (filedata_)
        return ButcherImageFactory::Load(*filedata_->GetData());
    else
    {
        wxFileInputStream f(filename_);
        return ButcherImageFactory::Load(f);
    }
}



void ButcherProjectAssortedFile::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_FILENAME, ButcherMetadataDataItem::DT_STRING)) ||
        (!metadata->Exists(BFILE_MDI_FILE, ButcherMetadataDataItem::DT_BINARY)))
        throw ButcherException(_("Invalid metadata for Assorted file"));

    SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    filename_=metadata->Get(BFILE_MDI_FILENAME).GetString();

    if (metadata->Exists(BFILE_MDI_FILETYPE, ButcherMetadataDataItem::DT_INTEGER))
        filetype_=static_cast<filetype_t>(metadata->Get(BFILE_MDI_FILETYPE).GetInteger());
    if (metadata->Exists(BFILE_MDI_FILEPATH, ButcherMetadataDataItem::DT_INTEGER))
        filepath_=metadata->Get(BFILE_MDI_FILEPATH).GetInteger();
    if (metadata->Exists(BFILE_MDI_SAVEFILENAME, ButcherMetadataDataItem::DT_STRING))
        savefilename_=metadata->Get(BFILE_MDI_SAVEFILENAME).GetString();

    if (metadata->Exists(BFILE_MDI_ASSORTEDFILEGROUP, ButcherMetadataDataItem::DT_METADATA))
        if (metadata->Get(BFILE_MDI_ASSORTEDFILEGROUP).GetMetadata().Exists(BFILE_MD_ASSORTEDFILEGROUP))
        {
            ButcherMetadataData *gmetadata;

            for (unsigned int i=0; i<metadata->Get(BFILE_MDI_ASSORTEDFILEGROUP).GetMetadata().Get(BFILE_MD_ASSORTEDFILEGROUP).Count(); i++)
            {
                gmetadata=&metadata->Get(BFILE_MDI_ASSORTEDFILEGROUP).GetMetadata().Get(BFILE_MD_ASSORTEDFILEGROUP).Get(i);

                if ((!gmetadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)))
                    throw ButcherException(_("Invalid metadata for Assorted file"));

                groups_.Add(gmetadata->Get(BFILE_MDI_ID).GetInteger());
            }
        }

    UpdateMetadata(metadata, true);
}




void ButcherProjectAssortedFile::SaveMetadata(ButcherMetadataData *metadata)
{
    wxFileName fn(filename_);
    wxInputStream *filebin;
    if (filedata_) filebin=filedata_->GetData(); else filebin=new wxFileInputStream(filename_);

    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_FILENAME, fn.GetFullName()))->
        MultiAdd(new ButcherMetadataDataItem_Binary(metadata, BFILE_MDI_FILE, filebin))->
        MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_ASSORTEDFILEGROUP))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FILETYPE, filetype_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FILEPATH, filepath_));

    if (!savefilename_.IsEmpty())
        metadata->
            MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_SAVEFILENAME, savefilename_));


    for (ButcherListIdList::const_iterator i=groups_.Get().begin(); i!=groups_.Get().end(); i++)
    {
        ButcherMetadataData *gmetadata=metadata->Get(BFILE_MDI_ASSORTEDFILEGROUP).GetMetadata().Get(BFILE_MD_ASSORTEDFILEGROUP, true).Add();
        gmetadata->
            MultiAdd(new ButcherMetadataDataItem_Integer(gmetadata, BFILE_MDI_ID, *i));
    }
}




void ButcherProjectAssortedFile::UpdateMetadata(ButcherMetadataData *metadata, bool reload)
{
    // on saving, BFILE_MDI_FILE changes, update our copy
    filedata_=dynamic_cast<ButcherMetadataDataItem_Binary*>(&metadata->Get(BFILE_MDI_FILE));
    filename_=metadata->Get(BFILE_MDI_FILENAME).GetString();
}


