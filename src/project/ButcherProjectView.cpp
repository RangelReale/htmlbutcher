//=============================================================================
/**
 *  @file   ButcherProjectView.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectView.h"
#include "ButcherProjectFilePath.h"
#include "ButcherProjectAreaConfig.h"
#include "ButcherProject.h"

#include <cppcomp/wxccufile.h>
#include <wx/filename.h>
#include <wx/wfstream.h>



using namespace cppcomp;

/////////////////////////////////
// CLASS
//      ButcherProjectView
/////////////////////////////////
ButcherProjectView::ButcherProjectView(ButcherProject *project, const wxString &name,
    BLID_t fileid, BLID_t maskid) :
ButcherProjectBaseBLId(project), name_(name), layouttype_(ButcherProjectConsts::LTYPE_DEFAULT), fileid_(fileid),
        maskid_(maskid), COMPATmaskconfigid_(0),
        imagefilepathid_(project->FilePaths().DefaultId()), cssfileid_(0),
        items_(ITEM_HTML|ITEM_IMAGES|ITEM_IMAGESGLOBAL), filename_(wxEmptyString), bodytagadd_(wxEmptyString),
        tabletagadd_(wxEmptyString), title_(wxEmptyString), head_(wxEmptyString), bodyprepend_(wxEmptyString), bodyappend_(wxEmptyString),
        encoding_(ButcherHTMLConsts::BE_ISO8859_1), assortedfilegroups_(this),
        bgimage_(this), previewbgimage_(this), bgrepeat_(ButcherProjectAreaConfig::BR_NONE),
        isbgcolor_(false), isstretch_(false), styleseparated_(false),
		bgcolor_(), previewbgimagedraw_()
{
    memset(filealternateid_, 0, sizeof(filealternateid_));
}



ButcherProjectView::ButcherProjectView(ButcherProject *project) :
    ButcherProjectBaseBLId(project), name_(wxT("__TEMP__")), layouttype_(ButcherProjectConsts::LTYPE_DEFAULT), fileid_(0),
        maskid_(0), COMPATmaskconfigid_(0),
        imagefilepathid_(project->FilePaths().DefaultId()), cssfileid_(0),
        items_(ITEM_HTML|ITEM_IMAGES|ITEM_IMAGESGLOBAL), filename_(wxEmptyString), bodytagadd_(wxEmptyString),
        tabletagadd_(wxEmptyString), title_(wxEmptyString), head_(wxEmptyString), bodyprepend_(wxEmptyString), bodyappend_(wxEmptyString),
        encoding_(ButcherHTMLConsts::BE_ISO8859_1), assortedfilegroups_(this),
        bgimage_(this), previewbgimage_(this), bgrepeat_(ButcherProjectAreaConfig::BR_NONE),
        isbgcolor_(false), isstretch_(false), styleseparated_(false),
		bgcolor_(), previewbgimagedraw_()
{
    memset(filealternateid_, 0, sizeof(filealternateid_));
}




ButcherProjectView::~ButcherProjectView()
{

}




ButcherProjectFile *ButcherProjectView::GetFile()
{
    return GetProject()->Files().Get(fileid_);
}




ButcherProjectFile *ButcherProjectView::GetFileAlternate(int index)
{
    if (index<0 || index>BUTCHERCONST_VIEW_MAXALTERNATE-1) return NULL;

    if (filealternateid_[index]>0)
        return GetProject()->Files().Get(filealternateid_[index]);
    return NULL;
}



ButcherProjectFile *ButcherProjectView::GetFileCurrent(int alternatefile)
{
    ButcherProjectFile *ret=GetFileAlternate(alternatefile);
    if (!ret) ret=GetFile();
    return ret;
}




bool ButcherProjectView::HaveFileAlternate()
{
    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
        if (filealternateid_[i]>0) return true;
    return false;
}




ButcherProjectMask *ButcherProjectView::GetMask()
{
    return GetProject()->Masks().Get(maskid_);
}




/*
ButcherProjectMaskConfig *ButcherProjectView::GetMaskConfig()
{
    return GetMask()->Configs().Get(maskconfigid_);
}
*/

ButcherImage *ButcherProjectView::CreateAreaImage(ButcherProjectArea *area, int alternate)
{
    if (area->Configs().Get(GetBLId())->GetImageSource()==ButcherProjectAreaConfig::IS_LINK)
    {
        if (!area->Configs().Get(GetBLId())->ImageLink().IsImage()) return NULL;
        return area->Configs().Get(GetBLId())->ImageLink().CreateImage();
    } else {
        return CreateAreaImageDefault(area, alternate);
    }
}



ButcherImage *ButcherProjectView::CreateAreaImageDefault(ButcherProjectArea *area, int alternate)
{
    if (alternate==-1)
		return GetFileCurrent(area->Configs().Get(GetBLId())->GetAlternateFile())->GetSubImage(area->Configs().Get(GetBLId())->GetGlobalRect());
    return GetFileCurrent(alternate)->GetSubImage(area->Configs().Get(GetBLId())->GetGlobalRect());
}




ButcherImage *ButcherProjectView::CreateAreaErrorImage(ButcherProjectArea *area)
{
    wxBitmap wm(area->GetWidth(), area->GetHeight(), -1);
    wxMemoryDC wmdc;

    wmdc.SetPen(*wxTRANSPARENT_PEN);
    wmdc.SetBrush(*wxRED_BRUSH);
    wmdc.DrawRectangle(wxPoint(0, 0), wxSize(area->GetWidth(), area->GetHeight()));
    //wxFont cfont(wmdc.GetFont());
    //wmdc.SetFont(
    wmdc.SetTextForeground(wxColour(wxT("WHITE")));
    wmdc.DrawText(_("IMAGE ERROR"), 10, 10);

    wxImage img(wm.ConvertToImage());
    return ButcherImageFactory::Load(img);
}




wxString ButcherProjectView::GetBaseFilename()
{
    if (name_.IsEmpty())
        return wxString::Format(wxT("view%d"), GetBLId());
    return ButcherProjectFilePath::GetValidFilename(name_);
}



wxString ButcherProjectView::GetDefaultFilename()
{
    if (!filename_.IsEmpty()) return ButcherProjectFilePath::GetValidFilename(filename_);
    if (!name_.IsEmpty()) return ButcherProjectFilePath::GetValidFilename(name_)+wxT(".html");
    return GetBaseFilename()+wxT(".html");
}




wxString ButcherProjectView::GetImageDir(const wxString &basepath, bool formatted)
{
    return GetProject()->FilePaths().GetFilePath(imagefilepathid_,
        basepath, true, wxT("images"));


/*
    wxString ret=wxT("images");
    if (imagefilepathid_>0)
        ret=GetProject()->FilePaths().Get(imagefilepathid_)->GetPath();

    if (formatted)
    {
        ret.Replace(wxT("\\"), wxT("/"));

        if (!ret.EndsWith(wxT("/")))
            ret+=wxT("/");
    }

    return ret;
*/
}



