//=============================================================================
/**
 *  @file    ButcherViewEditor.cpp
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-02
 */
//=============================================================================

#include "ButcherViewEditor.h"
#include "ButcherProject.h"
#include "ButcherDocument.h"
#include "ButcherProjectMaskDrawSelection.h"

#include "wxpopupmenu.h"



/////////////////////////////////
// CLASS
//      ButcherViewEditor
/////////////////////////////////

BEGIN_EVENT_TABLE(ButcherViewEditor, ButcherView)
    EVT_BUTCHERDOCUMENTMOUSE(wxID_ANY, ButcherViewEditor::OnDocumentMouse)
    EVT_BUTCHERDOCUMENTKEY(wxID_ANY, ButcherViewEditor::OnDocumentKey)
    EVT_BUTCHERVIEWSELECT(wxID_ANY, ButcherViewEditor::OnBViewSelect)
END_EVENT_TABLE()

ButcherViewEditor::ButcherViewEditor(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style,
    const wxString& name) :
    ButcherView(parent, id, pos, size, style, name),
    zoom_(100), rulersize_(26), designwidth_(200), designheight_(200),
    showrulers_(true), showgrid_(false), showpreview_(false), showborders_(true),
    showareasglobal_(true), showareas_(true),
    gridsize_(25), project_(NULL), projectviewid_(0),
    mode_(MODE_DEFAULT), defaultmode_(MODE_DEFAULT), operation_(OP_NONE),
    lineshover_(NULL), areashover_(NULL), band_(-1, -1, -1, -1),
    areaview_(AV_AREA|AV_AREAGLOBAL|AV_AREAMAP)
{
    designer_->Connect(wxID_ANY, wxEVT_LEAVE_WINDOW, wxMouseEventHandler(ButcherViewEditor::OnDWindowLeave), NULL, this);

    RepositionView();
}




ButcherViewEditor::~ButcherViewEditor()
{
    if (project_) {
        project_->Disconnect(wxID_ANY, wxEVT_BUTCHERPROJECT_ACTION,
            ButcherProjectEventHandler(ButcherViewEditor::OnProjectEvent),
            NULL, this);
        project_=NULL;
    }
    if (lineshover_)
        delete lineshover_;
    if (areashover_)
        delete areashover_;
}




void ButcherViewEditor::OnDWindowLeave(wxMouseEvent &event)
{
    DoHoverClear();
    //Refresh();
    event.Skip();
}




void ButcherViewEditor::OnProjectEvent(ButcherProjectEvent& event)
{
    //wxMessageBox(wxT("Project event"), wxT("Event"), wxOK | wxICON_INFORMATION);

    switch (event.GetEvent()) {
    case ButcherProjectEvent::BPE_FILEMODIFIED:
        if (GetProjectView()!=NULL && (event.GetEId() == 0 || event.GetEId() == GetProjectView()->GetFileId()))
        {
            Refresh();
            DoChangedEvent(ButcherViewChangedEvent::VC_VIEW);
        }
        break;
    case ButcherProjectEvent::BPE_MASKMODIFIED:
        if (GetProjectView()!=NULL && (event.GetEId() == 0 || event.GetEId() == GetProjectView()->GetMaskId()))
        {
            SetDesignSize(GetProjectView()->GetMask()->GetWidth(),
                GetProjectView()->GetMask()->GetHeight());
            // clear hover (can contain deleted lines/areas)
            //DoHoverClear();
            SelectionClear();
            //Refresh();
        }
        break;
    case ButcherProjectEvent::BPE_VIEWMODIFIED:
        if (GetProjectView()!=NULL && (event.GetEId() == 0 || event.GetEId() == projectviewid_))
        {
            SetDesignSize(GetProjectView()->GetMask()->GetWidth(),
                GetProjectView()->GetMask()->GetHeight());
            //DoHoverClear();
            SelectionClear();
            //Refresh();

            DoChangedEvent(ButcherViewChangedEvent::VC_VIEW);
        }
        break;
    case ButcherProjectEvent::BPE_VIEWDELETED:
        if (projectviewid_ && (event.GetEId() == 0 || event.GetEId() == projectviewid_)) {
            SetProjectViewId(0);
        }
        break;
    case ButcherProjectEvent::BPE_PROJECTCLOSE:
        //SetProject(NULL);
        SetProjectViewId(0);
        break;
    default:
        break;
    }
    event.Skip(); // IMPORTANT!
}





