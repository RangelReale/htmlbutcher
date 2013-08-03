//=============================================================================
/**
 *  @file   ButcherProjectMask.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMask.h"
#include "ButcherProjectBaseAutoDisable.h"
#include "ButcherProjectBaseAutoUpdate.h"
#include "ButcherProjectView.h"
#include "ButcherProjectMasks.h"
#include "ButcherProjectMaskDrawSelection.h"
#include "ButcherProjectMaskRoot.h"
#include "ButcherProjectTemplateParser.h"
#include "ButcherProject.h"


#include <wx/filename.h>
#include <wx/dcmemory.h>
#include <wx/html/htmprint.h>



/////////////////////////////////
// CLASS
//      ButcherProjectMask
/////////////////////////////////
ButcherProjectMask::ButcherProjectMask(ButcherProject *project, ButcherProjectArea *parent,
    const wxString &name, int width, int height) :
    ButcherProjectBaseBLId(project), parent_(parent), name_(name),
    width_(width), height_(height),
    tabletagadd_(wxEmptyString),
    containers_()
{
    ButcherProjectBaseAutoDisable autodis(this);

/*
    if (!parent_)
        configs_=new ButcherProjectMaskConfigs(this);

    if (configs_)
        configs_->Add(wxT("DEFAULT"), 1);
*/
}




ButcherProjectMask::ButcherProjectMask(ButcherProject *project) :
    ButcherProjectBaseBLId(project), parent_(NULL), name_(wxT("__TEMP__")),
    width_(0), height_(0),
    tabletagadd_(wxEmptyString),
    containers_()
{

}



ButcherProjectMask::~ButcherProjectMask()
{
/*
	if (!parent_)
        delete configs_;
*/
}



ButcherProjectConsts::layouttype_t ButcherProjectMask::GetCurrentLayoutType(BLID_t viewid)
{
	if (GetParent() && GetParent()->Configs().Get(viewid)->GetLayoutType()!=ButcherProjectConsts::LTYPE_DEFAULT)
		return GetParent()->Configs().Get(viewid)->GetLayoutType();
	if (viewid>0 && GetProject()->Views().Get(viewid)->GetLayoutType()!=ButcherProjectConsts::LTYPE_DEFAULT)
		return GetProject()->Views().Get(viewid)->GetLayoutType();
	if (GetProject()->ProjectOptions().GetLayoutType()!=ButcherProjectConsts::LTYPE_DEFAULT)
		return GetProject()->ProjectOptions().GetLayoutType();
	return ButcherProjectConsts::DEFAULT_LAYOUTTYPE;
}

int ButcherProjectMask::GetWidth()
{
    if (!parent_) return width_;
    return parent_->GetWidth();
}




int ButcherProjectMask::GetHeight()
{
    if (!parent_) return height_;
    return parent_->GetHeight();
}



int ButcherProjectMask::GetSize(ButcherProjectLine::orientation_t orientation)
{
    if (orientation == ButcherProjectLine::BPL_HORIZONTAL)
        return GetHeight();
    return GetWidth();
}



long ButcherProjectMask::GetDrawStartX()
{
    if (parent_)
        return parent_->GetLeft()->GetPosition()+parent_->GetMask()->GetDrawStartX();
    return 0;
    //return line_left_->GetPosition();
}




long ButcherProjectMask::GetDrawStartY()
{
    if (parent_)
        return parent_->GetTop()->GetPosition()+parent_->GetMask()->GetDrawStartY();
    return 0;
    //return line_top_->GetPosition();
}




long ButcherProjectMask::GetDrawStart(ButcherProjectLine::orientation_t orientation)
{
    if (orientation == ButcherProjectLine::BPL_HORIZONTAL)
        return GetDrawStartY();
    return GetDrawStartX();
}




long ButcherProjectMask::GlobalToMask(ButcherProjectLine::orientation_t orientation, long position)
{
    long ret=position-GetDrawStart(orientation);
    if (ret<0 || ret>=GetSize(orientation)) return -1;
    return ret;
}




long ButcherProjectMask::MaskToGlobal(ButcherProjectLine::orientation_t orientation, long position)
{
    return GetDrawStart(orientation)+position;
}



wxPoint ButcherProjectMask::GlobalToMask(const wxPoint &position)
{
    return wxPoint(GlobalToMask(ButcherProjectLine::BPL_VERTICAL, position.x),
        GlobalToMask(ButcherProjectLine::BPL_HORIZONTAL, position.y));
}




wxPoint ButcherProjectMask::MaskToGlobal(const wxPoint &position)
{
    return wxPoint(MaskToGlobal(ButcherProjectLine::BPL_VERTICAL, position.x),
        MaskToGlobal(ButcherProjectLine::BPL_HORIZONTAL, position.y));
}




