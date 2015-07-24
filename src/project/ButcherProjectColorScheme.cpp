//=============================================================================
/**
 *  @file   ButcherProjectColorScheme.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectColorScheme.h"
#include "ButcherFileDefs.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProject.h"



/////////////////////////////////
// CLASS
//      ButcherProjectColorScheme
/////////////////////////////////
ButcherProjectColorScheme::ButcherProjectColorScheme(ButcherProject *project, const wxString &name) :
    ButcherProjectBaseBLId(project), cscheme_(), name_(name)
{
    Initialize();
}




ButcherProjectColorScheme::ButcherProjectColorScheme(ButcherProject *project) :
    ButcherProjectBaseBLId(project), cscheme_(), name_(wxT("_TEMP_"))
{
    Initialize();
}




void ButcherProjectColorScheme::Initialize()
{
    ButcherProjectBaseAutoDisable autodis(this);

    SetColor(BCOLOR_AREABORDER, *wxGREEN);
#ifdef QT_HIDE_FROM
	SetColor(BCOLOR_AREAGLOBALBORDER, wxTheColourDatabase->Find(wxT("YELLOW")));
    //SetColor(BCOLOR_AREAMAPBORDER, wxTheColourDatabase->Find(wxT("GREEN YELLOW")));
    SetColor(BCOLOR_AREAMAPBORDER, wxTheColourDatabase->Find(wxT("BROWN")));
#else // QT_HIDE_FROM
	SetColor(BCOLOR_AREAGLOBALBORDER, *wxGREEN);
	//SetColor(BCOLOR_AREAMAPBORDER, wxTheColourDatabase->Find(wxT("GREEN YELLOW")));
	SetColor(BCOLOR_AREAMAPBORDER, *wxGREEN);
#endif QT_HIDE_FROM
	SetColor(BCOLOR_AREABORDERSELECTED, *wxBLUE);
    SetColor(BCOLOR_AREABORDERHOVER, *wxRED);
    SetColor(BCOLOR_AREATEXT, *wxBLACK);
}




void ButcherProjectColorScheme::SetColor(unsigned int colorid, const wxColour &color)
{
    cscheme_[colorid]=color;
    ProjectModified();
}




wxColour ButcherProjectColorScheme::GetColor(unsigned int colorid)
{
        if (cscheme_.find(colorid)!=cscheme_.end())
            return cscheme_[colorid];
        return *wxBLACK;
}




wxPen *ButcherProjectColorScheme::GetPen(unsigned int colorid, int width, int style)
{
    return wxThePenList->FindOrCreatePen(GetColor(colorid), width, style);
}




wxPen *ButcherProjectColorScheme::GetSelectionPen(unsigned int colorid, int width,
    selectionpen_t selectionpen)
{
    wxPen *ret;
    switch (selectionpen)
    {
    case SP_HIDDEN:
        ret=wxThePenList->FindOrCreatePen(GetColor(colorid), width, wxUSER_DASH);
        ret->SetDashes(ButcherProjectHiddenDashCount, ButcherProjectHiddenDash);
		//ret=const_cast<wxPen*>(wxTRANSPARENT_PEN);
        break;
    default:
        ret=wxThePenList->FindOrCreatePen(GetColor(colorid), width, /*wxUSER_DASH*/wxSOLID);
        ret->SetDashes(ButcherProjectSelectionDashCount, ButcherProjectSelectionDash);
        break;
    }
    return ret;
}



wxBrush *ButcherProjectColorScheme::GetBrush(unsigned int colorid, int style)
{
    return wxTheBrushList->FindOrCreateBrush(GetColor(colorid), style);
}




void ButcherProjectColorScheme::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING)) ||
        (!metadata->Exists(BFILE_MDI_COLORITEM, ButcherMetadataDataItem::DT_METADATA)))
        throw ButcherException(_("Invalid metadata for colorscheme"));

    SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    name_=metadata->Get(BFILE_MDI_NAME).GetString();

    if (metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Exists(BFILE_MD_COLORSCHEME_ITEM))
        for (unsigned int i=0; i<metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Get(BFILE_MD_COLORSCHEME_ITEM).Count(); i++)
        {
            LoadMetadataColorItem(&metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Get(BFILE_MD_COLORSCHEME_ITEM).Get(i));
        }
}




void ButcherProjectColorScheme::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_))->
        MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_COLORITEM));

    SaveMetadataColorItem(BCOLOR_AREABORDER, metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Get(BFILE_MD_COLORSCHEME_ITEM, true).Add());
    SaveMetadataColorItem(BCOLOR_AREAGLOBALBORDER, metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Get(BFILE_MD_COLORSCHEME_ITEM, true).Add());
    SaveMetadataColorItem(BCOLOR_AREAMAPBORDER, metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Get(BFILE_MD_COLORSCHEME_ITEM, true).Add());
    SaveMetadataColorItem(BCOLOR_AREABORDERSELECTED, metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Get(BFILE_MD_COLORSCHEME_ITEM, true).Add());
    SaveMetadataColorItem(BCOLOR_AREABORDERHOVER, metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Get(BFILE_MD_COLORSCHEME_ITEM, true).Add());
    SaveMetadataColorItem(BCOLOR_AREATEXT, metadata->Get(BFILE_MDI_COLORITEM).GetMetadata().Get(BFILE_MD_COLORSCHEME_ITEM, true).Add());
}



void ButcherProjectColorScheme::LoadMetadataColorItem(ButcherMetadataData *metadata)
{
    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_COLOR, ButcherMetadataDataItem::DT_INTEGER)))
        throw ButcherException(_("Invalid metadata for colorscheme color item"));

    wxColor tmpcolor;
    tmpcolor.Set(metadata->Get(BFILE_MDI_COLOR).GetInteger());
    SetColor(metadata->Get(BFILE_MDI_ID).GetInteger(), tmpcolor);
}




void ButcherProjectColorScheme::SaveMetadataColorItem(unsigned int colorid, ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, colorid))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_COLOR,
            BFILE_RGB(GetColor(colorid).Red(),
                GetColor(colorid).Green(),
                GetColor(colorid).Blue())));
}


