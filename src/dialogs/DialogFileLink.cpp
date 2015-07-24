//=============================================================================
/**
 *  @file    DialogFileLink.cpp
 *
 *  $Id: DialogFileLink.cpp,v 1.8 2008/02/09 00:15:51 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-09
 */
//=============================================================================
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <memory>
#include <wx/statline.h>
#include <wx/valgen.h>
#include <wx/stattext.h>

#include "DialogFileLink.h"



using namespace std;



class FileLinkTreeItemData : public wxTreeItemData
{
public:
    enum itemtype_t { IT_AREA, IT_FILE, IT_ASSORTEDFILE, IT_VIEW, IT_LTAREA,
        IT_LTAREAGLOBAL, IT_SUBAREA, IT_SUBAREAGLOBAL, IT_ALTERNATE,
        IT_AREAM_AREA, IT_AREAM_AREAGLOBAL, IT_AREAM_ALTERNATE };

    FileLinkTreeItemData(itemtype_t itemtype, BLID_t iid = 0, const wxString &burl = wxEmptyString) :
        wxTreeItemData(), itemtype_(itemtype), iid_(iid), burl_(burl), area_(NULL), aid_(-1) {}
    FileLinkTreeItemData(itemtype_t itemtype, ButcherProjectArea *area, BLID_t viewid,
        const wxString &burl = wxEmptyString, int aid = -1) :
        wxTreeItemData(), itemtype_(itemtype), iid_(viewid), burl_(burl), area_(area), aid_(aid) {}

    itemtype_t GetItemType() { return itemtype_; }
    BLID_t GetIID() { return iid_; }
    const wxString &GetBURL() { return burl_; }
    ButcherProjectArea *GetArea() { return area_; }

    bool Compare(FileLinkTreeItemData *source) {
        return itemtype_==source->itemtype_ &&
                iid_==source->iid_ &&
                area_==source->area_ &&
                aid_==source->aid_;
    }
private:
    itemtype_t itemtype_;
    BLID_t iid_;
    wxString burl_;
    ButcherProjectArea *area_;
    int aid_;
};





BEGIN_EVENT_TABLE(HTMLButcherFileLinkDialog, ButcherControl_Dialog)
    EVT_TREE_ITEM_EXPANDING(ID_LINKS, HTMLButcherFileLinkDialog::OnLinksExpanding)
    EVT_TREE_SEL_CHANGED(ID_LINKS, HTMLButcherFileLinkDialog::OnLinksChanged)
    EVT_BUTTON(ID_BTN_NONE, HTMLButcherFileLinkDialog::OnButtonNone)
    EVT_CHECKBOX(ID_PREVIEWENABLE, HTMLButcherFileLinkDialog::OnPreviewEnable)
END_EVENT_TABLE()

HTMLButcherFileLinkDialog::HTMLButcherFileLinkDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style), link_(project), seltypes_(),
    lastburl_(wxEmptyString)
{
    Init();
    CreateControls();
}




void HTMLButcherFileLinkDialog::Init()
{

}




void HTMLButcherFileLinkDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxALL|wxGROW, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);


    // LINK LIST
    wxTreeCtrl *linksctrl = new wxTreeCtrl(this, ID_LINKS, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS);
    bodysizer->Add(linksctrl, 1, wxEXPAND|wxALL, 3);


    // LINK PREVIEW