bool ButcherProjectMask::CheckSet(int width, int height)
{
    if (!Areas()) return true;

	//if (!Areas()->GetLineLeft()->CanMove(marginleft)) return false;
    //if (!GetLineTop()->op_canmove(margintop)) return false;
    if (width<width_ && !Areas()->GetLineRight()->CanMove(width-1)) return false;
    if (height<height_ && !Areas()->GetLineBottom()->CanMove(height-1)) return false;

	return true;
}




void ButcherProjectMask::SetContainer(ButcherProjectArea::areaclass_t areaclass,
    ButcherProjectMaskContainer *container)
{
    if (container)
        containers_[areaclass]=container;
    else
        containers_.erase(areaclass);
    ProjectModified();
}



void ButcherProjectMask::Set(const wxString &name,
    int width, int height)
{
    if (!CheckSet(width, height))
        throw ButcherException(_("Cannot set these mask positions"));

    name_=name;
    width_=width;
    height_=height;
    ProjectModified();
}




ButcherProjectMaskContainer *ButcherProjectMask::Container(ButcherProjectArea::areaclass_t areaclass)
{
    MaskContainers_t::iterator c;
    if ((c=containers_.find(areaclass))!=containers_.end())
        return c->second;
    return NULL;
}




ButcherProjectMaskContainerArea *ButcherProjectMask::Areas()
{
    return dynamic_cast<ButcherProjectMaskContainerArea*>(Container(ButcherProjectArea::AC_DEFAULT));
}




ButcherProjectMaskContainerAreaGlobal *ButcherProjectMask::AreasGlobal()
{
    return dynamic_cast<ButcherProjectMaskContainerAreaGlobal*>(Container(ButcherProjectArea::AC_GLOBAL));
}




ButcherProjectMaskContainerAreaMap *ButcherProjectMask::AreasMap()
{
    return dynamic_cast<ButcherProjectMaskContainerAreaMap*>(Container(ButcherProjectArea::AC_MAP));
}




bool ButcherProjectMask::HaveArea()
{
    return containers_.size()>0;
}

bool ButcherProjectMask::HaveVariableSizeArea(BLID_t viewid)
{
	if (!Areas()) return false; // only DEFAULT areas



	for (ButcherProjectAreas::iterator i=Areas()->Areas().begin(); i!=Areas()->Areas().end(); i++)
	{
		if (i->Configs().Get(viewid)->GetVariableSize())
			return true;
		if (i->Configs().Get(viewid)->GetAreaKind()==ButcherProjectAreaConfigBase::AK_MASK &&
			i->Configs().Get(viewid)->GetInnerMask()->HaveVariableSizeArea(viewid))
			return true;
	}

	return false;
}

/*
ButcherProjectMaskConfigs &ButcherProjectMask::Configs()
{
    if (!parent_) return *configs_;
    return parent_->GetMask()->Configs();
}
*/



long ButcherProjectMask::GetStartMinMaxPos(ButcherProjectLine::orientation_t orientation)
{
    if (!Areas()) return false;
    //long ret=GetStart(orientation)->GetPosition()+GetDrawStart(orientation), newr;
    long ret=Areas()->GetStart(orientation)->GetPosition(), newr;

    // returns the rightmost/bottommost start line of all areas
	for (ButcherProjectAreas::iterator i=Areas()->Areas().begin(); i!=Areas()->Areas().end(); i++)
	{
        newr=i->GetStartMinMaxPos(orientation);
        if (newr>ret)
            ret=newr;
	}

    return ret;
}




long ButcherProjectMask::GetEndMinMaxPos(ButcherProjectLine::orientation_t orientation)
{
    if (!Areas()) return false;
    //long ret=GetEnd(orientation)->GetPosition()+GetDrawStart(orientation), newr;
    long ret=Areas()->GetEnd(orientation)->GetPosition(), newr;

    // returns the leftmost/topmost end line of all areas
	for (ButcherProjectAreas::iterator i=Areas()->Areas().begin(); i!=Areas()->Areas().end(); i++)
	{
        newr=i->GetEndMinMaxPos(orientation);
        if (newr<ret)
            ret=newr;
	}

    return ret;
}




void ButcherProjectMask::DrawLine(ButcherViewBase *view, wxDC *dc, ButcherProjectLine *line,
    ButcherProjectView *projectview, drawmode_t drawmode, ButcherProjectMaskDrawSelection *selection)
{
    ButcherProjectArea *basearea;
    ButcherProjectLine::ButcherProjectLineAreas *la;

    wxPoint st, ed, l2(0,0);
    wxRect drawl;

    switch (line->GetOrientation()) {
    case ButcherProjectLine::BPL_HORIZONTAL:
        st.y=ed.y=line->GetPosition();
        st.x=line->GetStart()->GetPosition();
        ed.x=line->GetEnd()->GetPosition();
        l2.y=-1;
        break;
    case ButcherProjectLine::BPL_VERTICAL:
        st.x=ed.x=line->GetPosition();
        st.y=line->GetStart()->GetPosition();
        ed.y=line->GetEnd()->GetPosition();
        l2.x=-1;
        break;
    default:
        return;
        break;
    }

    // areas draw 2 lines for each line, do the same
    for (int dl=0; dl<2; dl++)
    {
        la=(dl==0?&line->areas_after_:&line->areas_before_); // after lines are the "MAIN" lines

        if (!la->empty())
        {
            // get the first one, to find the default color
            basearea=*la->begin();


            dc->SetPen(*GetDrawPen(drawmode, basearea, projectview, selection));
            dc->SetBrush(*wxTRANSPARENT_BRUSH);

            drawl=wxRect(view->PosToClient(st, wxPoint(GetDrawStartX(), GetDrawStartY())),
                view->PosToClient(ed, wxPoint(GetDrawStartX(), GetDrawStartY())));
            if (dl)
            {
                drawl.SetX(drawl.GetX()+l2.x);
                drawl.SetY(drawl.GetY()+l2.y);
            }

            dc->DrawLine(drawl.GetTopLeft(), drawl.GetBottomRight());
        }
    }

    dc->SetPen(wxNullPen);
    dc->SetBrush(wxNullBrush);
}