void ButcherProjectView::DrawPreview(ButcherViewBase *view, wxDC *dc)
{
    wxRect vrect(view->PosToClient(wxRect(0, 0, view->GetDesignWidth(), view->GetDesignHeight())));

    if (isbgcolor_)
    {
        dc->SetPen(*wxTRANSPARENT_PEN);
        dc->SetBrush(*wxTheBrushList->FindOrCreateBrush(bgcolor_));

        dc->DrawRectangle(vrect);

        dc->SetPen(wxNullPen);
        dc->SetBrush(wxNullBrush);
    }

    if (bgimage_.GetLinkType()!=ButcherProjectFileLink::IL_NONE && bgimage_.IsImage())
    {
        if (!previewbgimagedraw_.get() || !previewbgimage_.Compare(bgimage_))
        {
            previewbgimagedraw_.reset(bgimage_.CreateImage());
            previewbgimage_.SetBURL(bgimage_.GetBURL());
        }

        wxRect dr(vrect.GetTopLeft(), previewbgimagedraw_->GetImageRect().GetSize());;
        wxRect darearect=view->PosToClient(dr);

        ButcherImage::repeat_t drepeat=ButcherImage::DR_NONE;
        switch (bgrepeat_)
        {
        case ButcherProjectAreaConfig::BR_ALL: drepeat=ButcherImage::DR_ALL; break;
        case ButcherProjectAreaConfig::BR_HORIZONTAL: drepeat=ButcherImage::DR_HORIZONTAL; break;
        case ButcherProjectAreaConfig::BR_VERTICAL: drepeat=ButcherImage::DR_VERTICAL; break;
        default: break;
        }

        previewbgimagedraw_->DrawRepeat(*dc, darearect.GetSize(), vrect, drepeat);
    }
}




void ButcherProjectView::CheckFile(ButcherProjectViewFileCheck &check)
{
    ccu_Path_wxString bpath(wxEmptyString);
    // html
    bpath=GetImageDir();
    check.Add(GetBLId(), _("HTML file"), GetDefaultFilename());

    // css
    if (cssfileid_>0)
    {
        bpath=GetProject()->CSSFiles().Get(cssfileid_)->GetOutputFileURL();
        check.Add(GetBLId(), _("CSS file"), bpath.GetPath(wxEmptyString, false, ccu_Path_wxString::PPF_URL));
    }

    do_checkfile(0, check, GetMask());
}




void ButcherProjectView::GenerateHTML(const wxString &filename)
{



	wxFileName fn(filename), fnfiles(filename);
    wxString dirname;

/*
    wxStringTokenizer pathtk(GetImageDir(true), wxT("/"));
    if (pathtk.HasMoreTokens())
    {
        while (pathtk.HasMoreTokens())
        {
            fnfiles.AppendDir(pathtk.GetNextToken());
            dirname=fnfiles.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);

            if (!wxFileName::DirExists(dirname))
                if (!wxFileName::Mkdir(dirname))
                    throw ButcherException(_("Could not create images dir"));
        }
    } else
        // root path
*/
        dirname=fnfiles.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
    // create directories
    ButcherProjectFilePath::ForcePathCreate(dirname);
    ButcherProjectFilePath::ForcePathCreate(GetImageDir(dirname));

	for (ButcherProjectFilePaths::iterator fpi=GetProject()->FilePaths().begin(); fpi!=GetProject()->FilePaths().end(); fpi++)
	{
		wxString p = fpi->GetFormattedPath(dirname);
		if (!wxFileName::DirExists(p))
			ButcherProjectFilePath::ForcePathCreate(p);
	}

    wxString imgfilename;


    wxFile htmlfile;
	if ((items_&ITEM_HTML)==ITEM_HTML)
	{
		htmlfile.Open(fn.GetFullPath(), wxFile::write);
		if (!htmlfile.IsOpened())
			throw ButcherException(_("Could not create file"));
	}

    // HEAD
    wxString htmlhead(wxEmptyString);
    if (cssfileid_>0)
    {
        htmlhead+=wxString::Format(wxT("<link rel=\"stylesheet\" href=\"%s\" type=\"text/css\" />"),
            GetProject()->CSSFiles().Get(cssfileid_)->GetOutputFileURL().c_str());
        if (!head_.IsEmpty())
            htmlhead+=wxT("\n");

        GetProject()->CSSFiles().Get(cssfileid_)->SaveFileToPath(dirname);
    }
    htmlhead+=head_;


    // BODY tag
	ButcherHTMLAttributeGenerator bodyattr, bodystylesttr(true);
    //wxString bodytag(wxEmptyString), bodystyle(wxEmptyString);
	bodyattr.Add(wxT("leftmargin"), wxT("0"));
	bodyattr.Add(wxT("topmargin"), wxT("0"));
	bodyattr.Add(wxT("rightmargin"), wxT("0"));
	bodyattr.Add(wxT("bottommargin"), wxT("0"));
	bodyattr.Add(wxT("marginwidth"), wxT("0"));
	bodyattr.Add(wxT("marginheight"), wxT("0"));

    if (isbgcolor_)
        //bodystyle+=wxString::Format(wxT("background-color: %s; "), bgcolor_.GetAsString(wxC2S_HTML_SYNTAX).c_str());
		bodystylesttr.Add(wxT("background-color"), bgcolor_.GetAsString(wxC2S_HTML_SYNTAX));
    if (bgimage_.GetLinkType()!=ButcherProjectFileLink::IL_NONE && bgimage_.IsImage())
    {
/*
		bodystyle+=wxString::Format(wxT("background-image: url('%s'); "),
            bgimage_.GetFilename().c_str());
*/
		bodystylesttr.Add(wxT("background-image"), wxString::Format(wxT("url('%s'); "),
			bgimage_.GetFileURL().c_str()));

        switch (bgrepeat_)
        {
        case ButcherProjectAreaConfigBase::BR_HORIZONTAL:
			//bodystyle+=wxT("background-repeat: repeat-x; ");
			bodystylesttr.Add(wxT("background-repeat"), wxT("repeat-x"));
			break;
        case ButcherProjectAreaConfigBase::BR_VERTICAL:
			//bodystyle+=wxT("background-repeat: repeat-y; ");
			bodystylesttr.Add(wxT("background-repeat"), wxT("repeat-y"));
			break;
        case ButcherProjectAreaConfigBase::BR_NONE:
			//bodystyle+=wxT("background-repeat: no-repeat; ");
			bodystylesttr.Add(wxT("background-repeat"), wxT("no-repeat"));
			break;
        default:
			break;
        }
    }

    //if (!bodystyle.IsEmpty())
        //bodytag+=wxString::Format(wxT(" style=\"%s\""), bodystyle.c_str());
	if (!bodystylesttr.IsEmpty())
		bodyattr.Add(wxT("STYLE"), bodystylesttr.Generate(ButcherHTMLAttributeGenerator::KIND_STYLE));
    if (!bodytagadd_.IsEmpty())
        //bodytag+=wxT(" ")+bodytagadd_;
		bodyattr.Add(ButcherHTMLAttributeGenerator(bodytagadd_));

    // HTML generation
    ButcherHTMLGenerator generator;
    generator.SetEncoding(encoding_);

    wxString htit=title_;
    if (htit.IsEmpty())
        htit=name_;
    if (htit.IsEmpty())
        htit=GetBaseFilename();
    generator.AddHead(htmlhead);
    generator.SetTitle(htit);
    //generator.AddBodyTagAdd(bodytag);
	generator.AddBodyTagAdd(bodyattr.Generate(ButcherHTMLAttributeGenerator::KIND_TAG));
    generator.AddBodyHeader(bodyprepend_);
    generator.AddBodyFooter(bodyappend_);

    ButcherProjectBaseAutoProgress upd(this, _("Generating HTML, please wait..."));

    do_genhtml(0, generator, dirname, GetMask(), &upd);

	if ((items_&ITEM_HTML)==ITEM_HTML)
		generator.Save(htmlfile);
}