/*
    wxPanel *previewctrl = new wxPanel(this, ID_PREVIEWPANEL, wxDefaultPosition, wxSize(300, 300));
    bodysizer->Add(previewctrl, 1, wxEXPAND|wxALL, 3);
*/

    wxBoxSizer *previewsizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(previewsizer, 1, wxEXPAND|wxALL, 3);

    // PREVIEW IMAGE
    imageview_ = new ButcherImageView(this, wxID_ANY, wxDefaultPosition, wxSize(300, 300));
    previewsizer->Add(imageview_, 1, wxEXPAND|wxALL, 3);

    // PREVIEW IMAGE FILENAME
    wxStaticText *previewfnctrl = new wxStaticText(this, ID_PREVIEWFILENAME, _("Preview"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    previewsizer->Add(previewfnctrl, 0, wxALL|wxEXPAND, 3);

    // PREVIEW TOGGLE
    wxCheckBox *previewenable = new wxCheckBox(this, ID_PREVIEWENABLE, _("Enable &Preview"));
    previewsizer->Add(previewenable, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);

    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    // BUTTONS
    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // none button
    wxButton* none = new wxButton ( this, ID_BTN_NONE, _("&None"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(none, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);


    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // cancel button
    wxButton* cancel = new wxButton ( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // validators
    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}




bool HTMLButcherFileLinkDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    if ((seltypes_&ST_ALLOWNONE)!=ST_ALLOWNONE)
    {
        FindWindow(ID_BTN_NONE)->Hide();
        GetSizer()->SetSizeHints(this);
    }

    wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);

    linksctrl->AddRoot(_("Links"));
    wxTreeItemId newitem;
    newitem=linksctrl->AppendItem(linksctrl->GetRootItem(), _("Areas"), -1, -1,
        new FileLinkTreeItemData(FileLinkTreeItemData::IT_AREA));
    linksctrl->SetItemHasChildren(newitem, true);
    newitem=linksctrl->AppendItem(linksctrl->GetRootItem(), _("Files"), -1, -1,
        new FileLinkTreeItemData(FileLinkTreeItemData::IT_FILE));
    linksctrl->SetItemHasChildren(newitem, true);
    newitem=linksctrl->AppendItem(linksctrl->GetRootItem(), _("Assorted Files"), -1, -1,
        new FileLinkTreeItemData(FileLinkTreeItemData::IT_ASSORTEDFILE));
    linksctrl->SetItemHasChildren(newitem, true);

    linksctrl->Expand(linksctrl->GetRootItem());

    if (link_.IsValid())
        ExpandLink(link_);

    return true;
}




bool HTMLButcherFileLinkDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);

    wxTreeItemId cid=linksctrl->GetSelection();
    bool isok=cid.IsOk();
    if (isok)
    {
        FileLinkTreeItemData *data=dynamic_cast<FileLinkTreeItemData*>(linksctrl->GetItemData(cid));
        isok=!data->GetBURL().IsEmpty();
        if (isok)
            link_.SetBURL(data->GetBURL());
    }

    if (!isok)
    {
        wxMessageBox(_("No link selected"), _("Error"), wxOK|wxICON_ERROR, this);
        return false;
    }

    return true;
}