wxPen *ButcherProjectMask::GetDrawPen(drawmode_t drawmode, ButcherProjectArea *area,
    ButcherProjectView *projectview, ButcherProjectMaskDrawSelection *selection)
{
    if (selection && area)
        drawmode=selection->GetDrawMode(area, drawmode);
    ButcherProjectColorScheme::selectionpen_t sp=ButcherProjectColorScheme::SP_DEFAULT;
    // use hidden style for global areas of kind NONE
    if (projectview && area && area->GetAreaClass()==ButcherProjectArea::AC_GLOBAL &&
        area->Configs().Get(projectview->GetBLId())->GetAreaKind()==ButcherProjectAreaConfig::AK_NONE)
        sp=ButcherProjectColorScheme::SP_HIDDEN;

    if (drawmode == DM_SELECTED)
        return GetProject()->ColorSchemes().Current()->GetSelectionPen(ButcherProjectColorScheme::BCOLOR_AREABORDERSELECTED, 1, sp);
    else if (drawmode == DM_HOVER)
        return GetProject()->ColorSchemes().Current()->GetSelectionPen(ButcherProjectColorScheme::BCOLOR_AREABORDERHOVER, 1, sp);
    else if (area)
        return GetProject()->ColorSchemes().Current()->GetSelectionPen(area->GetDefaultColorItem(), 1, sp);
    else
        return GetProject()->ColorSchemes().Current()->GetSelectionPen(ButcherProjectColorScheme::BCOLOR_AREABORDER, 1, sp);
}



wxBrush *ButcherProjectMask::GetDrawBrush(drawmode_t drawmode, ButcherProjectArea *area,
    ButcherProjectMaskDrawSelection *selection)
{
    if (selection && area)
        drawmode=selection->GetDrawMode(area, drawmode);

    if (drawmode == DM_SELECTED)
        return GetProject()->ColorSchemes().Current()->GetBrush(ButcherProjectColorScheme::BCOLOR_AREABORDERSELECTED);
    else if (drawmode == DM_HOVER)
        return GetProject()->ColorSchemes().Current()->GetBrush(ButcherProjectColorScheme::BCOLOR_AREABORDERHOVER);
    else if (area)
        return GetProject()->ColorSchemes().Current()->GetBrush(area->GetDefaultColorItem());
    else
        return GetProject()->ColorSchemes().Current()->GetBrush(ButcherProjectColorScheme::BCOLOR_AREABORDER);
}





