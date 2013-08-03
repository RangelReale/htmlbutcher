//=============================================================================
/**
 *  @file   ButcherProjectFile.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectFile.h"
#include "ButcherProjectBaseAutoProgress.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"

#include <wx/filename.h>
#include <wx/wfstream.h>



/////////////////////////////////
// CLASS
//      ButcherProjectFile
/////////////////////////////////
ButcherProjectFile::ButcherProjectFile(ButcherProject *project,
    const wxString &name, const wxString &filename,
    unsigned long imagewidth, unsigned long imageheight) :
    ButcherProjectBaseBLId(project), ButcherOutputFile(),
    name_(name), filename_(filename), image_(NULL),
    imagewidth_(imagewidth), imageheight_(imageheight),
	imageloaddata_(NULL), imagedata_(NULL),
    isloaderror_(false)
{
	if (filename_.IsEmpty())
		filename_=wxT("___TEMP___");
}

ButcherProjectFile::ButcherProjectFile(ButcherProject *project,
    const wxString &name, wxInputStream &filedata,
    unsigned long imagewidth, unsigned long imageheight) :
    ButcherProjectBaseBLId(project), ButcherOutputFile(),
    name_(name), filename_(wxT("___TEMP___")), image_(NULL),
    imagewidth_(imagewidth), imageheight_(imageheight),
	imageloaddata_(NULL), imagedata_(NULL),
    isloaderror_(false)
{
	filedata.SeekI(0);
	wxMemoryOutputStream tmpdata;
	tmpdata.Write(filedata);
	tmpdata.SeekO(0);

	imageloaddata_ = new wxMemoryInputStream(tmpdata);
	imageloaddata_->SeekI(0, wxFromStart);
}



ButcherProjectFile::ButcherProjectFile(ButcherProject *project) :
    ButcherProjectBaseBLId(project), name_(wxEmptyString), filename_(wxT("___TEMP___")),
    image_(NULL), imagewidth_(0), imageheight_(0),
	imageloaddata_(NULL), imagedata_(NULL), isloaderror_(false)
{

}




ButcherProjectFile::~ButcherProjectFile()
{
    if (image_)
        delete image_;
	if (imageloaddata_)
		delete imageloaddata_;
/*
    if (imagedata_)
        delete imagedata_;
*/
}




wxString ButcherProjectFile::GetDisplayName()
{
    if (!name_.IsEmpty())
        return name_;
    return GetPathFile();
}




void ButcherProjectFile::SetFilename(const wxString &n)
{
    if (filename_!=n)
    {
        if (!wxFileName::FileExists(n))
            throw ButcherException(_("File does not exists"));

        filename_=n;

        if (imagedata_) imagedata_=NULL;
		if (imageloaddata_) { delete imageloaddata_; imageloaddata_=NULL; }
        if (image_) { delete image_; image_=NULL; }

        ProjectModified();
    }
}

void ButcherProjectFile::SetFileData(wxInputStream &imageloaddata)
{
    if (imagedata_) imagedata_=NULL;
	if (imageloaddata_) { delete imageloaddata_; imageloaddata_=NULL; }
    if (image_) { delete image_; image_=NULL; }

	wxMemoryOutputStream tmpdata;
	tmpdata.Write(imageloaddata);

	imageloaddata_ = new wxMemoryInputStream(tmpdata);
	imageloaddata_->SeekI(0, wxFromStart);

	ProjectModified();
}



wxString ButcherProjectFile::GetOutputFileURL(BLID_t id)
{
    return GetPathFile();
}




wxString ButcherProjectFile::GetOutputFilename(const wxString &basepath, BLID_t id)
{
    wxFileName fn(basepath, GetOutputFileURL(id));
    return fn.GetFullPath();
}




void ButcherProjectFile::SaveOutputFile(const wxString &filename, BLID_t id)
{
    wxFileOutputStream sfile(filename);

    if (imagedata_)
    {
        auto_ptr<wxInputStream> istream(imagedata_->GetData());

        if (!istream.get())
            throw ButcherException(_("Error loading file stream"));

        istream->SeekI(0, wxFromStart);
        sfile.Write(*istream);
    }
	else if (imageloaddata_)
	{
		sfile.Write(*imageloaddata_);
	}
    else
    {
        wxFileInputStream ifile(filename_);
        sfile.Write(ifile);
    }
}




wxString ButcherProjectFile::GetPathFile()
{
    wxFileName fn(filename_);
    return fn.GetFullName();
}




wxString ButcherProjectFile::GetPathDir()
{
    wxFileName fn(filename_);
    return fn.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
}




ButcherImage *ButcherProjectFile::GetImage()
{
	if (!image_) {

        image_=CreateImage();
    }
    return image_;
}