void ButcherProjectView::do_genhtml(int level, ButcherHTMLGenerator &htmlgenerator, const wxString &dirname,
    ButcherProjectMask *mask, ButcherProjectBaseAutoProgress *progress)
{
    wxString imgdirname=GetImageDir(dirname);
    if (mask->Areas())
    {
        wxString imgfilename;//, imgbasename;
        wxString newline(wxEmptyString), tdnewline;
		ButcherProjectConsts::layouttype_t curlaytype=mask->GetCurrentLayoutType(GetBLId());
		if (curlaytype==ButcherProjectConsts::LTYPE_DEFAULT)
			throw ButcherException(_("Invalid layout type"));


		newline.Append(wxT("\n"));
        newline.Pad(level*10);
        tdnewline=newline;
        wxString tmp;


        // if root mask, use the view table tag
        if (!mask->GetParent())
            tmp=tabletagadd_;
        else
            tmp=mask->GetTableTagAdd();
/*
		if (!tmp.IsEmpty())
            tmp.Prepend(wxT(" "));
*/



		if (curlaytype==ButcherProjectConsts::LTYPE_TABLE)
		{
			ButcherHTMLAttributeGenerator tableattr;
			tableattr.Add(wxT("WIDTH"), (GetIsStretch()&&mask->HaveVariableSizeArea(GetBLId())?wxT("100%"):wxString::Format(wxT("%d"), mask->Areas()->GetClientWidth())));
			tableattr.Add(wxT("CELLSPACING"), wxT("0"));
			tableattr.Add(wxT("CELLPADDING"), wxT("0"));
			tableattr.Add(ButcherHTMLAttributeGenerator(tmp));

			htmlgenerator.AddBody(wxString::Format(wxT("%s%s%s  <tr>"),
				newline.c_str(),
				tableattr.Generate(ButcherHTMLAttributeGenerator::KIND_TAG, wxT("TABLE")).c_str(),
				newline.c_str()));

/*
			htmlgenerator.AddBody(wxString::Format(wxT("%s<TABLE WIDTH=\"%s\" CELLSPACING=\"0\" CELLPADDING=\"0\"%s>%s  <TR>"),
				newline.c_str(),
				(GetIsStretch()&&mask->HaveVariableSizeArea(GetBLId())?wxT("100%"):wxString::Format(wxT("%d"), mask->Areas()->GetClientWidth()).c_str()),
				tmp.c_str(), newline.c_str()));
*/
		}
		htmlgenerator.AddBody(wxString::Format(wxT("%s"), newline.c_str()));

        std::unique_ptr<ButcherProjectMaskLayout> list=std::unique_ptr<ButcherProjectMaskLayout>(mask->CreateLayout(curlaytype==ButcherProjectConsts::LTYPE_TABLE && level==0 && (!isstretch_)));
        ButcherProjectArea *ar=NULL;//, *mar=NULL;
        ButcherProjectAreaConfig *ac=NULL, *mac=NULL;
        wxRect sr;
        //wxString tdadd, tdcnt, imgadd, tdstyle, imagemapname, imagemap, tradd, aptlstyle, aptladd;
		wxString tdcnt, imagemapname, imagemap, tradd;
		ButcherHTMLAttributeGenerator tdattr, tdstyleattr(true), aptlattr, aptlstyleattr(true), imgattr;
        bool haveimage=false, havebg=false, skipcellline, haveimagemap=false, ismaskarea;
        for (unsigned int ct=0; ct<list->Count(); ct++)
        {
            if (progress) progress->SetProgress();

            std::unique_ptr<ButcherImage> img;

            ismaskarea=list->Get(ct)->areaid>0;

            if (ismaskarea)
            {
                ar=mask->Areas()->Areas().Get(list->Get(ct)->areaid);
                ac=ar->Configs().Get(GetBLId());
                sr=ar->GetRect();
                //sr.SetWidth(sr.GetWidth()+1);
                //sr.SetHeight(sr.GetHeight()+1);

                imgfilename=ar->GetImageFilename(GetBLId());

                haveimage=false;
                havebg=ac->GetBackground();
                haveimagemap=ac->GetHaveMap() && ac->GetInnerMask()->AreasMap() &&  !ac->GetInnerMask()->AreasMap()->IsEmpty();

                if (haveimagemap)
                {
                    if (!ac->GetImageMapName().IsEmpty())
                        imagemapname=ac->GetImageMapName();
                    else
                        imagemapname=ar->GetImageFilename(GetBLId(), false)+_("Map");
                }
            }


            skipcellline=true;

            tradd=wxEmptyString;
            tdcnt=wxEmptyString;
/*
			imgadd=wxEmptyString;
            tdadd=wxEmptyString;
            tdstyle=wxEmptyString;
            aptladd=wxEmptyString;
            aptlstyle=wxEmptyString;
*/

			tdattr.Clear();
			tdstyleattr.Clear();
			aptlattr.Clear();
			aptlstyleattr.Clear();
			imgattr.Clear();

            if (list->Get(ct)->colspan>1)
                //tdadd+=wxString::Format(wxT(" COLSPAN=\"%d\""), list->Get(ct)->colspan);
				tdattr.Add(wxT("COLSPAN"), wxString::Format(wxT("%d"), list->Get(ct)->colspan));
            if (list->Get(ct)->rowspan>1)
                //tdadd+=wxString::Format(wxT(" ROWSPAN=\"%d\""), list->Get(ct)->rowspan);
				tdattr.Add(wxT("ROWSPAN"), wxString::Format(wxT("%d"), list->Get(ct)->rowspan));
            if (list->Get(ct)->forcewidth>0)
                //tdadd+=wxString::Format(wxT(" WIDTH=\"%d\""), list->Get(ct)->forcewidth);
				tdattr.Add(wxT("WIDTH"), wxString::Format(wxT("%d"), list->Get(ct)->forcewidth));
            if (list->Get(ct)->forceheight>0)
                //tdadd+=wxString::Format(wxT(" HEIGHT=\"%d\""), list->Get(ct)->forceheight);
				tdattr.Add(wxT("HEIGHT"), wxString::Format(wxT("%d"), list->Get(ct)->forceheight));

            if (ismaskarea)
            {
				//aptladd=wxString::Format(wxT("ID=\"%s\""), ar->GetImageFilename(GetBLId(), false).c_str());
				aptlattr.Add(wxT("ID"), ar->GetImageFilename(GetBLId(), false));
/*
				aptlstyle=wxString::Format(wxT("position: absolute; left: %dpx; top: %dpx; width: %dpx; height: %dpx; z-index: %d;"),
					sr.GetLeft(), sr.GetTop(), sr.GetWidth(), sr.GetHeight(), ct);
*/
				aptlstyleattr.
					MultiAdd(wxT("position"), wxT("absolute")).
					//MultiAdd(wxT("overflow"), wxT("auto")).
					MultiAdd(wxT("left"), wxString::Format(wxT("%dpx"), sr.GetLeft())).
					MultiAdd(wxT("top"), wxString::Format(wxT("%dpx"), sr.GetTop())).
					MultiAdd(wxT("width"), wxString::Format(wxT("%dpx"), sr.GetWidth())).
					MultiAdd(wxT("height"), wxString::Format(wxT("%dpx"), sr.GetHeight())).
					MultiAdd(wxT("z-index"), wxString::Format(wxT("%d"), ct));

				if (!ac->GetVariableSize() && list->Get(ct)->forcewidth<=0)
					tdattr.Add(wxT("WIDTH"), GetIsStretch()&&ac->GetAreaKind()==ButcherProjectAreaConfigBase::AK_MASK&&ac->GetInnerMask()->HaveVariableSizeArea(GetBLId())?wxT("100%"):wxString::Format(wxT("%d"), sr.GetWidth()));
/*
					tdadd+=wxString::Format(wxT(" WIDTH=\"%s\""),
					(GetIsStretch()&&ac->GetAreaKind()==ButcherProjectAreaConfigBase::AK_MASK&&ac->GetInnerMask()->HaveVariableSizeArea(GetBLId())?wxT("100%"):wxString::Format(wxT("%d"), sr.GetWidth()).c_str()));
*/
                if (/*!ac->GetVariableSize() && */list->Get(ct)->forceheight<=0)
                    //tdadd+=wxString::Format(wxT(" HEIGHT=\"%d\""), sr.GetHeight());
					tdattr.Add(wxT("HEIGHT"), wxString::Format(wxT("%d"), sr.GetHeight()));

                switch (ac->GetAlign())
                {
                case ButcherProjectAreaConfigBase::AA_LEFT:
					//tdadd+=wxT(" ALIGN=\"LEFT\"");
					//aptlstyle+=wxT(" text-align: left;");
					tdattr.Add(wxT("ALIGN"), wxT("left"));
					aptlstyleattr.Add(wxT("text-align"), wxT("left"));
					break;
                case ButcherProjectAreaConfigBase::AA_CENTER:
					//tdadd+=wxT(" ALIGN=\"CENTER\"");
					//aptlstyle+=wxT(" text-align: center;");
					tdattr.Add(wxT("ALIGN"), wxT("center"));
					aptlstyleattr.Add(wxT("text-align"), wxT("center"));
					break;
                case ButcherProjectAreaConfigBase::AA_RIGHT:
					//tdadd+=wxT(" ALIGN=\"RIGHT\"");
					//aptlstyle+=wxT(" text-align: right;");
					tdattr.Add(wxT("ALIGN"), wxT("right"));
					aptlstyleattr.Add(wxT("text-align"), wxT("right"));
					break;
                default:
					break;
                }
                switch (ac->GetVAlign())
                {
                case ButcherProjectAreaConfigBase::AV_TOP:
					//tdadd+=wxT(" VALIGN=\"TOP\"");
					//aptlstyle+=wxT(" vertical-align: top;");
					tdattr.Add(wxT("VALIGN"), wxT("top"));
					aptlstyleattr.Add(wxT("vertical-align"), wxT("top"));
					break;
                case ButcherProjectAreaConfigBase::AV_MIDDLE:
					//tdadd+=wxT(" VALIGN=\"MIDDLE\"");
					//aptlstyle+=wxT(" vertical-align: middle;");
					tdattr.Add(wxT("VALIGN"), wxT("middle"));
					aptlstyleattr.Add(wxT("vertical-align"), wxT("middle"));
					break;
                case ButcherProjectAreaConfigBase::AV_BOTTOM:
					//tdadd+=wxT(" VALIGN=\"BOTTOM\"");
					//aptlstyle+=wxT(" vertical-align: bottom;");
					tdattr.Add(wxT("VALIGN"), wxT("bottom"));
					aptlstyleattr.Add(wxT("vertical-align"), wxT("bottom"));
					break;
                default: break;
                }
                switch (ac->GetBGRepeat())
                {
                case ButcherProjectAreaConfigBase::BR_HORIZONTAL:
					//tdstyle+=wxT("background-repeat: repeat-x; ");
					//aptlstyle+=wxT("background-repeat: repeat-x; ");
					tdstyleattr.Add(wxT("background-repeat"), wxT("repeat-x"));
					aptlstyleattr.Add(wxT("background-repeat"), wxT("repeat-x"));
					break;
                case ButcherProjectAreaConfigBase::BR_VERTICAL:
					//tdstyle+=wxT("background-repeat: repeat-y; ");
					//aptlstyle+=wxT("background-repeat: repeat-y; ");
					tdstyleattr.Add(wxT("background-repeat"), wxT("repeat-y"));
					aptlstyleattr.Add(wxT("background-repeat"), wxT("repeat-y"));
					break;
                case ButcherProjectAreaConfigBase::BR_NONE:
					//tdstyle+=wxT("background-repeat: no-repeat; ");
					//aptlstyle+=wxT("background-repeat: no-repeat; ");
					tdstyleattr.Add(wxT("background-repeat"), wxT("no-repeat"));
					aptlstyleattr.Add(wxT("background-repeat"), wxT("no-repeat"));
					break;
                default:
					break;
                }
                if (ac->GetIsBGColor())
				{
                    //tdstyle+=wxString::Format(wxT("background-color: %s; "), ac->GetBGColor().GetAsString(wxC2S_HTML_SYNTAX).c_str());
                    //aptlstyle+=wxString::Format(wxT("background-color: %s; "), ac->GetBGColor().GetAsString(wxC2S_HTML_SYNTAX).c_str());
					tdstyleattr.Add(wxT("background-color"), ac->GetBGColor().GetAsString(wxC2S_HTML_SYNTAX));
					aptlstyleattr.Add(wxT("background-color"), ac->GetBGColor().GetAsString(wxC2S_HTML_SYNTAX));
				}
                if (haveimagemap)
                    //imgadd+=wxString::Format(wxT(" USEMAP=\"%s\""), imagemapname.c_str());
					imgattr.Add(wxT("USEMAP"), imagemapname);

                switch (ac->GetAreaKind())
                {
                case ButcherProjectAreaConfig::AK_IMAGE:
                    if (!havebg)
                    {
                        haveimage=true;
                        skipcellline=false;
                        //havebg=false;
                        if (!ac->GetImageURL().IsEmpty())
                        {
							ButcherHTMLAttributeGenerator aattr;
							aattr.Add(wxT("HREF"), ac->GetImageURL());
                            if (!ac->GetImageURLTagAppend().IsEmpty())
								aattr.Add(ButcherHTMLAttributeGenerator(ac->GetImageURLTagAppend()));

/*
							wxString ut=wxEmptyString;
                            if (!ac->GetImageURLTagAppend().IsEmpty())
                                ut=wxString::Format(wxT(" target=\"%s\""), ac->GetImageURLTagAppend().c_str());

                            tdcnt+=wxString::Format(wxT("<a href=\"%s\"%s>"), ac->GetImageURL().c_str(), ut.c_str());
*/
							tdcnt+=aattr.Generate(ButcherHTMLAttributeGenerator::KIND_TAG, wxT("A"));
                        }

						imgattr.
							MultiAdd(wxT("SRC"), wxString::Format(wxT("%s%s"), GetImageDir(wxEmptyString, true).c_str(), imgfilename.c_str())).
							MultiAdd(wxT("WIDTH"), wxString::Format(wxT("%d"),
							sr.GetWidth())).
							MultiAdd(wxT("HEIGHT"), wxString::Format(wxT("%d"),
							sr.GetHeight())).
							MultiAdd(wxT("BORDER"), wxT("0"));

						if (!ac->GetImageTagAppend().IsEmpty())
							//imgadd+=wxString::Format(wxT(" %s"), ac->GetImageTagAppend().c_str());
							imgattr.Add(ButcherHTMLAttributeGenerator(ac->GetImageTagAppend()));

/*
						tdcnt+=wxString::Format(wxT("<IMG SRC=\"%s%s\" WIDTH=\"%d\" HEIGHT=\"%d\" BORDER=\"0\"%s>"),
                            GetImageDir(wxEmptyString, true).c_str(), imgfilename.c_str(),
							sr.GetWidth(), sr.GetHeight(),
                            imgadd.c_str()); // can't put \n, will break in IE
*/
						tdcnt+=imgattr.Generate(ButcherHTMLAttributeGenerator::KIND_TAG, wxT("IMG"), true); // can't put \n, will break in IE

						if (!ac->GetImageURL().IsEmpty())
                        {
                            tdcnt+=wxString::Format(wxT("</a>"));
                        }
                    }
                    break;
                default:
                    break;
                }

                // BACKGROUND
                if (havebg)
                {
                    haveimage=true;
/*
					tdstyle+=wxString::Format(wxT("background-image:url('%s%s'); "),
                        GetImageDir(wxEmptyString, true).c_str(), imgfilename.c_str());
                    aptlstyle+=wxString::Format(wxT("background-image:url('%s%s'); "),
                        GetImageDir(wxEmptyString, true).c_str(), imgfilename.c_str());
*/
					tdstyleattr.Add(wxT("background-image"),
						wxString::Format(wxT("url('%s%s')"), GetImageDir(wxEmptyString, true).c_str(), imgfilename.c_str()));
					aptlstyleattr.Add(wxT("background-image"),
						wxString::Format(wxT("url('%s%s')"), GetImageDir(wxEmptyString, true).c_str(), imgfilename.c_str()));
                }

                // DATA
                if (!ac->GetCellTagAppend().IsEmpty())
                {
                    //tdadd+=wxT(" ");
                    //tdadd+=ac->GetCellTagAppend();
					tdattr.Add(ButcherHTMLAttributeGenerator(ac->GetCellTagAppend()));
					aptlattr.Add(ButcherHTMLAttributeGenerator(ac->GetCellTagAppend()));
                }
                tdcnt+=ac->GetContent();
            }
			else if (curlaytype==ButcherProjectConsts::LTYPE_TABLE)
            {
                // !ismaskarea
                wxString trfmt(wxT("  "));
				trfmt+=_("<!-- alignment row -->");
				trfmt+=wxT("%s");
				tradd=tradd+wxString::Format(trfmt, newline.c_str());
                tdnewline=wxEmptyString;
            }

            // ROW BEGIN
            if (curlaytype==ButcherProjectConsts::LTYPE_TABLE && list->Get(ct)->skipline)
                htmlgenerator.AddBody(wxString::Format(wxT("  </tr>%s%s  <tr>%s"),
                    newline.c_str(), tradd.c_str(), tdnewline.c_str()));

            // CELL BEGIN
/*
			if (!tdstyle.IsEmpty())
                tdstyle=wxT(" STYLE=\"")+tdstyle+wxT("\"");
*/
			if (!tdstyleattr.IsEmpty())
			{
				if (tdattr.Exists(wxT("STYLE")))
					tdstyleattr.Add(ButcherHTMLAttributeGenerator(tdattr.Get(wxT("STYLE")), ButcherHTMLAttributeGenerator::KIND_STYLE));
				tdattr.Add(wxT("STYLE"), tdstyleattr.Generate(ButcherHTMLAttributeGenerator::KIND_STYLE));
			}
			if (!aptlstyleattr.IsEmpty())
			{
				if (aptlattr.Exists(wxT("STYLE")))
					aptlstyleattr.Add(ButcherHTMLAttributeGenerator(aptlattr.Get(wxT("STYLE")), ButcherHTMLAttributeGenerator::KIND_STYLE));
				aptlattr.Add(wxT("STYLE"), aptlstyleattr.Generate(ButcherHTMLAttributeGenerator::KIND_STYLE));
			}

			switch (curlaytype)
			{
			case ButcherProjectConsts::LTYPE_TABLE:
				//htmlgenerator.AddBody(wxString::Format(wxT("    <TD %s%s>%s"),
				htmlgenerator.AddBody(wxString::Format(wxT("    %s%s"),
					tdattr.Generate(ButcherHTMLAttributeGenerator::KIND_TAG, wxT("TD")).c_str(),
					//tdadd.c_str(), tdstyle.c_str(), tdnewline.c_str()));
					tdnewline.c_str()));
				break;
			case ButcherProjectConsts::LTYPE_APTABLELESS:
				if (styleseparated_ && aptlattr.Exists(wxT("STYLE")))
				{
					htmlgenerator.AddStyles(wxString::Format(wxT("#%s { %s }\n"),
						aptlattr.Get(wxT("ID")).c_str(), aptlattr.Get(wxT("STYLE")).c_str()));
					aptlattr.Remove(wxT("STYLE"));
				}

				//htmlgenerator.AddBody(wxString::Format(wxT("    <DIV %s STYLE=\"%s\">%s"),
				htmlgenerator.AddBody(wxString::Format(wxT("    %s%s"),
					aptlattr.Generate(ButcherHTMLAttributeGenerator::KIND_TAG, wxT("DIV")).c_str(),
					//aptladd.c_str(), aptlstyle.c_str(), tdnewline.c_str()));
					tdnewline.c_str()));
				break;
            default:
                break;
			}

            // CELL BODY BEGIN
            if (ismaskarea)
            {
                if (ac->GetInnerScrollable() && sr.GetWidth()>20 && sr.GetHeight()>20) // 20 is scrollbar width/height
                {
                    // put inside scrollable div
                    htmlgenerator.AddBody(wxT("      "));
                    htmlgenerator.AddBody(wxString::Format(
                        wxT("<div style=\"width: %dpx; height:%dpx; overflow: auto;\">%s"),
                        sr.GetWidth()-20, sr.GetHeight()-20, tdnewline.c_str()));
                }
            }

            // CELL BODY
            if (!tdcnt.IsEmpty())
            {
                htmlgenerator.AddBody(wxT("      "));
                htmlgenerator.AddBody(tdcnt);
            }

            if (ismaskarea)
            {
                // CELL BODY - INNER MASK
                if (ar->Configs().Get(GetBLId())->GetAreaKind()==ButcherProjectAreaConfig::AK_MASK)
                {
                    htmlgenerator.AddBody(wxT("\n"));
                    do_genhtml(level+1, htmlgenerator, dirname, ar->Configs().Get(GetBLId())->GetInnerMask(), progress);
                    htmlgenerator.AddBody(tdnewline);
                }
            }

            // CELL BODY END
            if (ismaskarea)
            {
                if (ac->GetInnerScrollable())
                {
                    // put inside scrollable div
                    htmlgenerator.AddBody(wxT("      "));
                    htmlgenerator.AddBody(wxString::Format(
                        wxT("</div>%s"),
                        tdnewline.c_str()));
                }
            }

            // CELL END
			if (skipcellline)
				htmlgenerator.AddBody(wxString::Format(wxT("%s    "), tdnewline.c_str()));

			switch (curlaytype)
			{
			case ButcherProjectConsts::LTYPE_TABLE:
				htmlgenerator.AddBody(wxString::Format(wxT("</td>%s"), tdnewline.c_str()));
				break;
			case ButcherProjectConsts::LTYPE_APTABLELESS:
				htmlgenerator.AddBody(wxString::Format(wxT("</div>%s"), tdnewline.c_str()));
				break;
            default:
                break;
			}

            if (ismaskarea && haveimage)
            {
				if ((items_&ITEM_IMAGES)==ITEM_IMAGES)
				{
					img=std::unique_ptr<ButcherImage>(CreateAreaImage(ar));
					if (img.get())
						img->Save(ac->ImageInfo().GetSaveParams(), imgdirname+imgfilename);
				}

                // IMAGEMAP
                if (haveimagemap)
                {
                    imagemap=wxString::Format(wxT("<map name=\"%s\">\n"), imagemapname.c_str());

					for (ButcherProjectAreas::iterator i=ac->GetInnerMask()->AreasMap()->Areas().begin();
						i!=ac->GetInnerMask()->AreasMap()->Areas().end(); i++)
					{
                        mac=i->Configs().Get(GetBLId());

						ButcherHTMLAttributeGenerator aattr;
						aattr.Add(wxT("SHAPE"), wxT("rect"));
						aattr.Add(wxT("COORDS"), wxString::Format(wxT(" coords=\"%d,%d,%d,%d\""),
                            i->GetLeft()->GetPosition(),
                            i->GetTop()->GetPosition(),
                            i->GetRight()->GetPosition()-1,
                            i->GetBottom()->GetPosition()-1));

                        if (!mac->GetImageURL().IsEmpty())
							aattr.Add(wxT("HREF"), mac->GetImageURL());
						if (!mac->GetImageURLTagAppend().IsEmpty())
							aattr.Add(ButcherHTMLAttributeGenerator(mac->GetImageURLTagAppend()));
                        if (!mac->GetImageTagAppend().IsEmpty())
							aattr.Add(wxT("ALT"), mac->GetImageTagAppend());

						imagemap+=aattr.Generate(ButcherHTMLAttributeGenerator::KIND_TAG, wxT("AREA"), true);
                        imagemap+=wxT("\n");

/*
						tmp=wxString::Format(wxT(" coords=\"%d,%d,%d,%d\""),
                            i->GetLeft()->GetPosition(),
                            i->GetTop()->GetPosition(),
                            i->GetRight()->GetPosition()-1,
                            i->GetBottom()->GetPosition()-1);
                        if (!mac->GetImageURL().IsEmpty())
                            tmp+=wxString::Format(wxT(" href=\"%s\""), mac->GetImageURL().c_str());
                        if (!mac->GetImageURLTagAppend().IsEmpty())
                            tmp+=wxString::Format(wxT(" target=\"%s\""), mac->GetImageURLTagAppend().c_str());
                        if (!mac->GetImageTagAppend().IsEmpty())
                            tmp+=wxString::Format(wxT(" alt=\"%s\""), mac->GetImageTagAppend().c_str());

                        imagemap+=wxString::Format(wxT("<area share=\"rect\"%s>\n"), tmp.c_str());
*/
					}
                    imagemap+=wxT("</map>\n");

                    htmlgenerator.AddBodyFooter(imagemap);
                }
            }

            if (ismaskarea && ((items_&ITEM_HTML)==ITEM_HTML) )
            {
                // alternate files
                for (int alti=0; alti<BUTCHERCONST_VIEW_MAXALTERNATE; alti++)
                {
                    if (ac->AlternateImageInfo(alti).GetEnabled())
                    {
                        // save image
                        imgfilename=ar->GetImageFilename(GetBLId(), true, alti);
                        //img=std::unique_ptr<ButcherImage>(GetFileCurrent(alti)->GetSubImage(ar->GetGlobalRect()));
                        img=std::unique_ptr<ButcherImage>(CreateAreaImage(ar, alti));
                        if (img.get())
                            img->Save(ac->AlternateImageInfo(alti).GetSaveParams(), imgdirname+imgfilename);
                    }
                }

                // assorted files
                if (assortedfilegroups_.Count()>0)
                    GetProject()->AssortedFiles().SaveFiles(dirname, assortedfilegroups_);
            }

            tdnewline=newline;
        }

		if (curlaytype==ButcherProjectConsts::LTYPE_TABLE)
			htmlgenerator.AddBody(wxString::Format(wxT("  </tr>%s</table>\n"), newline.c_str()));
    }

    // GLOBAL AREAS
	if ((items_&ITEM_IMAGESGLOBAL)==ITEM_IMAGESGLOBAL)
		GenerateImages(imgdirname, mask, ButcherProjectArea::AS_AREAGLOBAL);
}



