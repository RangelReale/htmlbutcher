//=============================================================================
/**
 *  @file   ButcherProjectMaskLayout.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include <map>
#include "ButcherProjectMaskLayout.h"
#include "ButcherProjectMask.h"



/////////////////////////////////
// CLASS
//      ButcherProjectMaskLayout
/////////////////////////////////
ButcherProjectMaskLayout::ButcherProjectMaskLayout(ButcherProjectMask *mask, bool alignmentrow) :
    ButcherProjectBase(mask->GetProject()), mask_(mask), list_(), hlines_(), vlines_(),
    alignmentrow_(alignmentrow)
{
    build();
}



int ButcherProjectMaskLayout::bldlinespos(ButcherProjectMaskLayout::bldlines *ln, long position)
{
    int ret=0;
    for (bldlines::const_iterator i=ln->begin(); i!=ln->end(); i++)
    {
        if (i->first == position)
        {
            return ret;
        }
        ret++;
    }

    return -1;
}




void ButcherProjectMaskLayout::build()
{


	if (mask_->Areas())
    {
        ButcherProjectMaskContainerArea *cnt=mask_->Areas();

        // build an ordered list of horizontal and vertical lines, based on position
        bldlines *curlines;

        // add default lines
/*
        hlines_[cnt->GetLineTop()].push_back(cnt->GetLineTop());
        hlines_[cnt->GetLineBottom()->GetPosition()].push_back(cnt->GetLineBottom());
        vlines_[cnt->GetLineLeft()].push_back(cnt->GetLineLeft());
        vlines_[cnt->GetLineRight()->GetPosition()].push_back(cnt->GetLineRight());
*/

		for (ButcherProjectLines::iterator i=cnt->Lines().begin(); i!=cnt->Lines().end(); i++)
		{
            if (i->GetOrientation()==ButcherProjectLine::BPL_HORIZONTAL)
                curlines=&hlines_;
            else
                curlines=&vlines_;

            //if (curlines->find(i->GetPosition()) == curlines->end())
            (*curlines)[i->GetPosition()].push_back(&*i);
            //el
		}

        // build an array with the areas in the actual position on the final table
        typedef map<int, ButcherProjectArea*> arreaitemarray_t;
        typedef map< int, arreaitemarray_t > areaarray_t;
        areaarray_t areaarray;

		for (ButcherProjectAreas::iterator i=cnt->Areas().begin(); i!=cnt->Areas().end(); i++)
		{
            areaarray[bldlinespos(&hlines_, i->GetTop()->GetPosition())][bldlinespos(&vlines_, i->GetLeft()->GetPosition())]=&*i;
		}

        // populate the list
        ButcherProjectMaskLayoutItem newlay;
        bool isnewline=false;

        for (areaarray_t::iterator i=areaarray.begin(); i!=areaarray.end(); i++)
        {
            isnewline=true && !list_.empty();
            for (arreaitemarray_t::iterator j=i->second.begin(); j!=i->second.end(); j++)
            {
                memset(&newlay, 0, sizeof(newlay));
                newlay.areaid=j->second->GetBLId();
                newlay.colspan=bldlinespos(&vlines_, j->second->GetRight()->GetPosition()) - j->first;
                newlay.rowspan=bldlinespos(&hlines_, j->second->GetBottom()->GetPosition()) - i->first;
                newlay.skipline=isnewline;
                list_.push_back(newlay);

                isnewline=false;
            }
        }

        // column alignment row
        if (alignmentrow_)
        {
            isnewline=true;
            long lastpos=-1;
            for (bldlines::const_iterator ai=vlines_.begin(); ai!=vlines_.end(); ai++)
            {
                if (lastpos>-1)
                {
                    memset(&newlay, 0, sizeof(newlay));
                    newlay.areaid=0;
                    newlay.colspan=1;
                    newlay.rowspan=1;
                    newlay.forcewidth=ai->first-lastpos;
                    newlay.forceheight=1;
                    newlay.skipline=isnewline;
                    list_.push_back(newlay);
                    isnewline=false;
                }
                lastpos=ai->first;
            }
        }
    }
}