void ButcherViewEditor::OnDocumentMouse(ButcherDocumentMouseEvent& event)
{
    if (event.GetOriginEventType() == wxEVT_LEFT_DOWN) {
        DoModeEvent(ME_MOUSELDOWN, event.GetX(), event.GetY());
    } else if (event.GetOriginEventType() == wxEVT_RIGHT_DOWN) {
        DoModeEvent(ME_MOUSERDOWN, event.GetX(), event.GetY());
    } else if (event.GetOriginEventType() == wxEVT_MIDDLE_DOWN) {
        DoModeEvent(ME_MOUSEMDOWN, event.GetX(), event.GetY());
    } else if (event.GetOriginEventType() == wxEVT_MOTION) {
        if (event.LeftIsDown())
            DoModeEvent(ME_MOUSELMOVE, event.GetX(), event.GetY());
        else
            DoModeEvent(ME_MOUSEHOVER, event.GetX(), event.GetY());
    }

    RulerPosition(event.GetX(), event.GetY());
    DoChangedEvent(ButcherViewChangedEvent::VC_MOVE, event.GetX(), event.GetY());

    event.Skip();
}



void ButcherViewEditor::OnDocumentKey(ButcherDocumentKeyEvent& event)
{
    if (event.GetOriginEventType() == wxEVT_KEY_DOWN) {
        switch (event.GetKeyCode()) {
        case WXK_ESCAPE:
            if (GetOperation()!=OP_NONE)
                StopOperation();
            break;
        case WXK_ADD:
        case WXK_NUMPAD_ADD:
        case '+':
            SetZoom(GetZoom()+10);
            break;
        case WXK_SUBTRACT:
        case WXK_NUMPAD_SUBTRACT:
        case '-':
            SetZoom(GetZoom()-10);
            break;
        case WXK_LEFT:
        case WXK_NUMPAD_LEFT:
            {
                int x, y;
                dwindow_->GetViewStart(&x, &y);
                dwindow_->Scroll(x-1, -1);

            }

            break;
        case WXK_RIGHT:
        case WXK_NUMPAD_RIGHT:
            {
                int x, y;
                dwindow_->GetViewStart(&x, &y);
                dwindow_->Scroll(x+1, -1);

            }
            break;
        case WXK_UP:
        case WXK_NUMPAD_UP:
            {
                int x, y;
                dwindow_->GetViewStart(&x, &y);
                dwindow_->Scroll(-1, y-1);

            }

            break;
        case WXK_DOWN:
        case WXK_NUMPAD_DOWN:
            {
                int x, y;
                dwindow_->GetViewStart(&x, &y);
                dwindow_->Scroll(-1, y+1);

            }
            break;
        default:
            break;
        }
    }
    event.Skip();
}