bool ButcherProjectMask::DrawArea(ButcherViewBase *view, wxDC *dc, ButcherProjectArea *area,
    ButcherProjectView *projectview, drawmode_t drawmode,
    ButcherProjectMaskDrawSelection *selection, bool recurse,
    ButcherProjectArea::areaselect_t areas, drawoptions_t options)
{
    bool ret=true, isdrawtitle=false;
    ButcherProjectAreaConfig *acfg=NULL;
    if (projectview && projectview->GetBLId()>0)
        acfg=area->Configs().Get(projectview->GetBLId());

    // don't draw global areas of kind NONE
	if (acfg && area && area->GetAreaClass()==ButcherProjectArea::AC_GLOBAL &&
        acfg->GetAreaKind()==ButcherProjectAreaConfig::AK_NONE)
		return false;

    dc->SetPen(*GetDrawPen(drawmode, area, projectview, selection));
    dc->SetBrush(*wxTRANSPARENT_BRUSH);

    wxRect r=area->GetRect();
    wxRect arearect=view->PosToClient(r, GetDrawStartPoint());
	if (arearect.GetWidth()==0) arearect.SetWidth(1);
	if (arearect.GetHeight()==0) arearect.SetHeight(1);

    //r.Offset(view->GetDesignLeft(), view->GetDesignTop());
    arearect.Offset(view->GetDesignLeft(), view->GetDesignTop());

    // CLIPPING
    dc->SetClippingRegion(arearect);

    // PREVIEW - check for changes
    if (acfg && view->GetShowPreview() && acfg->GetPreview(projectview->GetBLId())->NeedImage())
    {
        ret=(!acfg->GetPreview(projectview->GetBLId())->ConfigChanged()) && ret;
    }

    // BORDER
    dc->DrawRectangle(arearect);

	// SELECTED LINES
	if (selection)
	{
		for (unsigned short l=0; l<area->GetLineCount(); l++)
		{
			ButcherProjectMask::drawmode_t dm=selection->GetDrawMode(area->GetLine(l));
			if (dm!=ButcherProjectMask::DM_DEFAULT)
				DrawLine(view, dc, area->GetLine(l), projectview, dm);
		}

		// RESET AREA STYLE
		dc->SetPen(*GetDrawPen(drawmode, area, projectview, selection));
		dc->SetBrush(*wxTRANSPARENT_BRUSH);
	}


    // inner mask
    if (recurse && acfg->GetInnerMask()->HaveArea())
    {
        acfg->GetInnerMask()->Draw(view, dc, areas, projectview, drawmode, selection, options);

        // reset clipping, may have been changed
        dc->SetClippingRegion(arearect);

        // reset, inner mask may have modified
        dc->SetPen(*GetDrawPen(drawmode, area, projectview, selection));
        dc->SetBrush(*wxTRANSPARENT_BRUSH);

    }


    if (acfg && acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK)
    {
        if (recurse)
            // only last mask draw title
            isdrawtitle=true;
    }
    else
    {

        if (arearect.GetWidth()>10 && arearect.GetHeight()>10)
        {
            // draw "X"
            dc->DrawLine(arearect.GetLeftTop(), arearect.GetBottomRight());
            dc->DrawLine(arearect.GetTopRight(), arearect.GetBottomLeft());

            if (acfg && acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_NONE)
            {
                // draw "+"
                int hx=arearect.GetLeft()+(arearect.GetWidth()/2);
                int hy=arearect.GetTop()+(arearect.GetHeight()/2);
                dc->DrawLine(wxPoint(hx, arearect.GetTop()), wxPoint(hx, arearect.GetBottom()));
                dc->DrawLine(wxPoint(arearect.GetLeft(), hy), wxPoint(arearect.GetRight(), hy));
            }
        }
    }

    // TITLE
    dc->SetFont(*wxNORMAL_FONT);

    wxString anamefmt;
    wxSize curtsz(0, 0);
    wxPoint curtdrw(view->GetDesignLeft()+view->PosToClient(r.GetX()+GetDrawStartX()+(5*GetLevel())),
        view->GetDesignTop()+view->PosToClient(r.GetY()+GetDrawStartY())+(curtsz.GetHeight()*GetLevel()));

    wxSize dfontsize=dc->GetTextExtent(wxT("abcdefghijklmnopqrstuvwxyz"));
    // dont draw title if area is too small
    bool drawtitle_current=true;

    if ((options&DO_DONTDRAWTITLES)==DO_DONTDRAWTITLES)
        drawtitle_current=false;
    else
    {
        if (area->GetHeight()<dfontsize.GetHeight()*(area->GetContainedAreaClass()!=ButcherProjectArea::AC_MAP?2:4) ||
            area->GetWidth()<dfontsize.GetHeight()*(area->GetContainedAreaClass()!=ButcherProjectArea::AC_MAP?6:8))
            drawtitle_current=false;
    }

    if (!isdrawtitle && drawtitle_current)
    {
        ButcherProjectArea *tarea=area;
        ButcherProjectAreaConfig *tacfg=acfg;
        while (tarea)
        {
            anamefmt=tarea->GetDescription();
            if (true/*tarea->GetAreaClass() != ButcherProjectArea::AC_MAP || tarea->IsMap()*/) // don't draw inner areas for MAP
            {
                if (tarea->GetAreaClass() != ButcherProjectArea::AC_MAP &&
                    (tarea==area || area->IsFirst()))
                    //anamefmt+=wxString::Format(wxT(" [%s]"), tacfg->GetAreaKindID().c_str());
                    anamefmt+=wxString::Format(wxT(" [%s]"), tacfg->GetFlagsDescription().c_str());


                dc->SetBrush(*GetDrawBrush(drawmode, area, selection));

                if (tacfg && tarea->Configs().IsCustomization(projectview->GetBLId()))
                    dc->SetFont(*wxTheFontList->FindOrCreateFont(wxNORMAL_FONT->GetPointSize(), wxNORMAL_FONT->GetFamily(),
                        wxNORMAL_FONT->GetStyle(), wxFONTWEIGHT_BOLD, false, wxNORMAL_FONT->GetFaceName(),
                        wxNORMAL_FONT->GetDefaultEncoding()));
                else
                    dc->SetFont(*wxNORMAL_FONT);

                wxSize tsz=dc->GetTextExtent(anamefmt);
                wxPoint tdrw(view->GetDesignLeft()+view->PosToClient(r.GetX()+GetDrawStartX()+(5*tarea->GetMask()->GetLevel())),
                    view->GetDesignTop()+view->PosToClient(r.GetY()+GetDrawStartY())+(tsz.GetHeight()*tarea->GetMask()->GetLevel()));

                dc->DrawRectangle(tdrw.x, tdrw.y, tsz.GetWidth(), tsz.GetHeight());

                dc->SetBrush(*wxTRANSPARENT_BRUSH);

                dc->DrawText(anamefmt, tdrw.x, tdrw.y);
            }

            tarea=tarea->GetMask()->GetParent();
            if (tarea)
            {
                if (projectview && projectview->GetBLId()>0)
                    tacfg=tarea->Configs().Get(projectview->GetBLId());
                else
                    tacfg=NULL;
            }

        }
    }

    // PREVIEW IMAGE SIZE
    if (drawtitle_current && (area->GetFlags()&ButcherProjectArea::AF_HAVEIMAGE)==ButcherProjectArea::AF_HAVEIMAGE)
    {
        wxSize psz;
        wxPoint pdrw;
        dc->SetFont(*wxNORMAL_FONT);
        //dc->SetFont(*wxSMALL_FONT);

        if (acfg && view->GetShowPreview() &&
            acfg->GetPreview(projectview->GetBLId())->HavePreview() &&
            acfg->GetPreview(projectview->GetBLId())->GetPreview() &&
            !acfg->GetPreview(projectview->GetBLId())->IsError())
        {
            if (acfg->GetPreview(projectview->GetBLId())->GetPreview()->GetFileSize() >= 0)
            {
                anamefmt=wxString::Format(wxT("%s - %s"),
                    acfg->ImageInfo().GetImageFormat()->GetName().c_str(),
                    wxFileName::GetHumanReadableSize(acfg->GetPreview(projectview->GetBLId())->GetPreview()->GetFileSize(), wxT("0")).c_str());
                if (acfg->GetBackground())
                    anamefmt=wxString(_("BG:"))+anamefmt;
                psz=dc->GetTextExtent(anamefmt);


                dc->SetBrush(*GetDrawBrush(drawmode, area, selection));


                if (acfg->GetBackground() && acfg->GetAreaKind() == ButcherProjectAreaConfigBase::AK_MASK)
                {
                    // is BG but have inner mask, align bottom-right
                    pdrw.x=view->GetDesignLeft()+view->PosToClient(r.GetX()+GetDrawStartX()+r.GetWidth())-psz.GetWidth()-(5*area->GetMask()->GetLevel());
                    pdrw.y=view->GetDesignTop()+view->PosToClient(r.GetY()+GetDrawStartY()+r.GetHeight())-(psz.GetHeight()*(area->GetMask()->GetLevel()+1));
                }
                else
                {
                    // align middle
                    pdrw.x=view->GetDesignLeft()+view->PosToClient(r.GetX()+GetDrawStartX()+(r.GetWidth()/2))-(psz.GetWidth()/2);
                    pdrw.y=view->GetDesignTop()+view->PosToClient(r.GetY()+GetDrawStartY()+(r.GetHeight()/2))-(psz.GetHeight()/2);
                    if (curtdrw.y+curtsz.GetHeight() > pdrw.y)
                        // draw at bottom
                        pdrw.y=view->GetDesignTop()+view->PosToClient(r.GetBottom()+GetDrawStartY())-psz.GetHeight();
                }
                dc->DrawRectangle(pdrw.x, pdrw.y, psz.GetWidth(), psz.GetHeight());

                dc->SetBrush(*wxTRANSPARENT_BRUSH);

                dc->DrawText(anamefmt, pdrw.x, pdrw.y);
            }
        }
    }

    dc->DestroyClippingRegion();
    dc->SetPen(wxNullPen);
    dc->SetBrush(wxNullBrush);

    return ret;
}




