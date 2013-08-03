//=============================================================================
/**
 *  @file    DialogMaskAreaSlice.cpp
 *
 *  $Id: DialogMaskAreaListEdit.cpp,v 1.2 2008/02/25 21:52:32 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-08
 */
//=============================================================================
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/valgen.h>
#include <wx/dcbuffer.h>
#include <wx/radiobox.h>

#include "DialogMaskAreaListEdit.h"
#include "ButcherProjectMaskDrawSelection.h"
#include "BList.h"
#include "BProc.h"





class MaskAreaListTreeItemData : public wxTreeItemData
{
public:
    MaskAreaListTreeItemData(ButcherProjectArea *area) : wxTreeItemData(), area_(area) {}

    ButcherProjectArea *GetArea() { return area_; }
private:
    ButcherProjectArea *area_;
};




BEGIN_EVENT_TABLE(HTMLButcherMaskAreaListDialog, ButcherControl_Dialog)
    EVT_TREE_SEL_CHANGED(ID_AREALIST, HTMLButcherMaskAreaListDialog::OnAreaChanged)
    EVT_BUTCHERIMAGEVIEW_AFTERPAINT(ID_PREVIEW, HTMLButcherMaskAreaListDialog::OnAfterDraw)
END_EVENT_TABLE()

HTMLButcherMaskAreaListDialog::HTMLButcherMaskAreaListDialog( wxWindow* parent,
    wxWindowID id,
    ButcherProject *project,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style) :
    ButcherControl_Dialog(parent, id, project, caption, pos, size, style),
    previewctrl_(NULL), view_(NULL)
{
    Init();
    CreateControls();
}




void HTMLButcherMaskAreaListDialog::Init()
{
#ifdef BUTCHER_USE_HELP
	// HELP
	SetHelpSection(wxT("dialog-currentareas.html"));
#endif
}