void ButcherViewEditor::OnBViewSelect(ButcherViewSelectEvent& event)
{
	bool evtfwd=false; // if in operation, only forward hover events
	if (event.GetSelect() == ButcherViewSelectEvent::SEL_LINEHOVER) {
        // DRAW SELECTED LINES
        DoDrawHover(event.GetLineSelect());
		evtfwd=true;
    } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_AREAHOVER) {
        // DRAW SELECTED AREAS
        DoDrawHover(event.GetAreaSelect());
		evtfwd=true;
    } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_NONE) {
        // DRAW SELECTED NONE
        DoDrawHoverClear();
		evtfwd=true;
    }
    if (operation_==OP_NONE || evtfwd )
        event.Skip();

    switch (operation_) {
        // INSERT HORIZONTAL/VERTICAL LINE
        case OP_INSERTHLINE:
        case OP_INSERTVLINE: {
            // inverse orientation
            ButcherProjectLine::orientation_t orientation=(operation_==OP_INSERTHLINE?ButcherProjectLine::BPL_VERTICAL:ButcherProjectLine::BPL_HORIZONTAL);

            if (event.GetSelect() == ButcherViewSelectEvent::SEL_LINE ||
                event.GetSelect() == ButcherViewSelectEvent::SEL_DEFLINE) {
                if (operationid_>0) {
                    // second line selected
                    if (operationline_)
                    {
                        ButcherProjectLine *selline;
                        if (event.GetSelect() == ButcherViewSelectEvent::SEL_DEFLINE)
                            selline=operationline_->GetContainerJ()->GetEnd(orientation);
                        else
                            // ask user for line
                            selline=DoMenuSelectLine(event.GetLineSelect());
                        if (selline && (operationline_->GetContainer() == selline->GetContainer()))
                        {

                            if (orientation==ButcherProjectLine::BPL_VERTICAL)
                                operationpos_=event.GetY()-operationline_->GetMask()->GetDrawStartY();
                            else
                                operationpos_=event.GetX()-operationline_->GetMask()->GetDrawStartX();

                            // insert line
                            operationline_->GetContainer()->AddLine(
                                operationline_,
                                selline,
                                ButcherProjectLine::InvOrientation(orientation),
                                operationpos_);
                            StopOperation();
                        }
                    }
                } else {
                    // first line selected
                    if (event.GetSelect() == ButcherViewSelectEvent::SEL_DEFLINE)
                        operationline_=GetProjectView()->GetMask()->Areas()->GetStart(orientation);
                    else
                        // ask user for line
                        operationline_=DoMenuSelectLine(event.GetLineSelect());

                    if (operationline_)
                    {
						if (operationline_->GetContainerJ()==NULL)
							throw ButcherException(_("Lines can only be added in masks - convert to mask first"));

                        operationid_=1;
                        if (orientation==ButcherProjectLine::BPL_VERTICAL)
                        {
                            SetMode(MODE_SELECTVLINE_OPERATION);
                            DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select ending vertical line"));
                        }
                        else
                        {
                            SetMode(MODE_SELECTHLINE_OPERATION);
                            DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select ending horizontal line"));
                        }
                    }
                }
            }
            else if (event.GetSelect() == ButcherViewSelectEvent::SEL_LINEHOVER ||
                    event.GetSelect() == ButcherViewSelectEvent::SEL_NONE)
            {
                if (operationid_>0)
                {
                    // draw rubber band
                    if (operationline_)
                    {
                        if (operationline_->GetMask()->GlobalToMask(ButcherProjectLine::BPL_VERTICAL, event.GetX())>-1 &&
                            operationline_->GetMask()->GlobalToMask(ButcherProjectLine::BPL_HORIZONTAL, event.GetY())>-1)
                        {
                            wxRect dl(0, 0, 0, 0);

                            if (operationline_->GetInvOrientation()==ButcherProjectLine::BPL_VERTICAL)
                            {
                                dl.SetY(operationline_->GetPosition()+operationline_->GetMask()->GetDrawStartY());
                                dl.SetHeight(event.GetY()-dl.GetY());
                                dl.SetX(event.GetX());
                                dl.SetWidth(1);
                            }
                            else
                            {
                                dl.SetX(operationline_->GetPosition()+operationline_->GetMask()->GetDrawStartX());
                                dl.SetWidth(event.GetX()-dl.GetX());
                                dl.SetY(event.GetY());
                                dl.SetHeight(1);
                            }


                            DoDrawBand(PosToClient(dl));
                        }
                    }
                }
            }

        }
        break;
        // DELETE LINE
        case OP_DELETELINE: {
            if (event.GetSelect() == ButcherViewSelectEvent::SEL_LINE) {
                ButcherProjectLine *delline=DoMenuSelectLine(event.GetLineSelect());
                if (delline)
                {
                    if (!delline->CanEdit())
                        throw ButcherException(_("Cannot delete this line"));

                    delline->GetContainer()->DeleteLine(delline);
                    StopOperation();
                }
            }
        }
        break;
        // MOVE LINE
        case OP_MOVELINE: {
            if (event.GetSelect() == ButcherViewSelectEvent::SEL_POSITION) {
                if (operationid_>0 && operationline_) {
                    // move line
                    long pos=(operationline_->GetOrientation()==ButcherProjectLine::BPL_HORIZONTAL?
                        event.GetY()-operationline_->GetMask()->GetDrawStartY():
                        event.GetX()-operationline_->GetMask()->GetDrawStartX());

                    if (operationline_->CanMove(pos)) {
                        operationline_->Move(pos);
                        StopOperation();
                    } else
                        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Cannot move to this position"));
                } else {
                    SetMode(MODE_SELECTLINE);
                }
            } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_LINE) {
                operationline_=DoMenuSelectLine(event.GetLineSelect());
                if (operationline_)
                {
                    if (!operationline_->IsMoveable())
                        throw ButcherException(_("Cannot move this line"));

                    operationid_=1;
                    SetMode(MODE_SELECTPOSITION);
                    DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select destination position"));
                }
            } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_HOVER) {
                // draw rubber band
                if (operationid_>0 && operationline_) {
                    wxRect mr(event.GetX(), event.GetY(), 1, 1);

                    if (operationline_->GetOrientation()==ButcherProjectLine::BPL_HORIZONTAL)
                    {
                        mr.SetX(operationline_->GetStart()->GetGlobalPosition());
                        mr.SetWidth(operationline_->GetSize());
                    }
                    else
                    {
                        mr.SetY(operationline_->GetStart()->GetGlobalPosition());
                        mr.SetHeight(operationline_->GetSize());
                    }

                    DoDrawBand(mr);
                }
            }

        }
        break;
        // INSERT AREA GLOBAL
        case OP_INSERTAREAGLOBAL: {
            if (event.GetSelect() == ButcherViewSelectEvent::SEL_POSITION) {
                if (operationid_>0) {
                    ButcherProjectArea *newarea=GetProjectView()->GetMask()->AreasGlobal()->AddArea(
                        operationpt_.x, operationpt_.y, event.GetX(), event.GetY());
					newarea->Configs().Add(GetProjectView()->GetBLId());
                    StopOperation();
                } else {
                    operationid_=1;
                    operationpt_=wxPoint(event.GetX(), event.GetY());
                    SetMode(MODE_SELECTPOSITION);
                    DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select end position"));
                }
            } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_HOVER ||
                        event.GetSelect() == ButcherViewSelectEvent::SEL_NONE) {
                // draw rubber band
                if (operationid_>0) {
                    wxRect mr(operationpt_, wxPoint(event.GetX(), event.GetY()));

                    DoDrawBand(PosToClient(mr));
                }
            }
        }
        break;
        // DELETE AREA GLOBAL
        case OP_DELETEAREAGLOBAL: {
            if (event.GetSelect() == ButcherViewSelectEvent::SEL_AREA) {
                ButcherProjectArea *selarea=DoMenuSelectArea(event.GetAreaSelect());
                if (selarea)
                {
                    GetProjectView()->GetMask()->AreasGlobal()->DeleteArea(selarea);
                    StopOperation();
                }
            }
        }
        break;
        // INSERT AREA MAP
        case OP_INSERTAREAMAP: {
            if (event.GetSelect() == ButcherViewSelectEvent::SEL_AREA) {
                if (operationid_==0) {
                    // only NORMAL areas of kind AK_IMAGE allowed
                    ButcherProjectArea *selarea=DoFindAreaMapSelectArea(event.GetAreaSelect());
                    if (selarea)
                    {
                        operationid_=1;
                        operationpt_=wxPoint(event.GetX(), event.GetY());
                        operationarea_=selarea;
                        SetMode(MODE_SELECTPOSITION);
                        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select end position"));
                    }
                    else
                    {
                        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("No compatible area selected"));
                    }
                }
            } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_POSITION) {
                if (operationid_>0) {
                    wxRect maprect(operationarea_->Configs().Get(GetProjectView()->GetBLId())->GetInnerMask()->GlobalToMask(operationpt_),
                        operationarea_->Configs().Get(GetProjectView()->GetBLId())->GetInnerMask()->GlobalToMask(wxPoint(event.GetX(), event.GetY())));
                    if (operationarea_->Select(operationarea_->GetMask()->GlobalToMask(wxPoint(event.GetX(), event.GetY()))))
                    {
                        operationarea_->Configs().Get(GetProjectView()->GetBLId())->SetHaveMap(true);
                        operationarea_->Configs().Get(GetProjectView()->GetBLId())->GetInnerMask()->AreasMap()->AddArea(
                            maprect.GetLeft(),
                            maprect.GetTop(),
                            maprect.GetRight(),
                            maprect.GetBottom());

                        StopOperation();
                    }
                    else
                    {
                        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Position must be on the same area"));
                    }
                }
            } else if (event.GetSelect() == ButcherViewSelectEvent::SEL_HOVER ||
                        event.GetSelect() == ButcherViewSelectEvent::SEL_NONE) {
                // draw rubber band
                if (operationid_>0) {
                    wxRect mr(operationpt_, wxPoint(event.GetX(), event.GetY()));

                    DoDrawBand(PosToClient(mr));
                }
            }
        }
        break;
        // DELETE AREA MAP
        case OP_DELETEAREAMAP: {
            if (event.GetSelect() == ButcherViewSelectEvent::SEL_AREA) {
                ButcherProjectArea *selarea=DoMenuSelectArea(event.GetAreaSelect());
                if (selarea)
                {
                    selarea->GetContainer()->DeleteArea(selarea);
                    StopOperation();
                }
            }
        }
        break;
        // DEFAULT
        default:
            break;
    }
    //event.Skip();
}



