//=============================================================================
/**
 *  @file   ButcherProjectImageFormat.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectImageFormat.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectImageFormat
/////////////////////////////////
ButcherProjectImageFormat::ButcherProjectImageFormat(ButcherProject *project,
    const wxString &name, ButcherImage::format_t format, int flags) :
    ButcherProjectBaseBLId(project), name_(name), format_(format), flags_(flags)
{

}



ButcherProjectImageFormat::ButcherProjectImageFormat(ButcherProject *project) :
    ButcherProjectBaseBLId(project), name_(wxT("TEMP")),
        format_(ButcherImage::FMT_JPG), flags_(0)
{

}




void ButcherProjectImageFormat::Set(const wxString &name,
    ButcherImage::format_t format, int flags)
{
    name_=name;
    format_=format;
    flags_=flags;
    ProjectModified();
}




void ButcherProjectImageFormat::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING)) ||
        (!metadata->Exists(BFILE_MDI_FORMAT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_FLAGS, ButcherMetadataDataItem::DT_INTEGER)))
        throw ButcherException(_("Invalid metadata for image format"));

    SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    name_=metadata->Get(BFILE_MDI_NAME).GetString();
    format_=static_cast<ButcherImage::format_t>(metadata->Get(BFILE_MDI_FORMAT).GetInteger());
    flags_=metadata->Get(BFILE_MDI_FLAGS).GetInteger();
}




void ButcherProjectImageFormat::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FORMAT, format_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FLAGS, flags_));
}