void HTMLButcherFileLinkDialog::ExpandItem(wxTreeItemId item)
{
    wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);

    ButcherProjectFileLink tmplink(link_.GetProject());;

    // add items to root items
    wxTreeItemId cid=linksctrl->GetLastChild(item), cid2;
    if (!cid.IsOk())
    {
        //ButcherListIdList::const_iterator idc;
        FileLinkTreeItemData *data=dynamic_cast<FileLinkTreeItemData*>(linksctrl->GetItemData(item));
        if (!data) return;
        //if (data->GetBURL().IsEmpty())
            switch (data->GetItemType())
            {
            // AREAS
            case FileLinkTreeItemData::IT_AREA:

				for (ButcherProjectViews::iterator i=link_.GetProject()->Views().begin(); 
					i!=link_.GetProject()->Views().end(); i++)
				{
                    cid=linksctrl->AppendItem(item, i->GetName(),
                        -1, -1, new FileLinkTreeItemData(FileLinkTreeItemData::IT_VIEW, i.first()));

                    cid2=linksctrl->AppendItem(cid, _("Areas"), -1, -1,
                        new FileLinkTreeItemData(FileLinkTreeItemData::IT_LTAREA, i.first()));
                    linksctrl->SetItemHasChildren(cid2, true);
                    if (i->GetMask()->AreasGlobal() &&
                        i->GetMask()->AreasGlobal()->Areas().Count()>0)
                    {
                        cid2=linksctrl->AppendItem(cid, _("Global Areas"), -1, -1,
                            new FileLinkTreeItemData(FileLinkTreeItemData::IT_LTAREAGLOBAL, i.first()));
                        linksctrl->SetItemHasChildren(cid2, true);
                    }
				}
                break;
            // LTAREAS
            case FileLinkTreeItemData::IT_LTAREA:
                AddSubArea(item, data->GetIID(),
                    link_.GetProject()->Views()[data->GetIID()]->GetMask()->Areas(),
                    false);
                break;
            // LTAREASGLOBAL
            case FileLinkTreeItemData::IT_LTAREAGLOBAL:
                AddSubArea(item, data->GetIID(),
                    link_.GetProject()->Views()[data->GetIID()]->GetMask()->AreasGlobal(),
                    true);
                break;
            // SUBAREAS
            //case FileLinkTreeItemData::IT_SUBAREA:
            //case FileLinkTreeItemData::IT_SUBAREAGLOBAL:
            case FileLinkTreeItemData::IT_AREAM_AREA:
            case FileLinkTreeItemData::IT_AREAM_AREAGLOBAL:
                AddSubArea(item, data->GetIID(),
                    data->GetArea()->Configs().Get(data->GetIID())->GetInnerMask()->Areas(),
                    data->GetItemType()==FileLinkTreeItemData::IT_AREAM_AREAGLOBAL);
                break;

            // FILES
            case FileLinkTreeItemData::IT_FILE:
				for (ButcherProjectFiles::iterator i=link_.GetProject()->Files().begin(); 
					i!=link_.GetProject()->Files().end(); i++)
				{
                    tmplink.SetFile(i.first());

                    linksctrl->AppendItem(item, i->GetDisplayName(),
                        -1, -1, new FileLinkTreeItemData(FileLinkTreeItemData::IT_FILE, i.first(), tmplink.GetBURL()));
				}
                break;
            // ASSORTED FILES
            case FileLinkTreeItemData::IT_ASSORTEDFILE:
				for (ButcherProjectAssortedFiles::iterator i=link_.GetProject()->AssortedFiles().begin(); 
					i!=link_.GetProject()->AssortedFiles().end(); i++)
				{
                    tmplink.SetAssortedFile(i.first());
                    if (((seltypes_&ST_IMAGEONLY)!=ST_IMAGEONLY)||tmplink.IsImage())
                    {
                        linksctrl->AppendItem(item, i->GetPathFile(),
                            -1, -1, new FileLinkTreeItemData(FileLinkTreeItemData::IT_ASSORTEDFILE, i.first(), tmplink.GetBURL()));
                    }
				}
                break;
            default:
                break;
            }
    }
}





void HTMLButcherFileLinkDialog::OnLinksExpanding(wxTreeEvent &event)
{
    //wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);
    ExpandItem(event.GetItem());
    event.Skip();
}