void ButcherProjectView::GenerateImages(const wxString &dirname, ButcherProjectMask* mask,
    ButcherProjectArea::areaselect_t areas, ButcherProjectArea::areaselect_t areasinner)
{
    //ButcherProjectArea *ar;
    ButcherProjectAreaConfig *ac;
    wxString imgfilename;
    //ButcherListIdList ids;
    std::unique_ptr<ButcherImage> img;

    ButcherProjectFilePath::ForcePathCreate(dirname);

    for (ButcherProjectMask::MaskContainers_t::iterator c=mask->containers_.begin(); c!=mask->containers_.end(); c++)
    {
        // iterate over areas
		for (ButcherProjectAreas::iterator i=c->second->Areas().begin(); i!=c->second->Areas().end(); i++)
		{
            //ar=c->second->Areas().Get(*i);
            ac=i->Configs().Get(GetBLId());
            if (c->second->GetContainedAreaClass()!=ButcherProjectArea::AC_MAP && c->second->IsAreaSelect(areas))
            {
                if (ac->GetAreaKind()==ButcherProjectAreaConfig::AK_IMAGE)
                {
                    // save image
                    imgfilename=i->GetImageFilename(GetBLId());

                    //img=std::unique_ptr<ButcherImage>(GetFileCurrent(ac->GetAlternateFile())->GetSubImage(ar->GetGlobalRect()));
                    img=std::unique_ptr<ButcherImage>(CreateAreaImage(&*i));

                    if (img.get())
                        img->Save(ac->ImageInfo().GetSaveParams(), dirname+imgfilename);
                }

                // alternate files
                for (int alti=0; alti<BUTCHERCONST_VIEW_MAXALTERNATE; alti++)
                {
                    if (ac->AlternateImageInfo(alti).GetEnabled())
                    {
                        // save image
                        imgfilename=i->GetImageFilename(GetBLId(), true, alti);

                        //img=std::unique_ptr<ButcherImage>(GetFileCurrent(alti)->GetSubImage(ar->GetGlobalRect()));
                        img=std::unique_ptr<ButcherImage>(CreateAreaImage(&*i, alti));
                        if (img.get())
                            img->Save(ac->AlternateImageInfo(alti).GetSaveParams(), dirname+imgfilename);

                    }
                }
            }

            // process inner masks
            if (c->second->IsAreaSelect(areas) && ac->GetInnerMask()->HaveArea())
                GenerateImages(dirname, ac->GetInnerMask(), areasinner, areasinner); // pass inner flag
		}
    }
}