void ButcherProjectMask::GeneratePreviewArea(ButcherProjectArea *area,
    ButcherProjectView *projectview, bool recurse,
    ButcherProjectArea::areaselect_t areas)
{
    ButcherProjectAreaConfig *acfg=NULL;
    if (projectview && projectview->GetBLId()>0)
        acfg=area->Configs().Get(projectview->GetBLId());

    // PREVIEW
    if (acfg)
        acfg->GetPreview(projectview->GetBLId())->GetPreview();

    // inner mask
    if (acfg && acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK)
        if (recurse)
            acfg->GetInnerMask()->GeneratePreview(areas, projectview);
}



void ButcherProjectMask::DrawPreviewArea(ButcherViewBase *view, wxDC *dc, ButcherProjectArea *area,
    ButcherProjectView *projectview, bool recurse,
    ButcherProjectArea::areaselect_t areas, drawoptions_t options)
{
    ButcherProjectAreaConfig *acfg=NULL;
    if (projectview && projectview->GetBLId()>0)
        acfg=area->Configs().Get(projectview->GetBLId());

    wxRect r=area->GetRect();
    wxRect arearect=view->PosToClient(r, GetDrawStartPoint());

    //r.Offset(view->GetDesignLeft(), view->GetDesignTop());
    arearect.Offset(view->GetDesignLeft(), view->GetDesignTop());

    // PREVIEW
    if (acfg && acfg->GetPreview(projectview->GetBLId())->GetPreview())
    {
        wxRect dr(area->GetRect().GetTopLeft(), acfg->GetPreview(projectview->GetBLId())->GetPreview()->GetImageRect().GetSize());;
        wxRect darearect=view->PosToClient(dr, GetDrawStartPoint());

        ButcherImage::repeat_t drepeat=ButcherImage::DR_NONE;
        if (acfg->GetBackground())
        {
            switch (acfg->GetBGRepeat())
            {
            case ButcherProjectAreaConfig::BR_ALL: drepeat=ButcherImage::DR_ALL; break;
            case ButcherProjectAreaConfig::BR_HORIZONTAL: drepeat=ButcherImage::DR_HORIZONTAL; break;
            case ButcherProjectAreaConfig::BR_VERTICAL: drepeat=ButcherImage::DR_VERTICAL; break;
            default: break;
            }
        }
        else
            darearect=arearect;

        acfg->GetPreview(projectview->GetBLId())->GetPreview()->DrawRepeat(*dc, darearect.GetSize(), arearect, drepeat);
    }

    // bgcolor
    if (acfg && acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_NONE && acfg->GetIsBGColor())
    {
        dc->SetBrush(*wxTheBrushList->FindOrCreateBrush(acfg->GetBGColor()));
        dc->SetPen(*wxTRANSPARENT_PEN);

        dc->DrawRectangle(arearect);

        dc->SetBrush(wxNullBrush);
        dc->SetPen(wxNullPen);
    }

    // content
    if (acfg &&
        (!acfg->GetContent().IsEmpty()) &&
        (acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_NONE ||
        (acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_IMAGE && acfg->GetBackground())))
    {
        wxHtmlDCRenderer htmlrender;
        wxArrayInt unused;

        wxString tdprop=wxEmptyString;
        // ALIGN
        if (acfg->GetAlign()==ButcherProjectAreaConfigBase::AA_LEFT)
            tdprop+=wxT(" align=\"left\"");
        else if (acfg->GetAlign()==ButcherProjectAreaConfigBase::AA_CENTER)
            tdprop+=wxT(" align=\"center\"");
        else if (acfg->GetAlign()==ButcherProjectAreaConfigBase::AA_RIGHT)
            tdprop+=wxT(" align=\"right\"");

        // VALIGN
        if (acfg->GetVAlign()==ButcherProjectAreaConfigBase::AV_MIDDLE)
            tdprop+=wxT(" valign=\"middle\"");
        else if (acfg->GetVAlign()==ButcherProjectAreaConfigBase::AV_TOP)
            tdprop+=wxT(" valign=\"top\"");
        else if (acfg->GetVAlign()==ButcherProjectAreaConfigBase::AV_BOTTOM)
            tdprop+=wxT(" valign=\"bottom\"");

        // CELL TAG APPEND
        if (!acfg->GetCellTagAppend().IsEmpty())
        {
            tdprop+=wxT(" ");
            tdprop+=acfg->GetCellTagAppend();
        }

        wxString htmldata(wxEmptyString);
        htmldata+=wxString::Format(wxT("<html><body><table width=\"100%%\" cellspacing=\"0\" cellpadding=\"0\"><tr><td %s>"),
            /*arearect.GetWidth(), */tdprop.c_str());

        ButcherProjectTemplateParser contentparser(GetProject(), acfg->GetContent());
        //htmldata+=acfg->GetContent();
        htmldata+=contentparser.GetResult();
        htmldata+=wxT("</td></tr></table></body></html>");

        htmlrender.SetDC(dc, view->GetPixelSize());
        htmlrender.SetSize(arearect.GetWidth(), arearect.GetHeight());
        htmlrender.SetHtmlText(htmldata);
        htmlrender.Render(arearect.GetLeft(), arearect.GetTop(), unused);
        dc->SetUserScale(1.0, 1.0);
    }



    // inner mask
    if (acfg && acfg->GetAreaKind() == ButcherProjectAreaConfig::AK_MASK)
        if (recurse)
            acfg->GetInnerMask()->DrawPreview(view, dc, areas, projectview, options);
}