void HTMLButcherFileLinkDialog::AddSubArea(wxTreeItemId parentitem, BLID_t viewid,
    ButcherProjectMaskContainer *container, bool isglobal)
{
    wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);
    ButcherProjectAreaConfig *ac;

    ButcherProjectFileLink tmplink(link_.GetProject());;

    wxTreeItemId cid, cid2;
    wxString desc;

	for (ButcherProjectAreas::iterator i=container->Areas().begin(); i!=container->Areas().end(); i++)
	{
        ac=i->Configs().Get(viewid);

        desc=wxString::Format(wxT("%s [%s]"), i->GetDescription().c_str(),
            ac->GetFlagsDescription().c_str());

        tmplink.SetArea(container->GetProject()->Views().Get(viewid), &*i, -1);

        cid=linksctrl->AppendItem(parentitem, desc,
            -1, -1, new FileLinkTreeItemData(
                (isglobal?FileLinkTreeItemData::IT_SUBAREAGLOBAL:FileLinkTreeItemData::IT_SUBAREA),
                &*i, viewid, tmplink.GetBURL()));

        // AREAS
        if (ac->GetInnerMask()->Areas()!=NULL)
        {
            cid2=linksctrl->AppendItem(cid, _("Areas"), -1, -1,
                new FileLinkTreeItemData(
                    (isglobal?FileLinkTreeItemData::IT_AREAM_AREAGLOBAL:FileLinkTreeItemData::IT_AREAM_AREA),
                    &*i, viewid));
            linksctrl->SetItemHasChildren(cid2, true);
        }
        // ALTERNATE
        if (ac->HaveAlternate())
        {
            cid2=linksctrl->AppendItem(cid, _("Alternate Files"), -1, -1,
                new FileLinkTreeItemData(
                    FileLinkTreeItemData::IT_AREAM_ALTERNATE,
                    &*i, viewid));
            for (int ctalt=0; ctalt<BUTCHERCONST_VIEW_MAXALTERNATE; ctalt++)
                if (ac->AlternateImageInfo(ctalt).GetEnabled())
                {
                    tmplink.SetAreaAlternate(ctalt);
                    linksctrl->AppendItem(cid2, wxString::Format(_("Alternate %d"), ctalt+1), -1, -1,
                        new FileLinkTreeItemData(
                            FileLinkTreeItemData::IT_ALTERNATE,
                            &*i, viewid, tmplink.GetBURL(), ctalt));
                }
        }
	}
}




void HTMLButcherFileLinkDialog::OnLinksChanged(wxTreeEvent &event)
{
    wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);

    FileLinkTreeItemData *data=dynamic_cast<FileLinkTreeItemData*>(linksctrl->GetItemData(event.GetItem()));

    if (data && !data->GetBURL().IsEmpty())
    {
        ButcherProjectFileLink fl(link_.GetProject(), data->GetBURL());
        UpdatePreview(fl);
        SetTitle(wxString::Format(wxT("%s [%s]"), fl.GetDescription().c_str(), data->GetBURL().c_str()));
    }
    else
    {
        UpdatePreview(wxEmptyString);
        SetTitle(_("File Link"));
    }

}



void HTMLButcherFileLinkDialog::ExpandLink(const ButcherProjectFileLink &link)
{
    wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);

    std::unique_ptr<FileLinkTreeItemData> finddata;
    wxTreeItemId cid;

    switch (link.GetLinkType())
    {
    case ButcherProjectFileLink::IL_AREA:
    case ButcherProjectFileLink::IL_GLOBALAREA:
        {
            ButcherProjectArea *lastarea=NULL;
            finddata.reset(new FileLinkTreeItemData(FileLinkTreeItemData::IT_AREA));
            ExpandItem(FindTreeData(finddata.get()));

            finddata.reset(new FileLinkTreeItemData(FileLinkTreeItemData::IT_VIEW, link.GetId()));
            ExpandItem(FindTreeData(finddata.get()));

            finddata.reset(new FileLinkTreeItemData(
                (link.GetLinkType()==ButcherProjectFileLink::IL_AREA?FileLinkTreeItemData::IT_LTAREA:FileLinkTreeItemData::IT_LTAREAGLOBAL),
                link.GetId()));
            ExpandItem(FindTreeData(finddata.get()));

            ButcherProjectFileLink::areas_t alist;
            link.GetAreas(alist);
            for (ButcherProjectFileLink::areas_t::const_iterator i=alist.begin();
                    i!=alist.end();
                    i++)
            {
                finddata.reset(new FileLinkTreeItemData(
                    (link.GetLinkType()==ButcherProjectFileLink::IL_AREA?FileLinkTreeItemData::IT_AREAM_AREA:FileLinkTreeItemData::IT_AREAM_AREAGLOBAL),
                    *i, link.GetId()));
                ExpandItem(FindTreeData(finddata.get()));
                lastarea=*i;
            }

            // select area
            if (link.GetAlternate()==-1)
            {
                finddata.reset(new FileLinkTreeItemData(
                    (link.GetLinkType()==ButcherProjectFileLink::IL_AREA?FileLinkTreeItemData::IT_SUBAREA:FileLinkTreeItemData::IT_SUBAREAGLOBAL),
                    lastarea, link.GetId()));
                //ExpandItem(FindTreeData(finddata.get()), true);
                linksctrl->SelectItem(FindTreeData(finddata.get()));
            } else {
                finddata.reset(new FileLinkTreeItemData(
                    FileLinkTreeItemData::IT_ALTERNATE,
                    lastarea, link.GetId(), wxEmptyString, link.GetAlternate()));
                //ExpandItem(FindTreeData(finddata.get()), true);
                linksctrl->SelectItem(FindTreeData(finddata.get()));
            }
        }
        break;
    case ButcherProjectFileLink::IL_FILE:
        finddata.reset(new FileLinkTreeItemData(FileLinkTreeItemData::IT_FILE));
        ExpandItem(FindTreeData(finddata.get()));
        finddata.reset(new FileLinkTreeItemData(FileLinkTreeItemData::IT_FILE, link.GetId()));
        linksctrl->SelectItem(FindTreeData(finddata.get()));
        break;
    case ButcherProjectFileLink::IL_ASSORTEDFILE:
        finddata.reset(new FileLinkTreeItemData(FileLinkTreeItemData::IT_ASSORTEDFILE));
        ExpandItem(FindTreeData(finddata.get()));
        finddata.reset(new FileLinkTreeItemData(FileLinkTreeItemData::IT_ASSORTEDFILE, link.GetId()));
        linksctrl->SelectItem(FindTreeData(finddata.get()));
        break;
    default:
        break;
    }
}




