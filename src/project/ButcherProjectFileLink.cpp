//=============================================================================
/**
 *  @file   ButcherProjectFileLink.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================

#include "ButcherProjectFileLink.h"
#include "ButcherProjectArea.h"
#include "ButcherProjectView.h"
#include "ButcherProject.h"

#include <wx/tokenzr.h>



/////////////////////////////////
// CLASS
//      ButcherProjectFileLink
/////////////////////////////////



ButcherProjectFileLink::ButcherProjectFileLink(ButcherProject *project,
    const wxString &burl) :
    ButcherProjectBaseModify(project), parent_(NULL), id_(0), linktype_(IL_NONE), arealist_(),
    alternate_(-1)
{
    if (!burl.IsEmpty())
        SetBURL(burl);
}




ButcherProjectFileLink::ButcherProjectFileLink(ButcherProjectBaseModify *parent, const wxString &burl) :
    ButcherProjectBaseModify(parent->GetProject()), parent_(parent), id_(0), linktype_(IL_NONE), arealist_(),
    alternate_(-1)
{
    if (!burl.IsEmpty())
        SetBURL(burl);
}




void ButcherProjectFileLink::ProjectEvent(ButcherProjectEventNotify &event)
{
    ButcherProjectBaseModify::ProjectEvent(event);
    switch (event.GetProjectEvent().GetEvent())
    {
    case ButcherProjectEvent::BPE_VIEWDELETED:
        if ((linktype_==IL_AREA||linktype_==IL_GLOBALAREA) &&
            (event.GetProjectEvent().GetEId()==0 || id_==event.GetProjectEvent().GetEId()))
        {
            event.AddChange();
            if (event.GetIsExecute())
                SetNone();
        }
        break;
// when mask is deleted, view is deleted too
//    case ButcherProjectEvent::BPE_MASKDELETED:
    case ButcherProjectEvent::BPE_ASSORTEDFILEDELETED:
        if (linktype_==IL_ASSORTEDFILE &&
            (event.GetProjectEvent().GetEId()==0 || id_==event.GetProjectEvent().GetEId()))
        {
            event.AddChange();
            if (event.GetIsExecute())
                SetNone();
        }
        break;
    case ButcherProjectEvent::BPE_FILEDELETED:
        if (linktype_==IL_FILE &&
            (event.GetProjectEvent().GetEId()==0 || id_==event.GetProjectEvent().GetEId()))
        {
            event.AddChange();
            if (event.GetIsExecute())
                SetNone();
        }
        break;
    default:
        break;
    }
}




void ButcherProjectFileLink::SetArea(ButcherProjectView *view, ButcherProjectArea *area, int alternate)
{
    if (area->GetAreaClass()==ButcherProjectArea::AC_GLOBAL)
        linktype_=IL_GLOBALAREA;
    else if (area->GetAreaClass()==ButcherProjectArea::AC_DEFAULT)
        linktype_=IL_AREA;
    else
        throw ButcherException(_("This area cannot be linked to"));
    id_=view->GetBLId();
    arealist_.clear();
    arealist_.push_front(area->GetBLId());
    while (area->GetMask()->GetParent())
    {
        area=area->GetMask()->GetParent();
        arealist_.push_front(area->GetBLId());
    }
    alternate_=alternate;

    ProjectModified();
}




void ButcherProjectFileLink::SetAreaAlternate(int alternate)
{
    if (linktype_!=IL_AREA && linktype_!=IL_GLOBALAREA)
        throw ButcherException(_("This link does not have an alternate item"));
    alternate_=alternate;

    ProjectModified();
}





ButcherProjectView *ButcherProjectFileLink::GetView() const
{
    if (linktype_!=IL_AREA && linktype_!=IL_GLOBALAREA)
        throw ButcherException(_("This is not an area link"));
    if (GetProject()->Views().Exists(id_))
        return GetProject()->Views()[id_];
    return NULL;
}




ButcherProjectArea *ButcherProjectFileLink::GetArea() const
{
    ButcherProjectView *view=GetView();

    ButcherProjectArea::areaclass_t aclass=(linktype_==IL_AREA?ButcherProjectArea::AC_DEFAULT:ButcherProjectArea::AC_GLOBAL);
    ButcherProjectMaskContainer *container=view->GetMask()->Container(aclass);

    ButcherProjectArea *curarea=NULL;

    for (arealist_t::const_iterator i=arealist_.begin(); i!=arealist_.end(); i++)
    {
        if (!container->Areas().Exists(*i))
            throw ButcherException(_("Linked area not found"));
        curarea=container->Areas().Get(*i);
        container=curarea->Configs().Get(view->GetBLId())->GetInnerMask()->Container(ButcherProjectArea::AC_DEFAULT);
    }
    return curarea;
}



void ButcherProjectFileLink::GetAreas(areas_t &a) const
{
    a.clear();
    ButcherProjectView *view=GetView();

    ButcherProjectArea::areaclass_t aclass=(linktype_==IL_AREA?ButcherProjectArea::AC_DEFAULT:ButcherProjectArea::AC_GLOBAL);
    ButcherProjectMaskContainer *container=view->GetMask()->Container(aclass);

    ButcherProjectArea *curarea=NULL;

    for (arealist_t::const_iterator i=arealist_.begin(); i!=arealist_.end(); i++)
    {
        if (!container->Areas().Exists(*i))
            throw ButcherException(_("Linked area not found"));
        curarea=container->Areas().Get(*i);
        a.push_back(curarea);
        container=curarea->Configs().Get(view->GetBLId())->GetInnerMask()->Container(ButcherProjectArea::AC_DEFAULT);
    }
}





ButcherProjectAreaImageInfo *ButcherProjectFileLink::GetImageInfo() const
{
    return &GetArea()->Configs().Get(GetView()->GetBLId())->GetValidImageInfo(alternate_);
}




void ButcherProjectFileLink::SetFile(BLID_t fileid)
{
    id_=fileid;
    linktype_=IL_FILE;
    arealist_.clear();

    ProjectModified();
}




void ButcherProjectFileLink::SetNone()
{
    id_=0;
    linktype_=IL_NONE;
    arealist_.clear();

    ProjectModified();
}




void ButcherProjectFileLink::SetAssortedFile(BLID_t assortedfileid)
{
    id_=assortedfileid;
    linktype_=IL_ASSORTEDFILE;
    arealist_.clear();

    ProjectModified();
}




bool ButcherProjectFileLink::IsValid() const
{
    switch (linktype_)
    {
    case IL_AREA:
    case IL_GLOBALAREA:
        {
            ButcherProjectView *view=GetView();
            if (!view) return false;

            ButcherProjectArea::areaclass_t aclass=(linktype_==IL_AREA?ButcherProjectArea::AC_DEFAULT:ButcherProjectArea::AC_GLOBAL);
            ButcherProjectMaskContainer *container=view->GetMask()->Container(aclass);

            ButcherProjectArea *curarea=NULL;

            for (arealist_t::const_iterator i=arealist_.begin(); i!=arealist_.end(); i++)
            {
                if (!container || !container->Areas().Exists(*i))
                    return false;
                curarea=container->Areas().Get(*i);
                container=curarea->Configs().Get(view->GetBLId())->GetInnerMask()->Container(ButcherProjectArea::AC_DEFAULT);
            }
            // check alternate enabled
            if (alternate_>=0 && !curarea->Configs().Get(view->GetBLId())->AlternateImageInfo(alternate_).GetEnabled())
                return false;

            return true;
        }
    case IL_FILE:
        return GetProject()->Files().Exists(id_);
    case IL_ASSORTEDFILE:
        return GetProject()->AssortedFiles().Exists(id_);
    default:
        return false;
    }

    return false;
}




bool ButcherProjectFileLink::Compare(const ButcherProjectFileLink& other) const
{
    return
        GetProject()==other.GetProject() &&
        linktype_==other.linktype_ &&
        id_==other.id_ &&
        arealist_==other.arealist_ &&
        alternate_==other.alternate_;
}

void ButcherProjectFileLink::Copy(const ButcherProjectFileLink& other)
{
	if (GetProject()!=other.GetProject())
		throw ButcherException(_("Incompatible source"));

	id_=other.id_;
	linktype_=other.linktype_;
	arealist_=other.arealist_;
	alternate_=other.alternate_;

	ProjectModified();
}



bool ButcherProjectFileLink::IsImage() const
{
    if (!IsValid()) return false;

    if (linktype_==IL_FILE||linktype_==IL_AREA||linktype_==IL_GLOBALAREA)
        return true;
    if (linktype_==IL_ASSORTEDFILE)
        if (IsValid() && GetProject()->AssortedFiles().Get(id_)->GetFileType()==ButcherProjectAssortedFile::AFT_IMAGE)
            return true;

    return false;
}




ButcherImage *ButcherProjectFileLink::CreateImage() const
{
    if (!IsImage())
        throw ButcherException(_("This link does not point to an image"));

    switch (linktype_)
    {
    case IL_AREA:
    case IL_GLOBALAREA:
        return GetView()->CreateAreaImageDefault(GetArea(), alternate_);
    case IL_FILE:
        return GetProject()->Files().Get(id_)->CreateImage();
    case IL_ASSORTEDFILE:
        return GetProject()->AssortedFiles().Get(id_)->CreateImage();
    default:
        return NULL;
    }
}




wxString ButcherProjectFileLink::GetFilename(const wxString &basepath) const
{
    //wxFileName tmp;

    switch (linktype_)
    {
    case IL_AREA:
    case IL_GLOBALAREA:
        return GetImageInfo()->GetOutputFilename(basepath, id_);
    case IL_FILE:
        return GetProject()->Files().Get(id_)->GetOutputFilename(basepath);
    case IL_ASSORTEDFILE:
        return GetProject()->AssortedFiles().Get(id_)->GetOutputFilename(basepath);
    default:
        return wxEmptyString;
    }
    return wxEmptyString;
}

wxString ButcherProjectFileLink::GetFileURL() const
{
    switch (linktype_)
    {
    case IL_AREA:
    case IL_GLOBALAREA:
        return GetImageInfo()->GetOutputFileURL(id_);
    case IL_FILE:
        return GetProject()->Files().Get(id_)->GetOutputFileURL();
    case IL_ASSORTEDFILE:
        return GetProject()->AssortedFiles().Get(id_)->GetOutputFileURL();
    default:
        return wxEmptyString;
    }
    return wxEmptyString;
}





wxString ButcherProjectFileLink::GetBURL() const
{
    wxString ret=wxT("burl:");
    switch (linktype_)
    {
    case IL_NONE:
        return wxEmptyString;
    case IL_AREA:
    case IL_GLOBALAREA:
        if (linktype_==IL_AREA)
            ret+=wxT("area");
        else
            ret+=wxT("garea");
        ret+=wxString::Format(wxT("@%d"), id_);
        for (arealist_t::const_iterator i=arealist_.begin(); i!=arealist_.end(); i++)
        {
            ret+=wxString::Format(wxT("/%d"), *i);
        }
        if (alternate_>=0)
            ret+=wxString::Format(wxT("#%d"), alternate_);
        break;
    case IL_FILE:
        ret+=wxString::Format(wxT("file@%d"), id_);
        break;
    case IL_ASSORTEDFILE:
        ret+=wxString::Format(wxT("assortedfile@%d"), id_);
        break;
    default:
        break;
    }
    return ret;
}




void ButcherProjectFileLink::SetBURL(const wxString &burl)
{
    ButcherProjectBaseAutoUpdate upd(this);

    SetNone();
    if (burl.IsEmpty()) return;

    wxStringTokenizer burltok(burl, wxT(":@/#"));
    wxString t;
    linktype_t ltype=IL_NONE;
    BLID_t bid=0;
    int ct=0;
    bool isalternate=false;
    while (burltok.HasMoreTokens())
    {
        t=burltok.GetNextToken();
        switch (ct)
        {
        case 0: // burl:
            if (/*t!=wxT("burl") || */burltok.GetLastDelimiter()!=wxString(wxT(":")))
                throw ButcherException(_("Invalid BURL (0)"));
            break;
        case 1: // linktype_;
            if (t==wxT("area"))
                ltype=IL_AREA;
            else if (t==wxT("garea"))
                ltype=IL_GLOBALAREA;
            else if (t==wxT("file"))
                ltype=IL_FILE;
            else if (t==wxT("assortedfile"))
                ltype=IL_ASSORTEDFILE;
            else
                throw ButcherException(_("Invalid BURL (1)"));
            break;
        default: // linktype dependent
            switch(ltype)
            {
            case IL_AREA:
            case IL_GLOBALAREA:
                if (ct==2)
                {
                    // view id
                    if (!t.ToULong(&bid))
                        throw ButcherException(_("Invalid BURL (A2)"));
                    id_=bid;
                }
                else
                {
                    if (!isalternate)
                    {
                        // area id
                        if (!t.ToULong(&bid))
                            throw ButcherException(_("Invalid BURL (A3)"));
                        arealist_.push_back(bid);
                    }
                    else
                    {
                        // alternate file
                        if (alternate_!=-1)
                            throw ButcherException(_("Invalid BURL (A5)"));

                        if (!t.ToULong(&bid))
                            throw ButcherException(_("Invalid BURL (A4)"));
                        alternate_=bid;
                    }
                    if (burltok.GetLastDelimiter()==wxString(wxT("#")))
                        isalternate=true;
                }
                break;
            case IL_FILE: // file id
                if (!t.ToULong(&bid))
                    throw ButcherException(_("Invalid BURL (F2)"));
                SetFile(bid);
                return;
            case IL_ASSORTEDFILE: // assorted file id
                if (!t.ToULong(&bid))
                    throw ButcherException(_("Invalid BURL (A2)"));
                SetAssortedFile(bid);
                return;
            default:
                break; // should not happen
            }
            break;
        }
        ct++;
    }

    if ((ltype==IL_AREA||ltype==IL_GLOBALAREA) && arealist_.size()>0)
    {
        linktype_=ltype;
    }


    if (linktype_==IL_NONE)
    {
        SetNone();
        throw ButcherException(_("Invalid BURL (E)"));
    }
    ProjectModified();
}



