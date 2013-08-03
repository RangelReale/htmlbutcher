//=============================================================================
/**
 *  @file   ButcherProjectAreaImageInfo.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAreaImageInfo.h"
#include "ButcherProjectAreaConfigBase.h"
#include "ButcherProjectImageFormat.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAreaImageInfo
/////////////////////////////////
ButcherProjectAreaImageInfo::ButcherProjectAreaImageInfo(ButcherProjectAreaConfigBase *areaconfig,
    int infoid) :
    ButcherProjectBaseModify(areaconfig->GetProject()), ButcherOutputFile(),
    areaconfig_(areaconfig), infoid_(infoid), enabled_(false),
    imageformat_(areaconfig->GetProject()->ImageFormats().DefaultId()),
    filename_(wxEmptyString), filename_prepend_(true), transparentcolors_(),
    transparentpixels_()
{

}




ButcherProjectImageFormat *ButcherProjectAreaImageInfo::GetImageFormat()
{
    if (GetProject()->ImageFormats().Exists(GetImageFormatId()))
        return GetProject()->ImageFormats().Get(GetImageFormatId());
    return GetProject()->ImageFormats().Get(GetProject()->ImageFormats().DefaultId());
}




void ButcherProjectAreaImageInfo::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    if (areaconfig_)
        areaconfig_->ProjectModified();
}




bool ButcherProjectAreaImageInfo::IsOutputImage()
{
    return
        GetAreaConfig()->GetArea()->GetAreaClass()!=ButcherProjectArea::AC_MAP &&
        (GetAreaConfig()->GetAreaKind()==ButcherProjectAreaConfigBase::AK_IMAGE ||
        GetAreaConfig()->GetBackground()==true);
}

void ButcherProjectAreaImageInfo::Copy(const ButcherProjectAreaImageInfo &other)
{
	infoid_=other.infoid_;
	enabled_=other.enabled_;
	imageformat_=other.imageformat_;
	filename_=other.filename_;
	filename_prepend_=other.filename_prepend_;
	transparentcolors_=other.transparentcolors_;
	transparentpixels_=other.transparentpixels_;

	ProjectModified();
}




// id is viewid
wxString ButcherProjectAreaImageInfo::GetOutputFileURL(BLID_t id)
{
    return
        (id>0?GetProject()->Views()[id]->GetImageDir(wxEmptyString):wxT(""))+
        areaconfig_->GetArea()->GetImageFilename(id, true, infoid_);
}




// id is viewid
wxString ButcherProjectAreaImageInfo::GetOutputFilename(const wxString &basepath, BLID_t id)
{


	return
        (id>0?GetProject()->Views()[id]->GetImageDir(basepath):wxT(""))+
        areaconfig_->GetArea()->GetImageFilename(id, true, infoid_);
}




void ButcherProjectAreaImageInfo::SaveOutputFile(const wxString &filename, BLID_t id)
{
    // TODO
}



void ButcherProjectAreaImageInfo::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseModify::ProjectEvent(event);
    switch (event.GetProjectEvent().GetEvent())
    {
    case ButcherProjectEvent::BPE_IMAGEFORMATDELETED:
        if (event.GetProjectEvent().GetEId()==0||event.GetProjectEvent().GetEId()==imageformat_)
        {
            event.AddChange();
            if (event.GetIsExecute())
            {
                imageformat_=GetProject()->ImageFormats().DefaultId();
                ProjectModified();
            }

        }
        break;
    default:
        break;
    }
}





void ButcherProjectAreaImageInfo::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_IMAGEFORMAT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_ENABLED, ButcherMetadataDataItem::DT_INTEGER)))
        throw ButcherException(_("Invalid metadata for image info"));

    enabled_=metadata->Get(BFILE_MDI_ENABLED).GetBool();
    imageformat_=metadata->Get(BFILE_MDI_IMAGEFORMAT).GetInteger();

    if (metadata->Exists(BFILE_MDI_FILENAME, ButcherMetadataDataItem::DT_STRING))
        filename_=metadata->Get(BFILE_MDI_FILENAME).GetString();
    if (metadata->Exists(BFILE_MDI_FILENAMEPREPEND, ButcherMetadataDataItem::DT_INTEGER))
        filename_prepend_=metadata->Get(BFILE_MDI_FILENAMEPREPEND).GetBool();
    // transparent colors
    if (metadata->Exists(BFILE_MDI_TRANSPARENTCOLORS, ButcherMetadataDataItem::DT_METADATA) &&
        metadata->Get(BFILE_MDI_TRANSPARENTCOLORS).GetMetadata().Exists(BFILE_MD_TRANSPARENTCOLOR))
    {
        wxColor tmp;
        for (unsigned int i=0; i<metadata->Get(BFILE_MDI_TRANSPARENTCOLORS).GetMetadata().Get(BFILE_MD_TRANSPARENTCOLOR).Count(); i++)
        {
            tmp.Set(metadata->Get(BFILE_MDI_TRANSPARENTCOLORS).GetMetadata().Get(BFILE_MD_TRANSPARENTCOLOR).Get(i).Get(BFILE_MDI_COLOR).GetInteger());
            transparentcolors_.push_back(tmp);
        }
    }
    // transparent pixels
    if (metadata->Exists(BFILE_MDI_TRANSPARENTPIXELS, ButcherMetadataDataItem::DT_METADATA) &&
        metadata->Get(BFILE_MDI_TRANSPARENTPIXELS).GetMetadata().Exists(BFILE_MD_TRANSPARENTPIXEL))
    {
        wxPoint tmp;
        for (unsigned int i=0; i<metadata->Get(BFILE_MDI_TRANSPARENTPIXELS).GetMetadata().Get(BFILE_MD_TRANSPARENTPIXEL).Count(); i++)
        {
            if (!metadata->Get(BFILE_MDI_TRANSPARENTPIXELS).GetMetadata().Get(BFILE_MD_TRANSPARENTPIXEL).Get(i).Exists(BFILE_MDI_LEFT, ButcherMetadataDataItem::DT_INTEGER) ||
                !metadata->Get(BFILE_MDI_TRANSPARENTPIXELS).GetMetadata().Get(BFILE_MD_TRANSPARENTPIXEL).Get(i).Exists(BFILE_MDI_TOP, ButcherMetadataDataItem::DT_INTEGER))
                throw ButcherException(_("Invalid metadata for image info transparent pixel"));

            tmp.x=metadata->Get(BFILE_MDI_TRANSPARENTPIXELS).GetMetadata().Get(BFILE_MD_TRANSPARENTPIXEL).Get(i).Get(BFILE_MDI_LEFT).GetInteger();
            tmp.y=metadata->Get(BFILE_MDI_TRANSPARENTPIXELS).GetMetadata().Get(BFILE_MD_TRANSPARENTPIXEL).Get(i).Get(BFILE_MDI_TOP).GetInteger();
            transparentpixels_.push_back(tmp);
        }
    }
}



void ButcherProjectAreaImageInfo::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ENABLED, enabled_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_IMAGEFORMAT, imageformat_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FILENAMEPREPEND, filename_prepend_));

    if (!filename_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_FILENAME, filename_));

    // transparent colors
    if (transparentcolors_.size()>0)
    {
        metadata->MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_TRANSPARENTCOLORS));
        ButcherMetadataData *tmetadata;
        for (ButcherImage::transparentcolors_t::const_iterator ti=transparentcolors_.begin();
            ti!=transparentcolors_.end(); ti++)
        {
            tmetadata=metadata->Get(BFILE_MDI_TRANSPARENTCOLORS).GetMetadata().Get(BFILE_MD_TRANSPARENTCOLOR, true).Add();
            tmetadata->
                MultiAdd(new ButcherMetadataDataItem_Integer(tmetadata, BFILE_MDI_COLOR,
                    BFILE_RGB(ti->Red(), ti->Green(), ti->Blue())));
        }
    }
    // transparent pixels
    if (transparentpixels_.size()>0)
    {
        metadata->MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_TRANSPARENTPIXELS));
        ButcherMetadataData *tmetadata;
        for (ButcherImage::transparentpixels_t::const_iterator ti=transparentpixels_.begin();
            ti!=transparentpixels_.end(); ti++)
        {
            tmetadata=metadata->Get(BFILE_MDI_TRANSPARENTPIXELS).GetMetadata().Get(BFILE_MD_TRANSPARENTPIXEL, true).Add();
            tmetadata->
                MultiAdd(new ButcherMetadataDataItem_Integer(tmetadata, BFILE_MDI_LEFT, ti->x))->
                MultiAdd(new ButcherMetadataDataItem_Integer(tmetadata, BFILE_MDI_TOP, ti->y));
        }
    }
}