void ButcherViewEditor::DoModeEvent(modeevent_t me, long x, long y)
{
    if (me==ME_MOUSEHOVER)
        DoSelectEvent(ButcherViewSelectEvent::SEL_HOVER, x, y);

    switch (GetMode()) {
    case MODE_SELECTPOSITION:
        if (me==ME_MOUSELDOWN)
            DoSelectEvent(ButcherViewSelectEvent::SEL_POSITION, x, y);
        break;
    case MODE_SELECTLINE:
    case MODE_SELECTHLINE:
    case MODE_SELECTVLINE:
    case MODE_SELECTHLINE_OPERATION:
    case MODE_SELECTVLINE_OPERATION:
        DoModeEvent_SelectLine(me, x, y);
        break;
    case MODE_SELECTAREA:
    case MODE_SELECTAREAGLOBAL:
    case MODE_SELECTAREAMAP:
        DoModeEvent_SelectArea(me, x, y);
        break;
    default:
        break;
    }
}




void ButcherViewEditor::DoModeEvent_SelectLine(modeevent_t me, long x, long y)
{
    if (me==ME_MOUSELDOWN || me==ME_MOUSELMOVE || me == ME_MOUSEHOVER || me==ME_MOUSERDOWN) {
        ButcherProjectMask::findline_t fl=ButcherProjectMask::FL_BOTH;
        switch (GetMode()) {
        case MODE_SELECTHLINE:
        case MODE_SELECTHLINE_OPERATION:
            fl=ButcherProjectMask::FL_HORIZONTAL;
            break;
        case MODE_SELECTVLINE:
        case MODE_SELECTVLINE_OPERATION:
            fl=ButcherProjectMask::FL_VERTICAL;
            break;
        default: break;
        }

        if (GetProjectView()!=NULL) {
            ButcherProjectMask *mask=GetProjectView()->GetMask();
            int maxlevel=-1;
            if (GetMode()==MODE_SELECTHLINE_OPERATION ||
                GetMode()==MODE_SELECTVLINE_OPERATION)
            {
                // select only lines of the same mask
                if (!operationline_) return; // must have previously selected line
                mask=operationline_->GetMask();
                maxlevel=operationline_->GetMask()->GetLevel();
            }

            // locate lines
            ButcherProjectMaskLineSelect select(mask);
            if (mask->FindLine(ButcherView_AreaSelect(GetAreaViewCurrent()),
                fl, x, y, select, GetProjectView()->GetBLId(), maxlevel))
            {
                ButcherViewSelectEvent::select_t sl=ButcherViewSelectEvent::SEL_LINEHOVER;
                if (me==ME_MOUSELDOWN) sl=ButcherViewSelectEvent::SEL_LINE;
                else if (me==ME_MOUSERDOWN) sl=ButcherViewSelectEvent::SEL_LINEEDIT;

                DoSelectEvent(sl, x, y, new ButcherProjectMaskLineSelect(select)); // make copy
            } else {
                DoSelectEvent(ButcherViewSelectEvent::SEL_NONE, x, y);
            }
        }
    } else if (me==ME_MOUSEMDOWN) {
        DoSelectEvent(ButcherViewSelectEvent::SEL_DEFLINE, x, y);
    }
}