bool ButcherProjectMask::Draw(ButcherViewBase *view, wxDC *dc, ButcherProjectArea::areaselect_t areas,
    ButcherProjectView *projectview, drawmode_t drawmode, ButcherProjectMaskDrawSelection *selection,
    drawoptions_t options)
{
    bool ret=true;
    //ButcherListIdList ids;

    for (MaskContainers_t::iterator c=containers_.begin(); c!=containers_.end(); c++)
    {
        if (c->second->IsAreaSelect(areas))
        {
			for (ButcherProjectAreas::iterator i=c->second->Areas().begin(); i!=c->second->Areas().end(); i++)
			{
				ret=DrawArea(view, dc, &*i, projectview, drawmode,
					selection, true, areas, options) && ret;
			}
        }

    }

    dc->SetPen(wxNullPen);
    dc->SetBrush(wxNullBrush);
    return ret;
}




void ButcherProjectMask::GeneratePreview(ButcherProjectArea::areaselect_t areas,
    ButcherProjectView *projectview)
{
    //ButcherListIdList ids;

    for (MaskContainers_t::iterator c=containers_.begin(); c!=containers_.end(); c++)
    {
        if (c->second->IsAreaSelect(areas))
        {
			for (ButcherProjectAreas::iterator i=c->second->Areas().begin(); i!=c->second->Areas().end(); i++)
			{
                if ((i->GetFlags()&&ButcherProjectArea::AF_HAVEIMAGE)==ButcherProjectArea::AF_HAVEIMAGE)
                    GeneratePreviewArea(&*i, projectview, true, areas);
			}

        }
    }
}