void ButcherProjectFileLink::SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2)
{
    if (parent_)
        parent_->ProjectModified();
/*
    else
        ButcherProjectBaseModify::SendProjectModified(mod, id);
*/
}




wxString ButcherProjectFileLink::GetDescription() const
{
    if (linktype_!=IL_NONE && !IsValid()) return wxString::Format(_("INVALID LINK: %s"), GetBURL().c_str());

    wxString ret=wxEmptyString;
    switch (linktype_)
    {
    case IL_NONE:
        ret+=_("Empty Link");
        break;
    case IL_AREA:
    case IL_GLOBALAREA:
        {
            if (linktype_==IL_AREA)
                ret=ret+_("Area")+wxT(" ");
            else
                ret=ret+_("Global Area")+wxT(" ");

            ret+=GetView()->GetName();

            ButcherProjectArea::areaclass_t aclass=(linktype_==IL_AREA?ButcherProjectArea::AC_DEFAULT:ButcherProjectArea::AC_GLOBAL);
            ButcherProjectMaskContainer *container=GetView()->GetMask()->Container(aclass);

            ButcherProjectArea *curarea=NULL;

            for (arealist_t::const_iterator i=arealist_.begin(); i!=arealist_.end(); i++)
            {
                //if (!container->Areas().Exists(*i))
                    //throw ButcherException(_("Linked area not found"));
                curarea=container->Areas().Get(*i);
                ret+=wxString::Format(wxT(" -> %s [%s]"), curarea->GetDescription().c_str(),
                    curarea->Configs().Get(GetView()->GetBLId())->GetFlagsDescription().c_str());
                container=curarea->Configs().Get(GetView()->GetBLId())->GetInnerMask()->Container(ButcherProjectArea::AC_DEFAULT);
            }
            if (alternate_>=0)
                ret+=wxT("->")+wxString::Format(_("Alternate %d"), alternate_+1);
        }
        break;
    case IL_FILE:
        ret+=wxString::Format(_("File #%d (%s)"), id_, GetProject()->Files()[id_]->GetDisplayName().c_str());
        break;
    case IL_ASSORTEDFILE:
        ret+=wxString::Format(_("Assorted File #%d (%s)"), id_, GetProject()->AssortedFiles()[id_]->GetPathFile().c_str());
        break;
    default:
        ret+=_("Unknown link");
        break;
    }
    return ret;
}