void ButcherProjectView::do_checkfile(int level, ButcherProjectViewFileCheck &check, ButcherProjectMask *mask)
{
    ccu_Path_wxString imgpath(GetImageDir());
    ccu_Path_wxString bpath(wxEmptyString);

    if (mask->Areas())
    {
        wxString imgfilename;//, imgbasename;

        std::unique_ptr<ButcherProjectMaskLayout> list=std::unique_ptr<ButcherProjectMaskLayout>(mask->CreateLayout(level==0 && (!isstretch_)));
        ButcherProjectArea *ar=NULL;
        ButcherProjectAreaConfig *ac=NULL;
        bool haveimage=false, havebg=false, ismaskarea;
        for (unsigned int ct=0; ct<list->Count(); ct++)
        {
            ismaskarea=list->Get(ct)->areaid>0;

            if (ismaskarea)
            {
                ar=mask->Areas()->Areas().Get(list->Get(ct)->areaid);
                ac=ar->Configs().Get(GetBLId());
                imgfilename=ar->GetImageFilename(GetBLId());

                haveimage=false;
                havebg=ac->GetBackground();
            }

            if (ismaskarea)
            {
                switch (ac->GetAreaKind())
                {
                case ButcherProjectAreaConfig::AK_IMAGE:
                    if (!havebg)
                    {
                        haveimage=true;
                    }
                    break;
                default:
                    break;
                }

                // BACKGROUND
                if (havebg)
                {
                    haveimage=true;
                }

            }

            if (ismaskarea)
            {
                // CELL BODY - INNER MASK
                if (ar->Configs().Get(GetBLId())->GetAreaKind()==ButcherProjectAreaConfig::AK_MASK)
                {
                    do_checkfile(level, check, ar->Configs().Get(GetBLId())->GetInnerMask());
                }
            }

            if (ismaskarea && haveimage)
            {
                check.Add(GetBLId(), wxString::Format(_("Area: %s"), ar->GetDescription().c_str()),
                    imgpath.GetPath(wxEmptyString, true, ccu_Path_wxString::PPF_URL)+imgfilename);
            }

            if (ismaskarea)
            {
                // alternate files
                for (int alti=0; alti<BUTCHERCONST_VIEW_MAXALTERNATE; alti++)
                {
                    if (ac->AlternateImageInfo(alti).GetEnabled())
                    {
                        // save image
                        imgfilename=ar->GetImageFilename(GetBLId(), true, alti);

                        check.Add(GetBLId(), wxString::Format(_("Area: %s alternate %d"), ar->GetDescription().c_str(), alti+1),
                            imgpath.GetPath(wxEmptyString, true, ccu_Path_wxString::PPF_URL)+imgfilename);
                    }
                }

                // assorted files
                if (assortedfilegroups_.Count()>0)
                {
					for (ButcherListIdList::const_iterator i=assortedfilegroups_.Get().begin();
                        i!=assortedfilegroups_.Get().end(); i++)
                    {
                        bpath=GetProject()->AssortedFiles().Get(*i)->GetOutputFileURL();
                        check.Add(GetBLId(), wxString::Format(_("Assorted file: %d - %s"), *i,
                            GetProject()->AssortedFiles().Get(*i)->GetPathFile().c_str()),
                            bpath.GetPath(wxEmptyString, true, ccu_Path_wxString::PPF_URL));
                    }
                }
            }
        }
    }

    // GLOBAL AREAS
    do_checkimages(check, mask, ButcherProjectArea::AS_AREAGLOBAL);
}



