//=============================================================================
/**
 *  @file    BProc.h
 *
 *  $Id: BProc.cpp,v 1.3 2008/06/09 14:27:27 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-10
 */
//=============================================================================
#include <wx/filename.h>
#include "BProc.h"
#include "wxpopupmenu.h"

#include "DialogMaskAreaEdit.h"
#include "DialogMaskAreaGlobalEdit.h"
#include "DialogMaskAreaMapEdit.h"
#include "DialogImageFormatEdit.h"
#include "DialogAdjustSize.h"
#include "DialogMaskAreaImageFormat.h"
#include "DialogMaskAreaFileAlternate.h"
#include "DialogFileLink.h"
#include "DialogMaskAreaSlice.h"
#include "DialogMaskAreaListEdit.h"



/////////////////////////////////
// CLASS
//      ButcherProjectProc_AreaMenu
/////////////////////////////////



wxMenu* ButcherProjectProc_AreaMenu::BuildAreaMenu(wxMenu *rootmenu,
    int index, ButcherProjectArea *area)
{
    int amenustart=(ID_AREAMENU_MAX-ID_AREAMENU_MIN+1)*index;

    ButcherProjectAreaConfig *acfg=area->Configs().Get(view_->GetBLId());

    wxString mtit=area->GetFullDescription();
    if (area->GetAreaClass() != ButcherProjectArea::AC_MAP)
        mtit+=wxString::Format(wxT(" [%s]"), acfg->GetFlagsDescription().c_str());

    // image format
    if (acfg->GetAreaKind()==ButcherProjectAreaConfig::AK_IMAGE||acfg->GetBackground())
    {
        mtit+=wxString::Format(wxT(" [%s%s]"),
            (acfg->GetBackground()?_("BG:"):wxT("")),
            acfg->ImageInfo().GetImageFormat()->GetName().c_str());

        // preview format/size
        if (showpreview_ &&
            acfg->GetPreview(view_->GetBLId())->HavePreview() &&
            acfg->GetPreview(view_->GetBLId())->GetPreview() &&
            !acfg->GetPreview(view_->GetBLId())->IsError())
        {
            if (acfg->GetPreview(view_->GetBLId())->GetPreview()->GetFileSize() >= 0)
            {
                mtit+=wxFileName::GetHumanReadableSize(acfg->GetPreview(view_->GetBLId())->GetPreview()->GetFileSize(), wxT("0")).c_str();
            }
        }
    }

    wxMenuItem *newi;
    wxMenu *ret=new wxMenu(mtit);

    if (area->GetAreaClass() == ButcherProjectArea::AC_DEFAULT ||
        area->GetAreaClass() == ButcherProjectArea::AC_GLOBAL)
    {
        if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_ADVANCED)
        {
            // CUSTOMIZE
            newi=ret->AppendCheckItem(amenustart+ID_AREAMENU_CONFIG, _("&Customize"), _("Customize this area"));
            bool cstmenu=area->Customizable(view_->GetBLId());
            newi->Enable(cstmenu);
            newi->Check(cstmenu && area->Configs().Exists(view_->GetBLId()));

            if (area->Editable(view_->GetBLId()))
			{
				// COPY
				wxMenu *amcopy=new wxMenu(_("Cop&y from"));

				amcopy->Append(amenustart+ID_AREAMENU_COPY_ITEMBEGIN+0,
					_("DEFAULT"), _("DEFAULT"));

				for (ButcherProjectViews::iterator i=GetProject()->Views().begin();
					 i!=GetProject()->Views().end(); i++)
				{
					if (i.first()!=view_->GetBLId() && area->Configs().Exists(i.first()))
						amcopy->Append(amenustart+ID_AREAMENU_COPY_ITEMBEGIN+i.first(),
							i->GetName(), i->GetName());
				}
				ret->Append(wxID_ANY, _("Cop&y from"), amcopy, _("Copy from"));

				// SEPARATOR
				ret->AppendSeparator();
			}
        }


		if (area->Editable(view_->GetBLId()))
		{
			// KIND
			wxMenu *amkind=new wxMenu(_("Kind"));
			amkind->AppendRadioItem(amenustart+ID_AREAMENU_KINDNONE, _("&None"), _("None"))->Check(acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_NONE);
			amkind->AppendRadioItem(amenustart+ID_AREAMENU_KINDIMAGE, _("&Image"), _("Image"))->Check(acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_IMAGE);
			amkind->AppendRadioItem(amenustart+ID_AREAMENU_KINDMASK, _("&Mask"), _("Mask"))->Check(acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK);
			ret->Append(wxID_ANY, _("&Kind"), amkind, _("Area kind"));
			amkind->SetEventHandler(rootmenu);

			// IMAGE FORMAT
			wxMenu *amformat=new wxMenu(_("&Image Format"));

			for (ButcherProjectImageFormats::iterator fmtaddi=area->GetProject()->ImageFormats().begin(); fmtaddi!=area->GetProject()->ImageFormats().end(); fmtaddi++)
			{
				newi=amformat->AppendRadioItem(amenustart+ID_AREAMENU_IMAGEFORMAT_ITEMBEGIN+fmtaddi.first(),
					fmtaddi->GetName(),
					fmtaddi->GetName());
				if (acfg->ImageInfo().GetImageFormatId()==fmtaddi.first())
					newi->Check(true);
			}

			amformat->AppendSeparator();
			amformat->Append(amenustart+ID_AREAMENU_IMAGEFORMAT_SELECT, _("&Select"), _("Select image format"));
			ret->Append(wxID_ANY, _("&Image Format"), amformat, _("Image format"));
			amformat->SetEventHandler(rootmenu);

			if (area->GetAreaClass() != ButcherProjectArea::AC_GLOBAL)
				// BACKGROUND
				ret->AppendCheckItem(amenustart+ID_AREAMENU_BACKGROUND, _("&Background Image"), _("Generate background image"))->Check(acfg->GetBackground());

			// SEPARATOR
			ret->AppendSeparator();
		}
    }

    if (area->GetAreaClass() == ButcherProjectArea::AC_MAP ||
        area->GetAreaClass() == ButcherProjectArea::AC_GLOBAL)
    {
		if (area->Editable(view_->GetBLId()))
		{
			// DELETE
			ret->Append(amenustart+ID_AREAMENU_DELETE, _("&Delete"), _("Delete"));

			// SEPARATOR
			ret->AppendSeparator();
		}
    }
    if (area->GetAreaClass() != ButcherProjectArea::AC_MAP)
    {
		if (area->Editable(view_->GetBLId()))
		{
			if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_NORMAL)
			{
				// IMAGE FORMAT
				ret->Append(amenustart+ID_AREAMENU_IMAGEFORMAT_SELECT, _("Image &format properties"), _("Image format properties"));
			}
			if (ButcherOptions::GetEditMode(GetProject()->GetOptions())>=ButcherOptions::EM_ADVANCED)
			{
				// FILE ALTERNATE
				ret->Append(amenustart+ID_AREAMENU_FILEALTERNATE, _("Al&ternate files"), _("Alternate files"));
			}

			//if (acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK)
			if (acfg->CanSlice())
				// SLICE
				ret->Append(amenustart+ID_AREAMENU_SLICE, _("Grid &Slice"), _("Slice mask in grid format"));
		}
	}

	if (area->Editable(view_->GetBLId()))
	{
		// SIZE
		ret->Append(amenustart+ID_AREAMENU_SIZE, _("Position and &Size"), _("Position and Size"));

		// SEPARATOR
		ret->AppendSeparator();

		// SAVE IMAGE AS
	#ifndef HTMLBUTCHER_DEMO
		ret->Append(amenustart+ID_AREAMENU_SAVERAWIMAGEAS, _("Save ra&w image as..."), _("Export raw image to file"));
	#endif
		ret->Append(amenustart+ID_AREAMENU_SAVEIMAGEAS, _("Sa&ve image as..."), _("Export image to file"));

		// SEPARATOR
		ret->AppendSeparator();

		// RENAME
		ret->Append(amenustart+ID_AREAMENU_RENAME, _("&Rename"), _("Rename this area"));

		// PROPERTIES
		ret->Append(amenustart+ID_AREAMENU_PROPERTIES, _("&Properties"), _("Properties"));
	}
    return ret;
}