wxTreeItemId HTMLButcherFileLinkDialog::FindTreeData(FileLinkTreeItemData *data)
{
    wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);
    return FindTreeData(linksctrl->GetRootItem(), data);
}




wxTreeItemId HTMLButcherFileLinkDialog::FindTreeData(wxTreeItemId root, FileLinkTreeItemData *data)
{
    wxTreeCtrl *linksctrl=(wxTreeCtrl*)FindWindow(ID_LINKS);

    FileLinkTreeItemData *curdata;

    wxTreeItemId ret;
    wxTreeItemIdValue cookie;
    ret=linksctrl->GetFirstChild(root, cookie);
    while (ret.IsOk())
    {
        curdata=dynamic_cast<FileLinkTreeItemData*>(linksctrl->GetItemData(ret));
        if (data->Compare(curdata)) return ret;
        if (linksctrl->GetLastChild(ret).IsOk())
        {
            ret=FindTreeData(ret, data);
            if (ret.IsOk()) return ret;
        }
        ret=linksctrl->GetNextChild(root, cookie);
    }
    return ret;
}




void HTMLButcherFileLinkDialog::OnButtonNone(wxCommandEvent &event)
{
    link_.SetNone();
    EndModal(wxID_OK);
}



void HTMLButcherFileLinkDialog::OnPreviewEnable(wxCommandEvent &event)
{
    UpdatePreview(lastburl_);
}




void HTMLButcherFileLinkDialog::UpdatePreview(const wxString &burl)
{
    ButcherProjectFileLink fl(GetProject(), burl);
    UpdatePreview(fl);
}




void HTMLButcherFileLinkDialog::UpdatePreview(const ButcherProjectFileLink &link)
{
    wxCheckBox *previewenablectrl = (wxCheckBox*)FindWindow(ID_PREVIEWENABLE);
    wxStaticText *previewfnctrl = (wxStaticText*)FindWindow(ID_PREVIEWFILENAME);

    if (link.IsValid())
        previewfnctrl->SetLabel(link.GetFilename());
    else
        previewfnctrl->SetLabel(_("Preview"));

    if (previewenablectrl->GetValue() && link.IsValid() && link.IsImage())
    {
        imageview_->SetImage(link.CreateImage());
    }
    else
        imageview_->SetImage(NULL);


    lastburl_=link.GetBURL();
}