void ButcherViewEditor::DoModeEvent_SelectArea(modeevent_t me, long x, long y)
{
    if (me==ME_MOUSELDOWN || me==ME_MOUSELMOVE || me == ME_MOUSEHOVER || me == ME_MOUSERDOWN) {
        if (GetProjectView()!=NULL) {
            ButcherProjectMaskAreaSelect select(GetProjectView()->GetMask());

            ButcherProjectArea::areaselect_t as=ButcherView_AreaSelect(GetAreaViewCurrent());
            if (GetProjectViewShowAreasGlobal() && GetMode()==MODE_SELECTAREAGLOBAL)
                as=ButcherProjectArea::AS_AREAGLOBAL;
            if ((GetAreaView()&AV_AREA)==AV_AREA && GetMode()==MODE_SELECTAREAMAP)
                as=ButcherProjectArea::AS_AREAMAP;

            if (GetProjectView()->GetMask()->FindArea(as,
                x, y, select, GetProjectView()->GetBLId()))
            {
                ButcherViewSelectEvent::select_t sl=ButcherViewSelectEvent::SEL_AREAHOVER;
                if (me==ME_MOUSELDOWN) sl=ButcherViewSelectEvent::SEL_AREA;
                else if (me==ME_MOUSERDOWN) sl=ButcherViewSelectEvent::SEL_AREAEDIT;

                DoSelectEvent(sl, x, y, NULL, new ButcherProjectMaskAreaSelect(select));
            } else {
                DoSelectEvent(ButcherViewSelectEvent::SEL_NONE, x, y);
            }
        }
    }
}




void ButcherViewEditor::DoSelectEvent(ButcherViewSelectEvent::select_t sel,
    long x, long y, ButcherProjectMaskLineSelect *lineselect,
    ButcherProjectMaskAreaSelect *areaselect)
{
    if (GetProjectView()!=NULL) {
        ButcherViewSelectEvent event(this, GetProjectView(), sel, x, y, lineselect, areaselect);
        event.SetEventObject(this);

        ProcessEvent(event);
    }
}




void ButcherViewEditor::DoStatusEvent(ButcherStatusEvent::status_t st, const wxString &message)
{
    ButcherStatusEvent event(st, message);
    event.SetEventObject(this);

    ProcessEvent(event);
}




void ButcherViewEditor::DoOperationEvent()
{
    wxCommandEvent event(wxEVT_BUTCHEROPERATION_ACTION, GetId());
    event.SetEventObject(this);

    ProcessEvent(event);
}




void ButcherViewEditor::StartOperation(ButcherViewEditor::operation_t operation)
{


	DoOperationStart(operation);
    DoOperationEvent();
    Refresh();
}




void ButcherViewEditor::StopOperation()
{
    if (operation_!=OP_NONE) {
        DoClearBand();
        SelectionClear(false);
        SetMode(MODE_DEFAULT);
        operation_=OP_NONE;
        operationline_=NULL;
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, wxEmptyString);
        DoOperationEvent();
        Refresh();
    }
}