void ButcherProjectProc_AreaMenu::ShowAreaMenu()
{
    wxPopupMenu areamenu(_("Areas"));
    ButcherProjectAreaConfig *acfg;

    wxMenu *itemmenu;
    for (unsigned long i=0; i<select_->GetCount(); i++)
    {
            acfg=select_->GetItem(i)->Configs().Get(view_->GetBLId());

            itemmenu=BuildAreaMenu(&areamenu, i, select_->GetItem(i));
            areamenu.Append(wxID_ANY,
                itemmenu->GetTitle(),
                itemmenu);
    }

    window_->PopupMenu(&areamenu);
    if (areamenu.GetSelectedId()!=-1)
    {
        ProcessAreaMenu(areamenu.GetSelectedId());
    }
}



void ButcherProjectProc_AreaMenu::ProcessAreaMenu(int menuid)
{
    int selmenu=ID_AREAMENU_MIN + ((menuid-ID_AREAMENU_MIN) % (ID_AREAMENU_MAX-ID_AREAMENU_MIN+1));
    int selitem=(menuid-selmenu) / (ID_AREAMENU_MAX-ID_AREAMENU_MIN+1);
    ButcherProjectAreaConfig::areakind_t ak;
    ButcherProjectArea *area=select_->GetItem(selitem);

    //if (view_ == NULL || menuid_==0) return;

    switch (selmenu)
    {
    case ID_AREAMENU_CONFIG:


		if (area->Configs().Exists(view_->GetBLId()))
            area->Configs().Delete(view_->GetBLId());
        else
            area->Configs().Add(view_->GetBLId());
        break;
    case ID_AREAMENU_KINDNONE:
    case ID_AREAMENU_KINDIMAGE:
    case ID_AREAMENU_KINDMASK:
        switch (selmenu)
        {
        case ID_AREAMENU_KINDIMAGE:
            ak=ButcherProjectAreaConfig::AK_IMAGE;
            break;
        case ID_AREAMENU_KINDMASK:
            ak=ButcherProjectAreaConfig::AK_MASK;
            break;
        default:
            ak=ButcherProjectAreaConfig::AK_NONE;
            break;
        }

        area->Configs().Get(view_->GetBLId())->SetAreaKind(ak);
        break;
    case ID_AREAMENU_BACKGROUND:
        area->Configs().Get(view_->GetBLId())->SetBackground(!area->Configs().Get(view_->GetBLId())->GetBackground());
        break;
    case ID_AREAMENU_DELETE:
        {
            if (area->GetAreaClass() == ButcherProjectArea::AC_MAP)
            {
                area->GetContainer()->DeleteArea(area);
            }
            else if (area->GetAreaClass() == ButcherProjectArea::AC_GLOBAL)
            {
                area->GetContainer()->DeleteArea(area);
            }
        }
        break;
    case ID_AREAMENU_SIZE:
        {
                HTMLButcherAdjustSizeDialog d(window_, wxID_ANY, GetProject(), _("Adjust Position and Size: ")+area->GetFullDescription());
                d.SetArea(view_->GetFileCurrent(area->Configs().Get(view_->GetBLId())->GetAlternateFile()), area);
                if (d.ShowModal() == wxID_OK)
                {

                }
                else
                    d.RevertToDefault();
        }
        break;
    case ID_AREAMENU_IMAGEFORMAT_SELECT:
        {
			HTMLButcherMaskAreaImageFormatDialog d(window_, wxID_ANY, GetProject());
            d.SetArea(view_->GetFileCurrent(area->Configs().Get(view_->GetBLId())->GetAlternateFile()), area);
            d.Load(&area->Configs().Get(view_->GetBLId())->ImageInfo());
			if (d.ShowModal()==wxID_OK) 
			{

                d.Save(&area->Configs().Get(view_->GetBLId())->ImageInfo());
			}
        }
        break;
    case ID_AREAMENU_FILEALTERNATE:
        {
            HTMLButcherMaskAreaFileAlternateDialog d(window_, wxID_ANY, GetProject());
            d.Load(view_, area,
                area->Configs().Get(view_->GetBLId()));
            if (d.ShowModal()==wxID_OK)
			{

                d.Save(view_, area, area->Configs().Get(view_->GetBLId()));
			}
        }
        break;
    case ID_AREAMENU_SLICE:
        {
            HTMLButcherMaskAreaSliceDialog d(window_, wxID_ANY, GetProject());
            d.Load(view_, area,
                area->Configs().Get(view_->GetBLId()));
            if (d.ShowModal()==wxID_OK)
			{

                d.Save(view_, area, area->Configs().Get(view_->GetBLId()));
			}
        }
        break;
#ifndef HTMLBUTCHER_DEMO
	case ID_AREAMENU_SAVERAWIMAGEAS:
        {
            wxFileDialog d(window_, _("Save file"), wxEmptyString, wxEmptyString,
                _("JPEG image (*.jpg)|*.jpg|GIF image (*.gif)|*.gif|PNG image (*.png)|*.png|BMP image (*.bmp)|*.bmp|TIFF image (*.tif)|*.tif|XPM image (*.xpm)|*.xpm"),
                wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
            if (d.ShowModal()==wxID_OK)
            {
				wxFileName fn(d.GetPath());
				std::unique_ptr<ButcherImage> aimg(view_->GetFile()->GetSubImage(area->GetGlobalRect()));
                aimg->Save(ButcherImage::GetExtFormat(fn.GetExt()), d.GetPath(), 0);
            }

        }
        break;
#endif
    case ID_AREAMENU_SAVEIMAGEAS:
        {
			wxString imgfilename=area->GetImageFilename(view_->GetBLId());
			wxFileName fn(imgfilename);

			wxFileDialog d(window_, _("Save file"), wxEmptyString, imgfilename,
				wxString::Format(wxT("*.%s"), fn.GetExt().c_str()),
                wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
            if (d.ShowModal()==wxID_OK)
            {
				std::unique_ptr<ButcherImage> img(view_->CreateAreaImage(area));
				if (img.get())
					img->Save(area->Configs().Get(view_->GetBLId())->ImageInfo().GetSaveParams(), d.GetPath());
				else
					throw ButcherException(_("Could not save area image - try saving raw area image."));

            }
        }
        break;
    case ID_AREAMENU_RENAME:
        {
			wxString newname=wxGetTextFromUser(_("Enter area name"), _("Rename area"), area->GetName(), window_);
			if (!newname.IsEmpty())
				area->SetName(newname);
        }
        break;
    case ID_AREAMENU_PROPERTIES:
        {
            if (area->GetAreaClass() == ButcherProjectArea::AC_GLOBAL)
            {
                HTMLButcherMaskAreaGlobalEditDialog d(window_, wxID_ANY, GetProject(), area->GetFullDescription());
                d.Load(view_,  area);
                if (d.ShowModal() == wxID_OK)
                {

					d.Save(view_,  area);
                }
            }
            else if (area->GetAreaClass() == ButcherProjectArea::AC_MAP)
            {
                HTMLButcherMaskAreaMapEditDialog d(window_, wxID_ANY, GetProject(), area->GetFullDescription());
                d.Load(area, area->Configs().Get(view_->GetBLId()));
                d.SetPreview(view_->GetFile()->GetSubImage(
                    area->GetGlobalRect()
                ));
                if (d.ShowModal() == wxID_OK)
                {


                    d.Save(area, area->Configs().Get(view_->GetBLId()));
                    //area->GetMask()->DoModified();
                }
            }
            else
            {
                HTMLButcherMaskAreaEditDialog d(window_, wxID_ANY, GetProject(), area->GetFullDescription());
                d.Load(view_,  area);
                if (d.ShowModal() == wxID_OK)
                {


                    d.Save(view_,  area);
                    //area->GetMask()->DoModified();
                }
            }
        }
        break;
    default:
        {
			// IMAGE FORMAT
            if (selmenu>=ID_AREAMENU_IMAGEFORMAT_ITEMBEGIN && selmenu<=ID_AREAMENU_IMAGEFORMAT_ITEMEND)
			{
				for (ButcherProjectImageFormats::iterator fmtaddi=area->GetProject()->ImageFormats().begin(); fmtaddi!=area->GetProject()->ImageFormats().end(); fmtaddi++)
				{
					if (selmenu == (int)(ID_AREAMENU_IMAGEFORMAT_ITEMBEGIN+(fmtaddi.first())))
					{
						area->Configs().Get(view_->GetBLId())->ImageInfo().SetImageFormatId(fmtaddi.first());
						//area->GetMask()->DoModified();
						break;
					}
				}
			}
			// COPY
            if (selmenu>=ID_AREAMENU_COPY_ITEMBEGIN && selmenu<=ID_AREAMENU_COPY_ITEMEND)
			{
				int copyid=selmenu-ID_AREAMENU_COPY_ITEMBEGIN;
				area->Configs().Get(view_->GetBLId())->Copy(*area->Configs().Get(copyid));
			}
        }
        break;
    }
}




/////////////////////////////////
// CLASS
//      ButcherProjectProc_LineMenu
/////////////////////////////////



wxMenu* ButcherProjectProc_LineMenu::BuildLineMenu(wxMenu *rootmenu,
    int index, ButcherProjectLine *line)
{
    int lmenustart=(ID_LINEMENU_MAX-ID_LINEMENU_MIN+1)*index;

    //wxMenuItem *newi;
    wxMenu *ret=new wxMenu(line->GetFullDescription());

    if (line->GetContainer()->GetAreaClass()==ButcherProjectArea::AC_DEFAULT)
    {
        ret->Append(lmenustart+ID_LINEMENU_DELETE, _("&Delete"), _("Delete line"));
        //ret->AppendSeparator();
    }
    //ret->Append(lmenustart+ID_LINEMENU_PROPERTIES, _("&Properties"), _("Properties"));
    return ret;
}



void ButcherProjectProc_LineMenu::ShowLineMenu()
{
    wxPopupMenu linemenu(_("Lines"));

    wxMenu *itemmenu;
    for (unsigned long i=0; i<select_->GetCount(); i++)
    {
        itemmenu=BuildLineMenu(&linemenu, i, select_->GetItem(i));
        linemenu.Append(wxID_ANY, select_->GetItem(i)->GetFullDescription(),
            itemmenu);
        //itemmenu->SetEventHandler(&linemenu);
    }

    window_->PopupMenu(&linemenu);
    //view_->HoverClear();
    if (linemenu.GetSelectedId()!=-1)
    {
        ProcessLineMenu(linemenu.GetSelectedId());
    }
    //view_->SelectionClear();
}



void ButcherProjectProc_LineMenu::ProcessLineMenu(int menuid)
{
    int selmenu=ID_LINEMENU_MIN + ((menuid-ID_LINEMENU_MIN) % (ID_LINEMENU_MAX-ID_LINEMENU_MIN+1));
    int selitem=(menuid-selmenu) / (ID_LINEMENU_MAX-ID_LINEMENU_MIN+1);
    ButcherProjectLine *line=select_->GetItem(selitem);
    ButcherProjectMaskContainer *maskc=line->GetContainer();

    //if (view_ == NULL || menuid_==0) return;

    switch (selmenu)
    {
    case ID_LINEMENU_DELETE:
        if (maskc->GetAreaClass()==ButcherProjectArea::AC_DEFAULT)
            if (!dynamic_cast<ButcherProjectMaskContainerArea*>(maskc)->DeleteLine(line))
				throw ButcherException(_("This line cannot be deleted."));
        break;
    case ID_LINEMENU_PROPERTIES:
        break;
    }
}