void ButcherProjectMask::DrawPreview(ButcherViewBase *view, wxDC *dc, ButcherProjectArea::areaselect_t areas,
    ButcherProjectView *projectview, drawoptions_t options)
{
    //ButcherListIdList ids;

    for (MaskContainers_t::iterator c=containers_.begin(); c!=containers_.end(); c++)
    {
        if (c->second->IsAreaSelect(areas))
        {
			for (ButcherProjectAreas::iterator i=c->second->Areas().begin(); i!=c->second->Areas().end(); i++)
			{
                if ((i->GetFlags()&&ButcherProjectArea::AF_HAVEIMAGE)==ButcherProjectArea::AF_HAVEIMAGE)
                    DrawPreviewArea(view, dc, &*i, projectview, true, areas, options);
			}

        }
    }

    dc->SetPen(wxNullPen);
    dc->SetBrush(wxNullBrush);
}



bool ButcherProjectMask::FindLine(ButcherProjectArea::areaselect_t areas, findline_t type,
    long x, long y, ButcherProjectMaskLineSelect &select, BLID_t viewid, int maxlevel)
{


	ButcherProjectLine::orientation_t orientation=(type==FL_HORIZONTAL?ButcherProjectLine::BPL_HORIZONTAL:ButcherProjectLine::BPL_VERTICAL);

    bool ret=false;
    //ButcherListIdList ids;

    for (MaskContainers_t::iterator c=containers_.begin(); c!=containers_.end(); c++)
    {
		if (c->second->IsAreaSelect(areas) &&
			dynamic_cast<ButcherProjectMaskContainerJoined*>(c->second)!=NULL) // only lines from joined containers can be selected
        {
			for (ButcherProjectLines::iterator i=c->second->Lines().begin(); i!=c->second->Lines().end(); i++)
			{
                if (type==FL_BOTH || i->GetOrientation() == orientation) {
                    if (i->Select(x-GetDrawStartX(), y-GetDrawStartY())) {
                        ret=true;
                        select.Add(this, &*i);
                    }
                }
			}
        }

        if (viewid > 0 && (maxlevel==-1 || GetLevel()<maxlevel))
        {
            // find in inner areas (only for the current mask)
			for (ButcherProjectAreas::iterator i=c->second->Areas().begin(); i!=c->second->Areas().end(); i++)
			{
                if (i->Configs().Get(viewid)->GetInnerMask()->HaveArea())
                    ret=i->Configs().Get(viewid)->GetInnerMask()->FindLine(areas, type,
                        x, y, select, viewid, maxlevel) || ret;
			}
        }

    }


    return ret;
}




bool ButcherProjectMask::FindArea(ButcherProjectArea::areaselect_t areas, long x, long y, ButcherProjectMaskAreaSelect &select,
    BLID_t viewid, int maxlevel)
{
	bool ret=false;
    //ButcherListIdList ids;


    for (MaskContainers_t::iterator c=containers_.begin(); c!=containers_.end(); c++)
    {
        if (c->second->IsAreaSelect(areas))
        {
			for (ButcherProjectAreas::iterator i=c->second->Areas().begin(); i!=c->second->Areas().end(); i++)
			{
                if (i->Select(x-GetDrawStartX(), y-GetDrawStartY()))
                {
                    ret=true;
                    select.Add(this, &*i);
                }
			}
        }

        if (viewid > 0 && (maxlevel==-1 || GetLevel()<maxlevel))
        {
            // find in inner areas
			for (ButcherProjectAreas::iterator i=c->second->Areas().begin(); i!=c->second->Areas().end(); i++)
			{
                ret=i->Configs().Get(viewid)->GetInnerMask()->FindArea(areas,
                    x, y, select, viewid, maxlevel) || ret;
			}
        }
    }

    return ret;
}



ButcherProjectMask *ButcherProjectMask::GetMaskRoot()
{
	if (GetParent()) return GetParent()->GetMaskRoot();
	return this;
}