void ButcherViewEditor::DoOperationStart(ButcherViewEditor::operation_t operation)
{
    StopOperation();

    SelectionClear();
    operation_=operation;

    switch (operation) {
    case OP_INSERTHLINE:
        SetMode(MODE_SELECTVLINE);
        operationid_=0;
        operationline_=NULL;
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select start vertical line"));
        break;
    case OP_INSERTVLINE:
        SetMode(MODE_SELECTHLINE);
        operationid_=0;
        operationline_=NULL;
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select start horizontal line"));
        break;
    case OP_DELETELINE:
        SetMode(MODE_SELECTLINE);
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select line to delete"));
        break;
    case OP_MOVELINE:
        SetMode(MODE_SELECTLINE);
        operationid_=0;
        operationline_=NULL;
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select line to move"));
        break;
    case OP_INSERTAREAGLOBAL:
        SetMode(MODE_SELECTPOSITION);
        operationid_=0;
        operationpt_=wxPoint(0, 0);
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select start position"));
        break;
    case OP_DELETEAREAGLOBAL:
        SetMode(MODE_SELECTAREAGLOBAL);
        operationid_=0;
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select global area to delete"));
        break;
    case OP_INSERTAREAMAP:
        SetMode(MODE_SELECTAREA);
        operationid_=0;
        operationpt_=wxPoint(0, 0);
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select start position"));
        break;
    case OP_DELETEAREAMAP:
        SetMode(MODE_SELECTAREAMAP);
        operationid_=0;
        DoStatusEvent(ButcherStatusEvent::ST_OPERATION, _("Select map area to delete"));
        break;
    default:
        break;
    }
}




void ButcherViewEditor::SetProject(ButcherProject *project)
{
    if (project==project_) return;

    if (project_)
        project_->Disconnect(wxID_ANY, wxEVT_BUTCHERPROJECT_ACTION,
            ButcherProjectEventHandler(ButcherViewEditor::OnProjectEvent),
            NULL, this);
    project_=project;
    if (project_!=NULL)
        project_->Connect(wxID_ANY, wxEVT_BUTCHERPROJECT_ACTION,
            ButcherProjectEventHandler(ButcherViewEditor::OnProjectEvent),
            NULL, this);
    ProjectChanged();
}




void ButcherViewEditor::SetProjectViewId(BLID_t projectviewid)
{
    if (!project_) return;
    if (projectviewid > 0 && project_->Views().Get(projectviewid)==NULL) return;

    if (projectviewid_!=projectviewid)
        StopOperation();

    projectviewid_=projectviewid;
    ProjectViewChanged();

    if (projectviewid_ > 0) {
        SetDesignSize(GetProjectView()->GetMask()->GetWidth(),
            GetProjectView()->GetMask()->GetHeight());
    } else
        SetDesignSize(0, 0);
    Refresh();

    DoChangedEvent(ButcherViewChangedEvent::VC_VIEW);
}




void ButcherViewEditor::ProjectChanged()
{
    ButcherView::ProjectChanged();

    StopOperation();
    projectviewid_=0;
    ProjectViewChanged();
    Refresh();

    DoChangedEvent(ButcherViewChangedEvent::VC_VIEW);
}



void ButcherViewEditor::ProjectViewChanged()
{
    delete selection_;
    selection_=NULL;

    if (GetProjectView())
    {
        // check if file is loadable
        if (!GetProjectView()->GetFile()->GetImage())
            wxMessageBox(_("Could not load image data"), _("Error"), wxOK|wxICON_ERROR, this);
    }
    SetFileAlternate(false, false);
}




void ButcherViewEditor::SetDesignWidth(unsigned int w)
{
    designwidth_=w;
    RepositionView();
}




void ButcherViewEditor::SetDesignHeight(unsigned int h)
{
    designheight_=h;
    RepositionView();
}




void ButcherViewEditor::SetDesignSize(unsigned int w, unsigned int h)
{
    designwidth_=w;
    designheight_=h;
    RepositionView();
}




void ButcherViewEditor::SetZoom(unsigned short z)
{

	
	zoom_=z;
    RepositionView();
    //Refresh();
}




void ButcherViewEditor::DoDrawBand(wxRect bandrect, bool clear)
{
    wxWindowDC dc(designer_);

    if (!clear)
        DoClearBand();

    InternalDrawBand(dc, bandrect);

    if (!clear)
        band_=bandrect;
}




void ButcherViewEditor::InternalDrawBand(wxDC &dc, wxRect bandrect)
{
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxBLACK_PEN);

    dc.SetLogicalFunction(wxINVERT);
    if (bandrect.GetWidth()==1 || bandrect.GetHeight()==1)
        dc.DrawLine(bandrect.GetTopLeft(), bandrect.GetBottomRight());
    else
        dc.DrawRectangle(bandrect);

    dc.SetBrush(wxNullBrush);
    dc.SetPen(wxNullPen);

    dc.SetLogicalFunction(wxCOPY);
}




void ButcherViewEditor::DoClearBand()
{
    if (band_.GetX()>-1)
        DoDrawBand(band_, true);
    DoResetBand();
}



void ButcherViewEditor::DoResetBand()
{
    band_=wxRect(-1, -1, -1, -1);
}