ButcherImage *ButcherProjectFile::GetSubImage(const wxRect &r)
{
    ButcherImage *ret=GetImage()->SubImage(r);
#ifdef HTMLBUTCHER_DEMO
    if (ret)
    {
        int i;
        // watermark image
        wxBitmap wm(ret->GetWidth(), ret->GetHeight(), -1);
        wxMemoryDC wmdc;

        wmdc.SelectObject(wm);
        // transparent BG
        wmdc.SetPen(*wxTRANSPARENT_PEN);
        wmdc.SetBrush(*wxRED_BRUSH);
        wmdc.DrawRectangle(ret->GetImageRect());
        // mark
        wmdc.SetPen(*wxThePenList->FindOrCreatePen(wxT("LIGHT STEEL BLUE"), 1, wxDOT_DASH));
        //wmdc.SetBrush(*wxTheBrushList->FindOrCreateBrush(wxT("LIGHT STEEL BLUE"), wxBDIAGONAL_HATCH  ));
        wmdc.SetBrush(*wxTRANSPARENT_BRUSH);
        for (i=0; i<ret->GetHeight(); i+=15)
            wmdc.DrawLine(0, i, ret->GetWidth()-1, i);

        wmdc.SelectObject(wxNullBitmap);

        ret->Composite(wm);
    }
#endif //HTMLBUTCHER_DEMO

    return ret;
}




ButcherImage *ButcherProjectFile::CreateImage()
{
    if (isloaderror_) return NULL;

    ButcherProjectBaseAutoProgress progress(this,
        wxString::Format(_("Loading image %s, please wait..."), GetDisplayName().c_str()));

    ButcherImage *ret=NULL;
    try
    {
        if (imagedata_)
        {
            auto_ptr<wxInputStream> istream(imagedata_->GetData());

            if (!istream.get())
            {
                //throw ButcherException(wxT("Error loading image stream"));
                isloaderror_=true;
                return NULL;
            }

            istream->SeekI(0, wxFromStart);
            ret=ButcherImageFactory::Load(*istream);
        }
		else if (imageloaddata_)
		{
			ret=ButcherImageFactory::Load(*imageloaddata_);
		}
        else
            ret=ButcherImageFactory::Load(filename_);
    } catch(ButcherException) {
        ret=NULL;
    }

    if (!ret)
    {
        //throw ButcherException(_("Error loading image"));
        isloaderror_=true;
        return NULL;
    }

    return ret;
}



void ButcherProjectFile::Set(const wxString &name, const wxString &filename,
    unsigned long imagewidth, unsigned long imageheight)
{
    name_=name;
    filename_=filename;
    imagewidth_=imagewidth;
    imageheight_=imageheight;
    if (image_) {
        delete image_;
        image_=NULL;
    }
    if (imagedata_)
    {
        //delete imagedata_;
        imagedata_=NULL;
    }
    if (imageloaddata_)
    {
        delete imageloaddata_;
        imageloaddata_=NULL;
    }
    isloaderror_=false;
    ProjectModified();
}




void ButcherProjectFile::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_FILENAME, ButcherMetadataDataItem::DT_STRING)) ||
        (!metadata->Exists(BFILE_MDI_WIDTH, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_HEIGHT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_FILE, ButcherMetadataDataItem::DT_BINARY)))
        throw ButcherException(_("Invalid metadata for file"));

    SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    if (metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING))
        name_=metadata->Get(BFILE_MDI_NAME).GetString();
    filename_=metadata->Get(BFILE_MDI_FILENAME).GetString();
    imagewidth_=metadata->Get(BFILE_MDI_WIDTH).GetInteger();
    imageheight_=metadata->Get(BFILE_MDI_HEIGHT).GetInteger();

    UpdateMetadata(metadata, true);
}



void ButcherProjectFile::SaveMetadata(ButcherMetadataData *metadata)
{
    wxFileName fn(filename_);
    wxInputStream *filebin;
    if (imagedata_) filebin=imagedata_->GetData();
	else if (imageloaddata_) filebin=new wxMemoryInputStream(imageloaddata_);
	else filebin=new wxFileInputStream(filename_);

    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_FILENAME, fn.GetFullName()))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_WIDTH, imagewidth_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_HEIGHT, imageheight_))->
        MultiAdd(new ButcherMetadataDataItem_Binary(metadata, BFILE_MDI_FILE, filebin));
    if (!name_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_));
}




void ButcherProjectFile::UpdateMetadata(ButcherMetadataData *metadata, bool reload)
{
    // on saving, BFILE_MDI_FILE changes, update our copy
    imagedata_=dynamic_cast<ButcherMetadataDataItem_Binary*>(&metadata->Get(BFILE_MDI_FILE));
    filename_=metadata->Get(BFILE_MDI_FILENAME).GetString();
    if (imageloaddata_)
    {
        delete imageloaddata_;
        imageloaddata_=NULL;
    }
    if (reload && image_)
    {
        delete image_;
        image_=NULL;
    }
    isloaderror_=false;
}