int ButcherProjectMask::GetLevel()
{
    if (parent_) return parent_->GetMask()->GetLevel()+1;
    return 0;
}




void ButcherProjectMask::LoadMetadata(ButcherMetadataData *metadata, int loadid)
{
    ButcherProjectBaseAutoUpdate autoupd(this);

    if ((!metadata->Exists(BFILE_MDI_ID, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_NAME, ButcherMetadataDataItem::DT_STRING)) ||
        //(!metadata->Exists(BFILE_MDI_PARENT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_WIDTH, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_HEIGHT, ButcherMetadataDataItem::DT_INTEGER)) ||
        (!metadata->Exists(BFILE_MDI_CONTAINERS, ButcherMetadataDataItem::DT_METADATA)))
        throw ButcherException(_("Invalid metadata for mask"));

    if (loadid==-1)
        SetBLId(metadata->Get(BFILE_MDI_ID).GetInteger());
    else
        SetBLId(loadid);
    name_=metadata->Get(BFILE_MDI_NAME).GetString();
    //parent_=metadata->Get(BFILE_MDI_PARENT).GetInteger();
    width_=metadata->Get(BFILE_MDI_WIDTH).GetInteger();
    height_=metadata->Get(BFILE_MDI_HEIGHT).GetInteger();

    if (metadata->Exists(BFILE_MDI_TABLETAGADD, ButcherMetadataDataItem::DT_STRING))
        tabletagadd_=metadata->Get(BFILE_MDI_TABLETAGADD).GetString();

/*
    if (!parent_)
    {
        if (!configs_)
        {
            configs_=new ButcherProjectMaskConfigs(this);
            configs_->Add(wxT("DEFAULT"), 1);
        }
    }
    else if (configs_)
    {
        delete configs_;
        configs_=NULL;
    }

    if (configs_ &&
        metadata->Exists(BFILE_MDI_CONFIGS, ButcherMetadataDataItem::DT_METADATA) &&
        metadata->Get(BFILE_MDI_CONFIGS).GetMetadata().Exists(BFILE_MD_CONFIG))
        configs_->LoadMetadata(&metadata->Get(BFILE_MDI_CONFIGS).GetMetadata().Get(BFILE_MD_CONFIG));
*/

    // containers
    if (metadata->Exists(BFILE_MDI_CONTAINERS, ButcherMetadataDataItem::DT_METADATA) &&
        metadata->Get(BFILE_MDI_CONTAINERS).GetMetadata().Exists(BFILE_MD_CONTAINER))
    {
        for (unsigned int i=0; i<metadata->Get(BFILE_MDI_CONTAINERS).GetMetadata().Get(BFILE_MD_CONTAINER).Count(); i++)
        {
            LoadMetadataContainer(&metadata->Get(BFILE_MDI_CONTAINERS).GetMetadata().Get(BFILE_MD_CONTAINER).Get(i));
        }
    }
}




void ButcherProjectMask::LoadMetadataContainer(ButcherMetadataData *metadata)
{
    if (metadata->Exists(BFILE_MDI_AREACLASS))
    {

        if (Container(static_cast<ButcherProjectArea::areaclass_t>(metadata->Get(BFILE_MDI_AREACLASS).GetInteger())))
            Container(static_cast<ButcherProjectArea::areaclass_t>(metadata->Get(BFILE_MDI_AREACLASS).GetInteger()))->LoadMetadata(metadata);
    }
    else
        throw ButcherException(_("Invalid metadata for container"));
}



void ButcherProjectMask::SaveMetadata(ButcherMetadataData *metadata)
{
    metadata->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_ID, GetBLId()))->
        MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_NAME, name_))->
        //MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_PARENT, (parent_?parent_->GetBLId():0)))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_WIDTH, width_))->
        MultiAdd(new ButcherMetadataDataItem_Integer(metadata, BFILE_MDI_HEIGHT, height_));

    if (!tabletagadd_.IsEmpty())
        metadata->MultiAdd(new ButcherMetadataDataItem_String(metadata, BFILE_MDI_TABLETAGADD, tabletagadd_));

/*
	// configs
    if (configs_)
    {
        metadata->Add(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_CONFIGS));
        configs_->SaveMetadata(&metadata->Get(BFILE_MDI_CONFIGS).GetMetadata().Get(BFILE_MD_CONFIG, true));
    }
*/

    // containers
    if (containers_.size()>0)
    {
        metadata->Add(new ButcherMetadataDataItem_Metadata(metadata, BFILE_MDI_CONTAINERS));

        MaskContainers_t::iterator c;
        for (c=containers_.begin(); c!=containers_.end(); c++)
        {
            c->second->SaveMetadata(metadata->Get(BFILE_MDI_CONTAINERS).GetMetadata().Get(BFILE_MD_CONTAINER, true).Add());
        }
    }
}





void ButcherProjectMask::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseBLId::ProjectEvent(event);
    for (MaskContainers_t::iterator i=containers_.begin(); i!=containers_.end(); i++)
        i->second->ProjectEvent(event);
}


