//=============================================================================
/**
 *  @file   ButcherProjectAreaConfigBase.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectAreaConfigBase.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProjectArea.h"
#include "BConsts.h"
#include "ButcherFileDefs.h"



/////////////////////////////////
// CLASS
//      ButcherProjectAreaConfigBase
/////////////////////////////////
ButcherProjectAreaConfigBase::ButcherProjectAreaConfigBase(ButcherProjectArea *area,
    BLID_t id) :
    ButcherProjectBaseModify(area->GetProject()), area_(area), blid_(id),
	areakind_(AK_IMAGE), layouttype_(ButcherProjectConsts::LTYPE_DEFAULT), background_(false),
    content_(wxEmptyString), celltagappend_(wxEmptyString), imageurl_(wxEmptyString),
    imageurltagappend_(wxEmptyString), imagetagappend_(wxEmptyString), imagemapname_(wxEmptyString),
    align_(AA_NONE), valign_(AV_TOP), bgrepeat_(BR_ALL),
    isbgcolor_(false), bgcolor_(wxT("BLACK")), innerscrollable_(false), variablesize_(false), havemap_(false),
    alternatefile_(-1), imagesource_(IS_AREA), imagelink_(this),
    imageinfo_(this, -1), altimageinfo_(), margins_()
{
    ButcherProjectBaseAutoDisable dis(this);

    imageinfo_.SetEnabled(true);
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        altimageinfo_.push_back(linked_ptr<ButcherProjectAreaImageInfo>(new ButcherProjectAreaImageInfo(this, i)));
}



ButcherProjectAreaConfigBase::~ButcherProjectAreaConfigBase()
{

}




bool ButcherProjectAreaConfigBase::GetIsDefault()
{
	return blid_==ButcherProjectAreaConfigs::DEFAULT_CONFIG;
}



wxString ButcherProjectAreaConfigBase::GetAreaKindID()
{
    return GetAreaKindID(GetAreaKind());
}




wxString ButcherProjectAreaConfigBase::GetAreaKindID(areakind_t areakind)
{
    switch (areakind)
    {
    case AK_NONE:
        return _("NONE");
        break;
    case AK_IMAGE:
        return _("IMAGE");
        break;
    case AK_MASK:
        return _("MASK");
        break;
    default:
        return _("UNKNOWN");
        break;
    }
}



ButcherProjectAreaImageInfo &ButcherProjectAreaConfigBase::GetValidImageInfo(int alternatefile)
{
    if (alternatefile>=0 && AlternateImageInfo(alternatefile).GetEnabled())
        return AlternateImageInfo(alternatefile);
    return imageinfo_;
}





ButcherProjectAreaImageInfo &ButcherProjectAreaConfigBase::CurrentImageInfo()
{
    return GetValidImageInfo(alternatefile_);
}




wxRect ButcherProjectAreaConfigBase::GetGlobalRect()
{
	wxRect ret=area_->GetGlobalRect();
	ret.SetLeft(ret.GetLeft()+margins_.GetLeft());
	ret.SetTop(ret.GetTop()+margins_.GetTop());
	ret.SetWidth(ret.GetWidth()-margins_.GetLeft()-margins_.GetRight());
	ret.SetHeight(ret.GetHeight()-margins_.GetTop()-margins_.GetBottom());
	return ret;
}

bool ButcherProjectAreaConfigBase::HaveAlternate()
{
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
    {
        if (altimageinfo_[i]->GetEnabled())
            return true;
    }
    return false;
}




bool ButcherProjectAreaConfigBase::Compare(const ButcherProjectAreaConfigBase &other)
{
    return
        area_== other.area_ &&
        areakind_ == other.areakind_ &&
        imageinfo_.imageformat_ == other.imageinfo_.imageformat_ &&
        background_ == other.background_ &&
        alternatefile_ == other.alternatefile_ &&
        imagesource_ == other.imagesource_ &&
		margins_ == other.margins_ &&
        (imagesource_!=IS_LINK || imagelink_.Compare(other.imagelink_));
}

void ButcherProjectAreaConfigBase::Copy(const ButcherProjectAreaConfigBase &other)
{
	if (other.area_!=area_ || other.blid_==blid_)
		throw ButcherException(_("Incompatible source"));

	areakind_=other.areakind_;
	layouttype_=other.layouttype_;
	background_=other.background_;
	content_=other.content_;
	celltagappend_=other.celltagappend_;
	imageurl_=other.imageurl_;
	imageurltagappend_=other.imageurltagappend_;
	imagetagappend_=other.imagetagappend_;
	imagemapname_=other.imagemapname_;
	align_=other.align_;
	valign_=other.valign_;
	bgrepeat_=other.bgrepeat_;
	isbgcolor_=other.isbgcolor_;
	bgcolor_=other.bgcolor_;
	innerscrollable_=other.innerscrollable_;
	variablesize_=other.variablesize_;
	havemap_=other.havemap_;
	alternatefile_=other.alternatefile_;
	imagesource_=other.imagesource_;
	imagelink_.Copy(other.imagelink_);
	imageinfo_.Copy(other.imageinfo_);
	margins_=other.margins_;
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
		altimageinfo_[i]->Copy(*other.altimageinfo_[i]);

	ProjectModified();
}



wxString ButcherProjectAreaConfigBase::GetFlagsDescription()
{
    wxString ret(wxEmptyString), tmp;
	if (area_->GetAreaClass() == ButcherProjectArea::AC_GLOBAL)
        ret+=wxT("<G>");
    switch (GetAreaKind())
    {
    case AK_NONE:
        ret+=wxT("N");
        break;
    case AK_IMAGE:
        ret+=wxT("I");
        break;
    case AK_MASK:
        ret+=wxT("M");
        break;
    default:
        ret+=wxT("?");
        break;
    }
    if (!GetIsDefault()) ret+=wxT("C");
    if (GetBackground()) ret+=wxT("B");
    if (GetIsBGColor()) ret+=wxT("L");
    if (GetImageSource()==IS_LINK) ret+=wxT("K");
    if (GetAlternateFile()>-1) ret+=wxT("A");
    if (GetInnerScrollable()) ret+=wxT("S");
    if (GetVariableSize()) ret+=wxT("Z");
    if (GetHaveMap()) ret+=wxT("P");
    if (ImageInfo().TransparentColors().size()>0) ret+=wxT("T");
    switch (GetAlign())
    {
    case AA_LEFT: ret+=wxT("@L"); break;
    case AA_CENTER: ret+=wxT("@C"); break;
    case AA_RIGHT: ret+=wxT("@R"); break;
    default: break;
    }
    switch (GetVAlign())
    {
    case AV_NONE: ret+=wxT("$N"); break;
    case AV_MIDDLE: ret+=wxT("$M"); break;
    case AV_BOTTOM: ret+=wxT("$B"); break;
    default: break;
    }
    switch (GetBGRepeat())
    {
    case BR_NONE: ret+=wxT("*N"); break;
    case BR_HORIZONTAL: ret+=wxT("*H"); break;
    case BR_VERTICAL: ret+=wxT("*V"); break;
    default: break;
    }
    tmp=wxEmptyString;
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
    {
        if (altimageinfo_[i]->GetEnabled())
            tmp+=wxString::Format(wxT("%d"), i+1);
    }
    if (!tmp.IsEmpty())
    {
        ret+=wxT("+");
        ret+=tmp;
    }
    return ret;
}




void ButcherProjectAreaConfigBase::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    area_->ProjectModified();
}



void ButcherProjectAreaConfigBase::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseModify::ProjectEvent(event);
    imagelink_.ProjectEvent(event);
    imageinfo_.ProjectEvent(event);
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        altimageinfo_[i]->ProjectEvent(event);
}




void ButcherProjectAreaConfigBase::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

	//wxLogDebug(wxT("BFile Version %d"), dynamic_cast<ButcherProjectMetadataFile*>(metadata->GetFile())->GetBVersion());

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_AREAKIND, ButcherMetadataDataItem::DT_INTEGER)))
        throw ButcherException(_("Invalid metadata for area config"));

    blid_=metadata->Get(BFILE_MDI_ID).GetInteger();
    areakind_=static_cast<areakind_t>(metadata->Get(BFILE_MDI_AREAKIND).GetInteger());

	if (metadata->Exists(BFILE_MDI_LAYOUTTYPE, ButcherMetadataDataItem::DT_INTEGER))
		layouttype_=static_cast<ButcherProjectConsts::layouttype_t>(metadata->Get(BFILE_MDI_LAYOUTTYPE).GetInteger());
    if (metadata->Exists(BFILE_MDI_IMAGEFORMAT, ButcherMetadataDataItem::DT_INTEGER))
        imageinfo_.imageformat_=metadata->Get(BFILE_MDI_IMAGEFORMAT).GetInteger();
    if (metadata->Exists(BFILE_MDI_BACKGROUND, ButcherMetadataDataItem::DT_INTEGER))
        background_=metadata->Get(BFILE_MDI_BACKGROUND).GetBool();
    if (metadata->Exists(BFILE_MDI_FILENAME, ButcherMetadataDataItem::DT_STRING))
        imageinfo_.filename_=metadata->Get(BFILE_MDI_FILENAME).GetString();
    if (metadata->Exists(BFILE_MDI_FILENAMEPREPEND, ButcherMetadataDataItem::DT_INTEGER))
        imageinfo_.filename_prepend_=metadata->Get(BFILE_MDI_FILENAMEPREPEND).GetBool();
    if (metadata->Exists(BFILE_MDI_ALIGN, ButcherMetadataDataItem::DT_INTEGER))
        align_=static_cast<align_t>(metadata->Get(BFILE_MDI_ALIGN).GetInteger());
    if (metadata->Exists(BFILE_MDI_VALIGN, ButcherMetadataDataItem::DT_INTEGER))
        valign_=static_cast<valign_t>(metadata->Get(BFILE_MDI_VALIGN).GetInteger());
    if (metadata->Exists(BFILE_MDI_BGREPEAT, ButcherMetadataDataItem::DT_INTEGER))
        bgrepeat_=static_cast<bgrepeat_t>(metadata->Get(BFILE_MDI_BGREPEAT).GetInteger());
    if (metadata->Exists(BFILE_MDI_ISBGCOLOR, ButcherMetadataDataItem::DT_INTEGER))
        isbgcolor_=metadata->Get(BFILE_MDI_ISBGCOLOR).GetBool();
    if (metadata->Exists(BFILE_MDI_BGCOLOR, ButcherMetadataDataItem::DT_INTEGER))
        bgcolor_.Set(metadata->Get(BFILE_MDI_BGCOLOR).GetInteger());
    if (metadata->Exists(BFILE_MDI_INNERSCROLLABLE, ButcherMetadataDataItem::DT_INTEGER))
        innerscrollable_=metadata->Get(BFILE_MDI_INNERSCROLLABLE).GetBool();
    if (metadata->Exists(BFILE_MDI_VARIABLESIZE, ButcherMetadataDataItem::DT_INTEGER))
        variablesize_=metadata->Get(BFILE_MDI_VARIABLESIZE).GetBool();
    if (metadata->Exists(BFILE_MDI_HAVEMAP, ButcherMetadataDataItem::DT_INTEGER))
        havemap_=metadata->Get(BFILE_MDI_HAVEMAP).GetBool();

    if (metadata->Exists(BFILE_MDI_CONTENT, ButcherMetadataDataItem::DT_STRING))
        content_=metadata->Get(BFILE_MDI_CONTENT).GetString();
    if (metadata->Exists(BFILE_MDI_CELLTAGAPPEND, ButcherMetadataDataItem::DT_STRING))
        celltagappend_=metadata->Get(BFILE_MDI_CELLTAGAPPEND).GetString();
    if (metadata->Exists(BFILE_MDI_IMAGEURL, ButcherMetadataDataItem::DT_STRING))
        imageurl_=metadata->Get(BFILE_MDI_IMAGEURL).GetString();
    if (metadata->Exists(BFILE_MDI_IMAGEURLTAGAPPEND, ButcherMetadataDataItem::DT_STRING))
        imageurltagappend_=metadata->Get(BFILE_MDI_IMAGEURLTAGAPPEND).GetString();
    if (metadata->Exists(BFILE_MDI_IMAGETAGAPPEND, ButcherMetadataDataItem::DT_STRING))
        imagetagappend_=metadata->Get(BFILE_MDI_IMAGETAGAPPEND).GetString();
    if (metadata->Exists(BFILE_MDI_IMAGEMAPNAME, ButcherMetadataDataItem::DT_STRING))
        imagemapname_=metadata->Get(BFILE_MDI_IMAGEMAPNAME).GetString();
    if (metadata->Exists(BFILE_MDI_FILEALTERNATE, ButcherMetadataDataItem::DT_INTEGER))
        alternatefile_=metadata->Get(BFILE_MDI_FILEALTERNATE).GetInteger();
    if (metadata->Exists(BFILE_MDI_IMAGESOURCE, ButcherMetadataDataItem::DT_INTEGER))
        imagesource_=static_cast<imagesource_t>(metadata->Get(BFILE_MDI_IMAGESOURCE).GetInteger());
    if (metadata->Exists(BFILE_MDI_IMAGEFILELINK, ButcherMetadataDataItem::DT_STRING))
        imagelink_.SetBURL(metadata->Get(BFILE_MDI_IMAGEFILELINK).GetString());
    if (metadata->Exists(BFILE_MDI_MARGINLEFT, ButcherMetadataDataItem::DT_INTEGER))
		margins_.SetLeft(metadata->Get(BFILE_MDI_MARGINLEFT).GetInteger());
    if (metadata->Exists(BFILE_MDI_MARGINTOP, ButcherMetadataDataItem::DT_INTEGER))
		margins_.SetTop(metadata->Get(BFILE_MDI_MARGINTOP).GetInteger());
    if (metadata->Exists(BFILE_MDI_MARGINRIGHT, ButcherMetadataDataItem::DT_INTEGER))
		margins_.SetRight(metadata->Get(BFILE_MDI_MARGINRIGHT).GetInteger());
    if (metadata->Exists(BFILE_MDI_MARGINBOTTOM, ButcherMetadataDataItem::DT_INTEGER))
		margins_.SetBottom(metadata->Get(BFILE_MDI_MARGINBOTTOM).GetInteger());

    imageinfo_.LoadMetadata(metadata);

    if (metadata->Exists(BFILE_MDI_ALTERNATEIMAGEINFO, ButcherMetadataDataItem::DT_METADATA) &&
        metadata->Get(BFILE_MDI_ALTERNATEIMAGEINFO).GetMetadata().Exists(BFILE_MD_ALTERNATEIMAGEINFO))
        LoadMetadataAlternateImageInfo(&metadata->Get(BFILE_MDI_ALTERNATEIMAGEINFO).GetMetadata().Get(BFILE_MD_ALTERNATEIMAGEINFO));
}



void ButcherProjectAreaConfigBase::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_AREAKIND, areakind_))->
		MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_LAYOUTTYPE, layouttype_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_IMAGEFORMAT, imageinfo_.imageformat_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_BACKGROUND, background_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FILENAMEPREPEND, imageinfo_.filename_prepend_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ALIGN, align_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_VALIGN, valign_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_BGREPEAT, bgrepeat_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ISBGCOLOR, isbgcolor_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_BGCOLOR, BFILE_RGB(bgcolor_.Red(), bgcolor_.Green(), bgcolor_.Blue())))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_HAVEMAP, havemap_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_INNERSCROLLABLE, innerscrollable_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_VARIABLESIZE, variablesize_))->
        MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_ALTERNATEIMAGEINFO))->
		MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MARGINLEFT, margins_.GetLeft()))->
		MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MARGINTOP, margins_.GetTop()))->
		MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MARGINRIGHT, margins_.GetRight()))->
		MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MARGINBOTTOM, margins_.GetBottom()) );

    if (!imageinfo_.filename_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_FILENAME, imageinfo_.filename_));
    if (!content_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_CONTENT, content_));
    if (!celltagappend_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_CELLTAGAPPEND, celltagappend_));
    if (!imageurl_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_IMAGEURL, imageurl_));
    if (!imageurltagappend_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_IMAGEURLTAGAPPEND, imageurltagappend_));
    if (!imagetagappend_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_IMAGETAGAPPEND, imagetagappend_));
    if (!imagemapname_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_IMAGEMAPNAME, imagemapname_));
    if (alternatefile_>=0)
        metadata->MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FILEALTERNATE, alternatefile_));
    if (imagesource_!=IS_AREA)
        metadata->MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_IMAGESOURCE, imagesource_));
    if (imagelink_.GetLinkType()!=ButcherProjectFileLink::IL_NONE)
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_IMAGEFILELINK, imagelink_.GetBURL()));

    imageinfo_.SaveMetadata(metadata);

    SaveMetadataAlternateImageInfo(&metadata->Get(BFILE_MDI_ALTERNATEIMAGEINFO).GetMetadata().Get(BFILE_MD_ALTERNATEIMAGEINFO, true));
}




void ButcherProjectAreaConfigBase::LoadMetadataAlternateImageInfo(ButcherMetadataDataList *metadatalist)
{
    ButcherMetadataData *metadata;
    for (unsigned long i=0; i<metadatalist->Count(); i++)
    {
        metadata=&metadatalist->Get(i);

        if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)))
            throw ButcherException(_("Invalid metadata for area config"));

        int id=metadata->Get(BFILE_MDI_ID).GetInteger();
        if (id>=0 && id<BUTCHERCONST_VIEW_MAXALTERNATE)
        {
            altimageinfo_[id]->LoadMetadata(metadata);
        }
    }
}



void ButcherProjectAreaConfigBase::SaveMetadataAlternateImageInfo(ButcherMetadataDataList *metadatalist)
{
    ButcherMetadataData *metadata;
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
    {
        if (altimageinfo_[i]->enabled_)
        {
            metadata=metadatalist->Add();
            metadata->
                MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, i));
            altimageinfo_[i]->SaveMetadata(metadata);
        }
    }
}