void ButcherProjectView::do_checkimages(ButcherProjectViewFileCheck &check, ButcherProjectMask* mask,
    ButcherProjectArea::areaselect_t areas, ButcherProjectArea::areaselect_t areasinner)
{
    ccu_Path_wxString imgpath(GetImageDir());
    ccu_Path_wxString bpath(wxEmptyString);

    //ButcherProjectArea *ar;
    ButcherProjectAreaConfig *ac;
    wxString imgfilename;
    //ButcherListIdList ids;

    for (ButcherProjectMask::MaskContainers_t::iterator c=mask->containers_.begin(); c!=mask->containers_.end(); c++)
    {
        // iterate over areas

		for (ButcherProjectAreas::iterator i=c->second->Areas().begin(); i!=c->second->Areas().end(); i++)
		{
			//ar=c->second->Areas().Get(*i);
			ac=i->Configs().Get(GetBLId());
			if (c->second->GetContainedAreaClass()!=ButcherProjectArea::AC_MAP && c->second->IsAreaSelect(areas))
			{
				if (ac->GetAreaKind()==ButcherProjectAreaConfig::AK_IMAGE)
				{
					// save image
					imgfilename=i->GetImageFilename(GetBLId());
					check.Add(GetBLId(), wxString::Format(_("Global Area: %s"), i->GetDescription().c_str()),
						imgpath.GetPath(wxEmptyString, true, ccu_Path_wxString::PPF_URL)+imgfilename);
				}

				// alternate files
				for (int alti=0; alti<BUTCHERCONST_VIEW_MAXALTERNATE; alti++)
				{
					if (ac->AlternateImageInfo(alti).GetEnabled())
					{
						// save image
						imgfilename=i->GetImageFilename(GetBLId(), true, alti);

						check.Add(GetBLId(), wxString::Format(_("Global Area: %s alternate %d"), i->GetDescription().c_str(), alti),
							imgpath.GetPath(wxEmptyString, true, ccu_Path_wxString::PPF_URL)+imgfilename);
					}
				}
			}

			// process inner masks
			if (c->second->IsAreaSelect(areas) && ac->GetInnerMask()->HaveArea())
				do_checkimages(check, ac->GetInnerMask(), areasinner, areasinner); // pass inner flag
		}
    }
}



