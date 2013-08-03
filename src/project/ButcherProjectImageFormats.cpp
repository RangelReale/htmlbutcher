//=============================================================================
/**
 *  @file   ButcherProjectImageFormats.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectImageFormats.h"
#include "ButcherProjectBaseAutoUpdate.h"



/////////////////////////////////
// CLASS
//      ButcherProjectImageFormats
/////////////////////////////////
ButcherProjectImageFormats::ButcherProjectImageFormats(ButcherProject *project) :
    ButcherList<ButcherProjectImageFormat>(), ButcherProjectBaseModify(project)
{

}




BLID_t ButcherProjectImageFormats::Add(const wxString &name, ButcherImage::format_t format, int flags, BLID_t id)
{
    return op_add(new ButcherProjectImageFormat(GetProject(), name, format, flags), id);
}




void ButcherProjectImageFormats::Edit(BLID_t id, const wxString &name, ButcherImage::format_t format, int flags)
{
    Get(id)->Set(name, format, flags);
    op_edit(id);
}




void ButcherProjectImageFormats::LoadDefault()
{
    Clear();
    Add(_("JPEG HIGH"), ButcherImage::FMT_JPG, ButcherImage::JPG_QUAL_75);
    Add(_("JPEG MAXIMUM"), ButcherImage::FMT_JPG, ButcherImage::JPG_QUAL_100);
    Add(_("JPEG LOW"), ButcherImage::FMT_JPG, ButcherImage::JPG_QUAL_25);
    Add(_("GIF"), ButcherImage::FMT_GIF, 0);
    Add(_("PNG"), ButcherImage::FMT_PNG, 0);
    Add(_("PNG 8 BIT"), ButcherImage::FMT_PNG, ButcherImage::GEN_FORCE8BIT);
    Add(_("PNG 32 BIT"), ButcherImage::FMT_PNG, ButcherImage::GEN_FORCE32BIT);
}




void ButcherProjectImageFormats::LoadMetadata(ButcherMetadataDataList *metadatalist)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if (metadatalist->Count()>0)
    {
        Clear();

        for (unsigned long i=0; i<metadatalist->Count(); i++)
        {
            Add(&metadatalist->Get(i));
        }
    }
}




void ButcherProjectImageFormats::SaveMetadata(ButcherMetadataDataList *metadatalist)
{
	for (iterator i=begin(); i!=end(); i++)
        i->SaveMetadata(metadatalist->Add());
}



BLID_t ButcherProjectImageFormats::Add(ButcherMetadataData *metadata)
{
    ButcherProjectImageFormat *mdadd=new ButcherProjectImageFormat(GetProject());
    mdadd->LoadMetadata(metadata);
    return op_add(mdadd, mdadd->GetBLId());
}


