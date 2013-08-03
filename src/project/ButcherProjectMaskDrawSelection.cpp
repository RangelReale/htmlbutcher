//=============================================================================
/**
 *  @file   ButcherProjectMaskDrawSelection.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectMaskDrawSelection.h"
#include "ButcherProjectView.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskDrawSelection
/////////////////////////////////
ButcherProjectMaskDrawSelection::ButcherProjectMaskDrawSelection(ButcherProjectMask *mask) :
    ButcherProjectBase(mask->GetProject()), mask_(mask),
    ahover_(NULL), aselect_(NULL), lhover_(NULL), lselect_(NULL)
{

}




ButcherProjectMaskDrawSelection::~ButcherProjectMaskDrawSelection()
{
    Clear();
}




void ButcherProjectMaskDrawSelection::Clear()
{
    if (ahover_) delete ahover_;
    if (aselect_) delete aselect_;
    if (lhover_) delete lhover_;
    if (lselect_) delete lselect_;
    ahover_=aselect_=NULL;
    lhover_=lselect_=NULL;
}



void ButcherProjectMaskDrawSelection::Add(ButcherProjectMask::drawmode_t drawmode,
    ButcherProjectArea *area)
{
    switch (drawmode)
    {
    case ButcherProjectMask::DM_HOVER:
        GetAHover()->Add(mask_, area);
        break;
    case ButcherProjectMask::DM_SELECTED:
        GetASelect()->Add(mask_, area);
        break;
    default:
        break;
    }
}




void ButcherProjectMaskDrawSelection::Add(ButcherProjectMask::drawmode_t drawmode,
    ButcherProjectLine *line)
{
    switch (drawmode)
    {
    case ButcherProjectMask::DM_HOVER:
        GetLHover()->Add(mask_, line);
        break;
    case ButcherProjectMask::DM_SELECTED:
        GetLSelect()->Add(mask_, line);
        break;
    default:
        break;
    }
}



void ButcherProjectMaskDrawSelection::Set(ButcherProjectMask::drawmode_t drawmode,
    ButcherProjectMaskAreaSelect *areaselect)
{
    switch (drawmode)
    {
    case ButcherProjectMask::DM_HOVER:
        GetAHover(true, true, areaselect);
        break;
    case ButcherProjectMask::DM_SELECTED:
        GetASelect(true, true, areaselect);
        break;
    default:
        break;
    }
}




void ButcherProjectMaskDrawSelection::Set(ButcherProjectMask::drawmode_t drawmode,
    ButcherProjectMaskLineSelect *lineselect)
{
    switch (drawmode)
    {
    case ButcherProjectMask::DM_HOVER:
        GetLHover(true, true, lineselect);
        break;
    case ButcherProjectMask::DM_SELECTED:
        GetLSelect(true, true, lineselect);
        break;
    default:
        break;
    }
}




ButcherProjectMask::drawmode_t ButcherProjectMaskDrawSelection::GetDrawMode(ButcherProjectArea *area,
    ButcherProjectMask::drawmode_t dmdefault)
{
    if (ahover_ && ahover_->Exists(area))
        return ButcherProjectMask::DM_HOVER;
    if (aselect_ && aselect_->Exists(area))
        return ButcherProjectMask::DM_SELECTED;
    return dmdefault;
}




ButcherProjectMask::drawmode_t ButcherProjectMaskDrawSelection::GetDrawMode(ButcherProjectLine *line,
    ButcherProjectMask::drawmode_t dmdefault)
{
    if (lhover_ && lhover_->Exists(line))
        return ButcherProjectMask::DM_HOVER;
    if (lselect_ && lselect_->Exists(line))
        return ButcherProjectMask::DM_SELECTED;
    return dmdefault;
}




ButcherProjectMaskAreaSelect *ButcherProjectMaskDrawSelection::GetAHover(bool recreate,
    bool set, ButcherProjectMaskAreaSelect *copy)
{
    if (ahover_ && recreate)
    {
        delete ahover_;
        ahover_=NULL;
    }
    if (!ahover_)
    {
        if (!recreate || !set)
            ahover_=new ButcherProjectMaskAreaSelect(mask_);
        else if (copy)
            ahover_=new ButcherProjectMaskAreaSelect(*copy);
    }
    return ahover_;
}



ButcherProjectMaskAreaSelect *ButcherProjectMaskDrawSelection::GetASelect(bool recreate,
    bool set, ButcherProjectMaskAreaSelect *copy)
{
    if (aselect_ && recreate)
    {
        delete aselect_;
        aselect_=NULL;
    }
    if (!aselect_)
    {
        if (!recreate || !set)
            aselect_=new ButcherProjectMaskAreaSelect(mask_);
        else if (copy)
            aselect_=new ButcherProjectMaskAreaSelect(*copy);
    }
    return aselect_;
}




ButcherProjectMaskLineSelect *ButcherProjectMaskDrawSelection::GetLHover(bool recreate,
    bool set, ButcherProjectMaskLineSelect *copy)
{
    if (lhover_ && recreate)
    {
        delete lhover_;
        lhover_=NULL;
    }
    if (!lhover_)
    {
        if (!recreate || !set)
            lhover_=new ButcherProjectMaskLineSelect(mask_);
        else if (copy)
            lhover_=new ButcherProjectMaskLineSelect(*copy);
    }
    return lhover_;
}




ButcherProjectMaskLineSelect *ButcherProjectMaskDrawSelection::GetLSelect(bool recreate,
    bool set, ButcherProjectMaskLineSelect *copy)
{
    if (lselect_ && recreate)
    {
        delete lselect_;
        lselect_=NULL;
    }
    if (!lselect_)
    {
        if (!recreate || !set)
            lselect_=new ButcherProjectMaskLineSelect(mask_);
        else if (copy)
            lselect_=new ButcherProjectMaskLineSelect(*copy);
    }
    return lselect_;
}




void ButcherProjectMaskDrawSelection::Draw(ButcherViewBase *view, wxDC *dc, ButcherProjectView *projectview,
	ButcherProjectMask::drawoptions_t options)
{
	// areas
	ButcherProjectMaskAreaSelect *alist[2] = { ahover_, aselect_ };
	ButcherProjectMask::drawmode_t amode[2] = { ButcherProjectMask::DM_HOVER, ButcherProjectMask::DM_SELECTED };

	for (int i=0; i<2; i++)
		if (alist[i])
			for (unsigned long ct=0; ct<alist[i]->GetCount(); ct++)
			{
				alist[i]->GetItem(ct)->GetMask()->DrawArea(view, dc, alist[i]->GetItem(ct),
					projectview, amode[i], NULL, false, ButcherProjectArea::AS_ALL, options);
			}

	// lines
	ButcherProjectMaskLineSelect *llist[2] = { lhover_, lselect_ };
	ButcherProjectMask::drawmode_t lmode[2] = { ButcherProjectMask::DM_HOVER, ButcherProjectMask::DM_SELECTED };

	for (int i=0; i<2; i++)
		if (llist[i])
			for (unsigned long ct=0; ct<llist[i]->GetCount(); ct++)
			{
				llist[i]->GetItem(ct)->GetMask()->DrawLine(view, dc, llist[i]->GetItem(ct),
					projectview, lmode[i]);
			}
}