void ButcherProjectView::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseBLId::ProjectEvent(event);
    bgimage_.ProjectEvent(event);

    switch (event.GetProjectEvent().GetEvent())
    {
/*
	case ButcherProjectEvent::BPE_MASKCONFIGDELETED:
        // mask config
        if (event.GetProjectEvent().GetEId()==maskid_ &&
            (event.GetProjectEvent().GetEId2()==0||event.GetProjectEvent().GetEId2()==maskconfigid_))
        {
            event.AddChange();
            if (event.GetIsExecute())
            {
                maskconfigid_=1;
                ProjectModified();
            }
        }
        break;
*/
    case ButcherProjectEvent::BPE_FILEDELETED:
        {
            // file alternate
            for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
            {
                if (filealternateid_[i]==event.GetProjectEvent().GetEId())
                {
                    event.AddChange();
                    if (event.GetIsExecute())
                    {
                        filealternateid_[i]=0;
                        ProjectModified();
                    }
                }
            }
        }
        break;
    case ButcherProjectEvent::BPE_FILEPATHDELETED:
        // images file path
        if (event.GetProjectEvent().GetEId()==0||event.GetProjectEvent().GetEId()==imagefilepathid_)
        {
            event.AddChange();
            if (event.GetIsExecute())
            {
                imagefilepathid_=0; // no default for file paths
                ProjectModified();
            }
        }
        break;
    case ButcherProjectEvent::BPE_CSSFILEDELETED:
        // CSS file
        if (event.GetProjectEvent().GetEId()==0||event.GetProjectEvent().GetEId()==cssfileid_)
        {
            event.AddChange();
            if (event.GetIsExecute())
            {
                cssfileid_=0; // no default for css files
                ProjectModified();
            }
        }
        break;
    case ButcherProjectEvent::BPE_ASSORTEDFILEGROUPDELETED:
        // assorted file groups
        if (assortedfilegroups_.Count()>0 &&
            (event.GetProjectEvent().GetEId()==0||assortedfilegroups_.Exists(event.GetProjectEvent().GetEId())))
        {
            event.AddChange();
            if (event.GetIsExecute())
            {
                if (event.GetProjectEvent().GetEId()==0) assortedfilegroups_.Clear();
                else assortedfilegroups_.Remove(event.GetProjectEvent().GetEId());
                ProjectModified();
            }
        }
        break;
    default:
        break;
    }
}