void ButcherViewEditor::DrawHover(ButcherProjectMaskLineSelect *lineshover,
    ButcherProjectMaskLineSelect *newlineshover, bool hover,
    bool checkredraw)
{
    bool needredraw=false;
    wxWindowDC wdc(designer_);
    //wxBufferedDC dc(&wdc);

    ButcherProjectMask::drawmode_t dm=(hover?ButcherProjectMask::DM_HOVER:ButcherProjectMask::DM_NORMAL);

    for (unsigned long i=0; i<lineshover->GetCount(); i++)
    {
        if (!newlineshover || !newlineshover->Exists(lineshover->GetItem(i)))
        {
            if (hover || GetShowBordersCurrent())
            {
                    lineshover->GetItem(i)->GetMask()->DrawLine(this, &wdc, lineshover->GetItem(i),
                        GetProjectView(), dm);
            }
            else
                needredraw=true;
            if (checkredraw && needredraw)
                break;
        }
    }

    if (checkredraw && needredraw)
    {
        Refresh();
        DrawHover(lineshover, newlineshover, hover, false);
    }
}




void ButcherViewEditor::DrawHover(ButcherProjectMaskAreaSelect *areashover,
    ButcherProjectMaskAreaSelect *newareashover, bool hover, bool checkredraw)
{
#ifdef __WXMAC__
	// On MAC drawing outside the paint event isn't reliable, must repaint every time
	Refresh();
	return;
#else
    bool needredraw=false;
    wxClientDC wdc(designer_);
    //wxBufferedDC dc(&wdc);

    ButcherProjectMask::drawmode_t dm=(hover?ButcherProjectMask::DM_HOVER:ButcherProjectMask::DM_NORMAL);

    for (int i=areashover->GetCount()-1; i>=0; i--)
    {
		//if (i<areashover->GetCount()-1) break;
        if (!newareashover || !newareashover->Exists(areashover->GetItem(i)))
        {
            if (hover || GetShowBordersCurrent())
            {
                needredraw=
                    (!areashover->GetItem(i)->GetMask()->DrawArea(this, &wdc, areashover->GetItem(i),
                        GetProjectView(), dm, NULL, false)) || needredraw;
            }
            else
            {
                // redraw image
                needredraw=true;
            }
            if (checkredraw && needredraw)
                break;
        }
    }

    if (checkredraw && needredraw)
    {
        Refresh();
        DrawHover(areashover, newareashover, hover, false);
    }
#endif
}





void ButcherViewEditor::DoDrawHover(ButcherProjectMaskLineSelect *lineshover)
{
    DoDrawHoverClear(lineshover);
    lineshover_=new ButcherProjectMaskLineSelect(*lineshover);
    DrawHover(lineshover, NULL, true);
    GetSelection()->Set(ButcherProjectMask::DM_HOVER, lineshover_);
}




void ButcherViewEditor::DoDrawHover(ButcherProjectMaskAreaSelect *areashover)
{
    DoDrawHoverClear(areashover);
    areashover_=new ButcherProjectMaskAreaSelect(*areashover);
    DrawHover(areashover, NULL, true);
    GetSelection()->Set(ButcherProjectMask::DM_HOVER, areashover_);
}



void ButcherViewEditor::DoHoverClear()
{
    if (lineshover_)
    {
        delete lineshover_;
        lineshover_=NULL;
    }
    if (areashover_)
    {
        delete areashover_;
        areashover_=NULL;
    }
}




void ButcherViewEditor::DoDrawHoverClear()
{
    DoDrawHoverClear((ButcherProjectMaskLineSelect*)NULL);
    DoDrawHoverClear((ButcherProjectMaskAreaSelect*)NULL);
}




void ButcherViewEditor::DoDrawHoverClear(ButcherProjectMaskLineSelect *lineshover)
{
    if (lineshover_)
    {
        DrawHover(lineshover_, lineshover, false);
        delete lineshover_;
        lineshover_=NULL;
        GetSelection()->Set(ButcherProjectMask::DM_HOVER, lineshover_);
    }

}




void ButcherViewEditor::DoDrawHoverClear(ButcherProjectMaskAreaSelect *areahover)
{
    if (areashover_)
    {
        DrawHover(areashover_, areahover, false);
        delete areashover_;
        areashover_=NULL;
        GetSelection()->Set(ButcherProjectMask::DM_HOVER, areashover_);
    }
}




ButcherProjectLine* ButcherViewEditor::DoMenuSelectLine(ButcherProjectMaskLineSelect *lines)
{
    if (lines->GetCount() == 1)
    {
        return lines->GetItem(0);
    }
    else if (lines->GetCount() > 0)
    {
        wxPopupMenu linemenu(_("Lines"));

        for (unsigned long i=0; i<lines->GetCount(); i++)
        {
            linemenu.Append(wxID_HIGHEST+100+i, lines->GetItem(i)->GetFullDescription());
            //itemmenu->SetEventHandler(&linemenu);
        }

        PopupMenu(&linemenu);
        if (linemenu.GetSelectedId()!=-1)
            return lines->GetItem(linemenu.GetSelectedId()-wxID_HIGHEST-100);
    }
    return NULL;
}