void HTMLButcherMaskAreaListDialog::CreateControls()
{
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(boxsizer, 1, wxEXPAND|wxALL, 3);

    // BODY
    wxBoxSizer *bodysizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(bodysizer, 1, wxEXPAND|wxALL, 3);


    // FIELDS
    wxBoxSizer *fieldssizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(fieldssizer, 0, wxEXPAND|wxALL, 3);

    // AREA LIST
    wxTreeCtrl *arealistctrl = new wxTreeCtrl(this, ID_AREALIST, wxDefaultPosition, wxSize(250, -1),
        wxTR_DEFAULT_STYLE);
    arealistctrl->SetToolTip(_("Right-click for area menu"));
    arealistctrl->Connect(ID_AREALIST, wxEVT_RIGHT_DOWN, wxMouseEventHandler(HTMLButcherMaskAreaListDialog::OnAreaMenu),
        NULL, this);
    fieldssizer->Add(arealistctrl, 1, wxEXPAND|wxALL, 3);



    // PREVIEW
    wxBoxSizer *previewsizer = new wxBoxSizer(wxVERTICAL);
    bodysizer->Add(previewsizer, 1, wxEXPAND, 3);

    // preview image
    previewctrl_ = new ButcherControl_ProjectImageView(this, ID_PREVIEW, wxDefaultPosition, wxSize(400, 350));
    previewsizer->Add(previewctrl_, 1, wxEXPAND|wxALL, 3);



    // divider line
    wxStaticLine *line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    boxsizer->Add(line, 0, wxGROW|wxALL, 3);

    wxBoxSizer *buttonsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(buttonsizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);

    // ok button
    wxButton* ok = new wxButton ( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    // cancel button
    wxButton* cancel = new wxButton ( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

#ifdef BUTCHER_USE_HELP
    // help button
    wxButton* help = new wxButton ( this, wxID_HELP, _("&Help"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonsizer->Add(help, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
#endif

    // validators

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    CentreOnScreen();
}




void HTMLButcherMaskAreaListDialog::Load(ButcherProjectView *view)
{
    view_=view;
}



void HTMLButcherMaskAreaListDialog::Save(ButcherProjectView *view)
{
    ButcherProjectBaseAutoUpdate upd(view);
}




bool HTMLButcherMaskAreaListDialog::TransferDataToWindow()
{
    if (!wxDialog::TransferDataToWindow()) return false;

    wxTreeCtrl *arealistctrl=(wxTreeCtrl*)FindWindow(ID_AREALIST);
    arealistctrl->AddRoot(view_->GetName());

    wxTreeItemId cid;
    cid=arealistctrl->AppendItem(arealistctrl->GetRootItem(), _("Areas"));
    AddSubArea(arealistctrl, cid, view_->GetMask()->Areas(), false);

    if (view_->GetMask()->AreasGlobal() &&
        view_->GetMask()->AreasGlobal()->Areas().Count()>0)
    {
        cid=arealistctrl->AppendItem(arealistctrl->GetRootItem(), _("Global Areas"));
        AddSubArea(arealistctrl, cid, view_->GetMask()->AreasGlobal(), true);
    }
    arealistctrl->ExpandAll();

    previewctrl_->SetDesignWidth(view_->GetMask()->GetWidth());
    previewctrl_->SetDesignHeight(view_->GetMask()->GetHeight());
    previewctrl_->SetImage(view_->GetFile()->GetImage(), false);
    return true;
}




bool HTMLButcherMaskAreaListDialog::TransferDataFromWindow()
{
    if (!wxDialog::TransferDataFromWindow()) return false;

    return true;
}



void HTMLButcherMaskAreaListDialog::OnAfterDraw(ButcherImageViewDrawEvent &event)
{
    wxTreeCtrl *arealistctrl=(wxTreeCtrl*)FindWindow(ID_AREALIST);

    ButcherProjectMaskDrawSelection drawsel(view_->GetMask());
    if (arealistctrl->GetSelection().IsOk() &&
        arealistctrl->GetItemData(arealistctrl->GetSelection()))
        drawsel.Add(ButcherProjectMask::DM_SELECTED, static_cast<MaskAreaListTreeItemData*>(arealistctrl->GetItemData(arealistctrl->GetSelection()))->GetArea());

    view_->GetMask()->Draw(previewctrl_, event.GetDC(),
        ButcherProjectArea::AS_ALL, view_, ButcherProjectMask::DM_DEFAULT,
        &drawsel, ButcherProjectMask::DO_DONTDRAWTITLES);

    //event.GetDC()->SetPen(*wxGREEN_PEN);
}




void HTMLButcherMaskAreaListDialog::AddSubArea(wxTreeCtrl *tree, wxTreeItemId parentitem,
    ButcherProjectMaskContainer *container, bool isglobal)
{
    ButcherProjectAreaConfig *ac;

    wxTreeItemId cid;
    wxString desc;

	for (ButcherProjectAreas::iterator i=container->Areas().begin(); i!=container->Areas().end(); i++)
	{
        ac=i->Configs().Get(view_->GetBLId());

/*
        desc=wxString::Format(wxT("%s [%s]"), container->Areas().Get(*i)->GetDescription().c_str(),
            ac->GetFlagsDescription().c_str());
*/

        cid=tree->AppendItem(parentitem, AreaDescription(&*i).c_str(),
            -1, -1, new MaskAreaListTreeItemData(&*i));

        // AREAS
        if (ac->GetInnerMask()->Areas()!=NULL)
        {
            AddSubArea(tree, cid, ac->GetInnerMask()->Areas());
        }
	}
}




void HTMLButcherMaskAreaListDialog::OnAreaChanged(wxTreeEvent &event)
{
    previewctrl_->Refresh();
}




void HTMLButcherMaskAreaListDialog::OnAreaMenu(wxMouseEvent &event)
{
    wxTreeCtrl *arealistctrl=(wxTreeCtrl*)FindWindow(ID_AREALIST);

    if (arealistctrl->GetSelection().IsOk() &&
        arealistctrl->GetItemData(arealistctrl->GetSelection()))
    {
        ButcherProjectMaskAreaSelect sel(view_->GetMask());
        sel.Add(
            static_cast<MaskAreaListTreeItemData*>(arealistctrl->GetItemData(arealistctrl->GetSelection()))->GetArea()->GetMask(),
            static_cast<MaskAreaListTreeItemData*>(arealistctrl->GetItemData(arealistctrl->GetSelection()))->GetArea());
        ButcherProjectProc_AreaMenu areamenu(GetProject(), this, &sel, view_);
        areamenu.ShowAreaMenu();

        arealistctrl->SetItemText(arealistctrl->GetSelection(),
            AreaDescription(static_cast<MaskAreaListTreeItemData*>(arealistctrl->GetItemData(arealistctrl->GetSelection()))->GetArea()));

        Refresh();
    }
}




wxString HTMLButcherMaskAreaListDialog::AreaDescription(ButcherProjectArea *area)
{
    wxString fmtadd(wxEmptyString);
    if (area->Configs().Get(view_->GetBLId())->ImageInfo().IsOutputImage())
        fmtadd=wxString::Format(wxT(" \"%s\""), area->Configs().Get(view_->GetBLId())->ImageInfo().GetOutputFileURL(view_->GetBLId()).c_str());

    return wxString::Format(wxT("%s [%s]%s"), area->GetDescription().c_str(),
        area->Configs().Get(view_->GetBLId())->GetFlagsDescription().c_str(), fmtadd.c_str());
}