void ButcherProjectView::LoadMetadata(ButcherMetadataData *metadata)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING)) ||
        (!metadata->Exists(BFILE_MDI_FILE, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_MASK, ButcherMetadataDataItem::DT_INTEGER)) ||
        //(!metadata->Exists(BFILE_MDI_MASKCONFIG, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_ITEMS, ButcherMetadataDataItem::DT_INTEGER)))
        throw ButcherException(_("Invalid metadata for view"));

    SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    name_=metadata->Get(BFILE_MDI_NAME).GetString();
	if (metadata->Exists(BFILE_MDI_LAYOUTTYPE, ButcherMetadataDataItem::DT_INTEGER))
		layouttype_=static_cast<ButcherProjectConsts::layouttype_t>(metadata->Get(BFILE_MDI_LAYOUTTYPE).GetInteger());
    fileid_=metadata->Get(BFILE_MDI_FILE).GetInteger();
    maskid_=metadata->Get(BFILE_MDI_MASK).GetInteger();
	if (metadata->Exists(BFILE_MDI_MASKCONFIG, ButcherMetadataDataItem::DT_INTEGER))
		COMPATmaskconfigid_=metadata->Get(BFILE_MDI_MASKCONFIG).GetInteger();
    items_=metadata->Get(BFILE_MDI_ITEMS).GetInteger();
    if (metadata->Exists(BFILE_MDI_FILEALTERNATE, ButcherMetadataDataItem::DT_METADATA) &&
        metadata->Get(BFILE_MDI_FILEALTERNATE).GetMetadata().Exists(BFILE_MD_FILEALTERNATE))
    {
        ButcherMetadataData *metadatafa;
        for (unsigned int i=0; i<metadata->Get(BFILE_MDI_FILEALTERNATE).GetMetadata().Get(BFILE_MD_FILEALTERNATE).Count(); i++)
        {
            metadatafa=&metadata->Get(BFILE_MDI_FILEALTERNATE).GetMetadata().Get(BFILE_MD_FILEALTERNATE).Get(i);
            if (metadatafa->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER) &&
                metadatafa->Exists(BFILE_MDI_FILEALTERNATE, ButcherMetadataDataItem::DT_INTEGER) &&
                metadatafa->Get(BFILE_MDI_ID).GetInteger() >= 0 &&
                metadatafa->Get(BFILE_MDI_ID).GetInteger() < BUTCHERCONST_VIEW_MAXALTERNATE)
                filealternateid_[metadatafa->Get(BFILE_MDI_ID).GetInteger()]=metadatafa->Get(BFILE_MDI_FILEALTERNATE).GetInteger();
        }
    }

    if (metadata->Exists(BFILE_MDI_IMAGEDIR, ButcherMetadataDataItem::DT_INTEGER))
        imagefilepathid_=metadata->Get(BFILE_MDI_IMAGEDIR).GetInteger();
    if (metadata->Exists(BFILE_MDI_CSS, ButcherMetadataDataItem::DT_INTEGER))
        cssfileid_=metadata->Get(BFILE_MDI_CSS).GetInteger();
    if (metadata->Exists(BFILE_MDI_FILENAME, ButcherMetadataDataItem::DT_STRING))
        filename_=metadata->Get(BFILE_MDI_FILENAME).GetString();
    if (metadata->Exists(BFILE_MDI_BODYTAGADD, ButcherMetadataDataItem::DT_STRING))
        bodytagadd_=metadata->Get(BFILE_MDI_BODYTAGADD).GetString();
    if (metadata->Exists(BFILE_MDI_TABLETAGADD, ButcherMetadataDataItem::DT_STRING))
        tabletagadd_=metadata->Get(BFILE_MDI_TABLETAGADD).GetString();
    if (metadata->Exists(BFILE_MDI_TITLE, ButcherMetadataDataItem::DT_STRING))
        title_=metadata->Get(BFILE_MDI_TITLE).GetString();
    if (metadata->Exists(BFILE_MDI_HEAD, ButcherMetadataDataItem::DT_STRING))
        head_=metadata->Get(BFILE_MDI_HEAD).GetString();
    if (metadata->Exists(BFILE_MDI_BODYPREPEND, ButcherMetadataDataItem::DT_STRING))
        bodyprepend_=metadata->Get(BFILE_MDI_BODYPREPEND).GetString();
    if (metadata->Exists(BFILE_MDI_BODYAPPEND, ButcherMetadataDataItem::DT_STRING))
        bodyappend_=metadata->Get(BFILE_MDI_BODYAPPEND).GetString();
    if (metadata->Exists(BFILE_MDI_ENCODING, ButcherMetadataDataItem::DT_INTEGER))
        encoding_=static_cast<ButcherHTMLConsts::encoding_t>(metadata->Get(BFILE_MDI_ENCODING).GetInteger());
    if (metadata->Exists(BFILE_MDI_BGIMAGE, ButcherMetadataDataItem::DT_STRING))
        bgimage_.SetBURL(metadata->Get(BFILE_MDI_BGIMAGE).GetString());
    if (metadata->Exists(BFILE_MDI_BGREPEAT, ButcherMetadataDataItem::DT_INTEGER))
        bgrepeat_=static_cast<ButcherProjectAreaConfigBase::bgrepeat_t>(metadata->Get(BFILE_MDI_BGREPEAT).GetInteger());
    if (metadata->Exists(BFILE_MDI_ISBGCOLOR, ButcherMetadataDataItem::DT_INTEGER))
        isbgcolor_=metadata->Get(BFILE_MDI_ISBGCOLOR).GetBool();
    if (metadata->Exists(BFILE_MDI_ISSTRETCH, ButcherMetadataDataItem::DT_INTEGER))
        isstretch_=metadata->Get(BFILE_MDI_ISSTRETCH).GetBool();
	if (metadata->Exists(BFILE_MDI_STYLESEPARATED, ButcherMetadataDataItem::DT_INTEGER))
		styleseparated_=metadata->Get(BFILE_MDI_STYLESEPARATED).GetBool();
    if (metadata->Exists(BFILE_MDI_BGCOLOR, ButcherMetadataDataItem::DT_INTEGER))
        bgcolor_.Set(metadata->Get(BFILE_MDI_BGCOLOR).GetInteger());

    if (metadata->Exists(BFILE_MDI_ASSORTEDFILEGROUP, ButcherMetadataDataItem::DT_METADATA))
        if (metadata->Get(BFILE_MDI_ASSORTEDFILEGROUP).GetMetadata().Exists(BFILE_MD_ASSORTEDFILEGROUP))
        {
            ButcherMetadataData *gmetadata;

            for (unsigned int i=0; i<metadata->Get(BFILE_MDI_ASSORTEDFILEGROUP).GetMetadata().Get(BFILE_MD_ASSORTEDFILEGROUP).Count(); i++)
            {
                gmetadata=&metadata->Get(BFILE_MDI_ASSORTEDFILEGROUP).GetMetadata().Get(BFILE_MD_ASSORTEDFILEGROUP).Get(i);

                if ((!gmetadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)))
                    throw ButcherException(_("Invalid metadata for view assorted file"));

                assortedfilegroups_.Add(gmetadata->Get(BFILE_MDI_ID).GetInteger());
            }
        }

}



void ButcherProjectView::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_))->
		MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_LAYOUTTYPE, layouttype_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_FILE, fileid_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MASK, maskid_))->
        //MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_MASKCONFIG, maskconfigid_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ITEMS, items_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ENCODING, encoding_))->
        MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_ASSORTEDFILEGROUP))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_BGREPEAT, bgrepeat_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ISBGCOLOR, isbgcolor_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ISSTRETCH, isstretch_))->
		MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_STYLESEPARATED, styleseparated_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_BGCOLOR, BFILE_RGB(bgcolor_.Red(), bgcolor_.Green(), bgcolor_.Blue())));

    metadata->MultiAdd(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_FILEALTERNATE));
    ButcherMetadataData *metadatafa;

    for (int i=0; i<BUTCHERCONST_VIEW_MAXALTERNATE; i++)
    {
        if (filealternateid_[i]>0)
        {
            metadatafa=metadata->Get(BFILE_MDI_FILEALTERNATE).GetMetadata().Get(BFILE_MD_FILEALTERNATE, true).Add();
            metadatafa->
                MultiAdd(new ButcherMetadataDataItem_Integer(metadatafa, BFILE_MDI_ID, i))->
                MultiAdd(new ButcherMetadataDataItem_Integer(metadatafa, BFILE_MDI_FILEALTERNATE, filealternateid_[i]));
        }
    }

    if (imagefilepathid_>0)
        metadata->MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_IMAGEDIR, imagefilepathid_));
    if (cssfileid_>0)
        metadata->MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_CSS, cssfileid_));
    if (!filename_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_FILENAME, filename_));
    if (!bodytagadd_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_BODYTAGADD, bodytagadd_));
    if (!tabletagadd_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_TABLETAGADD, tabletagadd_));
    if (!title_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_TITLE, title_));
    if (!head_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_HEAD, head_));
    if (!bodyprepend_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_BODYPREPEND, bodyprepend_));
    if (!bodyappend_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_BODYAPPEND, bodyappend_));
    if (bgimage_.GetLinkType()!=ButcherProjectFileLink::IL_NONE)
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_BGIMAGE, bgimage_.GetBURL()));

    for (ButcherListIdList::const_iterator i=assortedfilegroups_.Get().begin(); i!=assortedfilegroups_.Get().end(); i++)
    {
        ButcherMetadataData *gmetadata=metadata->Get(BFILE_MDI_ASSORTEDFILEGROUP).GetMetadata().Get(BFILE_MD_ASSORTEDFILEGROUP, true).Add();
        gmetadata->
            MultiAdd(new ButcherMetadataDataItem_Integer(gmetadata, BFILE_MDI_ID, *i));
    }
}