ButcherProjectArea* ButcherViewEditor::DoMenuSelectArea(ButcherProjectMaskAreaSelect *areas)
{
    if (areas->GetCount() == 1)
    {
        return areas->GetItem(0);
    }
    else if (areas->GetCount() > 0)
    {
        wxPopupMenu areamenu(_("Areas"));

        for (unsigned long i=0; i<areas->GetCount(); i++)
        {
            areamenu.Append(wxID_HIGHEST+100+i, areas->GetItem(i)->GetFullDescription());
            //itemmenu->SetEventHandler(&linemenu);
        }

        PopupMenu(&areamenu);
        if (areamenu.GetSelectedId()!=-1)
            return areas->GetItem(areamenu.GetSelectedId()-wxID_HIGHEST-100);
    }
    return NULL;
}




ButcherProjectArea* ButcherViewEditor::DoFindAreaMapSelectArea(ButcherProjectMaskAreaSelect *areas)
{
    // only DEFAULT areas, of kind AK_IMAGE, allowed
    for (unsigned long i=0; i<areas->GetCount(); i++)
    {
        if (areas->GetItem(i)->GetContainedAreaClass()==ButcherProjectArea::AC_DEFAULT &&
            areas->GetItem(i)->Configs().Get(GetProjectView()->GetBLId())->GetAreaKind()==ButcherProjectAreaConfigBase::AK_IMAGE)
            return areas->GetItem(i);
    }
    return NULL;
}



void ButcherViewEditor::SetShowPreview(bool s)
{
    showpreview_=s;

    if (showpreview_ && GetProjectView())
    {
        ButcherProjectBaseAutoProgress progress(GetProjectView(), _("Generating preview, please wait..."),
            ButcherProject::UF_GROUP);

        // generate preview
        if (GetProjectView())
            GetProjectView()->GetMask()->GeneratePreview(ButcherView_AreaSelect(GetAreaViewCurrent()),
                GetProjectView());

        //Update(); // call update to generate preview images
    }

    Refresh();
}





ButcherViewEditor::areaview_t ButcherViewEditor::GetAreaViewCurrent()
{
    areaview_t ret=0;
    if (GetProjectViewShowAreas())
    {
        if (operation_!=OP_NONE || (areaview_&AV_AREA)==AV_AREA) ret|=AV_AREA;
        if (operation_!=OP_NONE || (areaview_&AV_AREAMAP)==AV_AREAMAP) ret|=AV_AREAMAP;
    }
    if (GetProjectViewShowAreasGlobal())
    {
        if (operation_!=OP_NONE || (areaview_&AV_AREAGLOBAL)==AV_AREAGLOBAL) ret|=AV_AREAGLOBAL;
    }
    return ret;
}




bool ButcherViewEditor::GetProjectViewShowAreas()
{
    return project_ && projectviewid_>0 &&
        (((GetProjectView()->GetItems()&ButcherProjectView::ITEM_IMAGES)==ButcherProjectView::ITEM_IMAGES) ||
          ((GetProjectView()->GetItems()&ButcherProjectView::ITEM_IMAGES)==ButcherProjectView::ITEM_HTML));
}




bool ButcherViewEditor::GetProjectViewShowAreasGlobal()
{
    return project_ && projectviewid_>0 &&
        ((GetProjectView()->GetItems()&ButcherProjectView::ITEM_IMAGESGLOBAL)==ButcherProjectView::ITEM_IMAGESGLOBAL);
}




ButcherProjectMaskDrawSelection *ButcherViewEditor::GetSelection()
{
    if (!IsActive()) return NULL;

    if (selection_ && selection_->GetMask() != GetProjectView()->GetMask())
    {
        delete selection_;
        selection_=NULL;
    }
    if (!selection_)
    {
        selection_=new ButcherProjectMaskDrawSelection(GetProjectView()->GetMask());
    }
    return selection_;
}




void ButcherViewEditor::DoAfterDraw(ButcherDocumentDrawEvent& event)
{
    ButcherView::DoAfterDraw(event);
    // redraw rubber band of repaint regions
    if (band_.GetX()>-1)
    {
        if (event.GetUpdateRegion().IsEmpty())
        {
            InternalDrawBand(*event.GetDC(), band_);
            return;
        }

        switch (event.GetUpdateRegion().Contains(band_))
        {
        case wxInRegion:
        case wxPartRegion:
            event.GetDC()->SetClippingRegion(event.GetUpdateRegion());
            InternalDrawBand(*event.GetDC(), band_);
            event.GetDC()->DestroyClippingRegion();
            break;
        default:
            break;
        }
    }
}


