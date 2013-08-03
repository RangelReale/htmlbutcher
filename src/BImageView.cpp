//=============================================================================
/**
 *  @file    BImageView.cpp
 *
 *  $Id: BImageView.cpp,v 1.8 2008/05/14 21:21:26 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-08
 */
//=============================================================================

#include "BImageView.h"
#include <wx/dcbuffer.h>
#include "wxpopupmenu.h"

#include <wx/colordlg.h>
#include <wx/numdlg.h>
#include <wx/filename.h>



/////////////////////////////////
// CLASS
//      ButcherImageViewColorEvent
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_BUTCHERIMAGEVIEWCOLOR_ACTION )

ButcherImageViewColorEvent::ButcherImageViewColorEvent(const wxColor &color,
    select_t select, const wxPoint &pixel, int id, wxEventType commandType) :
    wxEvent(id, commandType), color_(color), select_(select), pixel_(pixel)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}

wxEvent* ButcherImageViewColorEvent::Clone() const
{
    return new ButcherImageViewColorEvent(*this);
}

/////////////////////////////////
// CLASS
//      ButcherImageViewDrawEvent
/////////////////////////////////



DEFINE_EVENT_TYPE( wxEVT_BUTCHERIMAGEVIEWAFTERPAINT_ACTION )

ButcherImageViewDrawEvent::ButcherImageViewDrawEvent(ButcherImageViewControl *control,
    wxDC *dc, const wxRegion &updateregion, int id, wxEventType commandType) :
    wxEvent(id, commandType), control_(control), dc_(dc), updateregion_(updateregion)
{
    m_propagationLevel=wxEVENT_PROPAGATE_MAX; // force propagation
}

wxEvent* ButcherImageViewDrawEvent::Clone() const
{
    return new ButcherImageViewDrawEvent(*this);
}




/////////////////////////////////
// CLASS
//      ButcherImageViewControl
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_BUTCHERIMAGEVIEWMENU_ACTION )

BEGIN_EVENT_TABLE(ButcherImageViewControl, wxControl)
    EVT_ERASE_BACKGROUND(ButcherImageViewControl::OnErase)
    EVT_PAINT(ButcherImageViewControl::OnPaint)
    EVT_LEFT_DOWN(ButcherImageViewControl::OnMouse)
    EVT_LEFT_DCLICK(ButcherImageViewControl::OnMouse)
    EVT_RIGHT_DOWN(ButcherImageViewControl::OnMouse)
    EVT_MOTION(ButcherImageViewControl::OnMouse)
	EVT_CHILD_FOCUS(ButcherImageViewControl::OnChildFocus)
END_EVENT_TABLE()

ButcherImageViewControl::ButcherImageViewControl(wxWindow* parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size,
    long style, const wxValidator &validator, const wxString& name) :
    wxControl(parent, id, pos, size, style, validator, name),
    image_(NULL), ownsimage_(false), fit_(true), selectcolor_(false),
    zoom_(100), repeat_(1), emptytitle_(wxEmptyString)
{

}

ButcherImageViewControl::~ButcherImageViewControl()
{
    ReleaseImage();
}



void ButcherImageViewControl::ReleaseImage()
{
    if (image_ && ownsimage_)
    {
        delete image_;
    }
    image_=NULL;
    ownsimage_=false;
}

void ButcherImageViewControl::SetImage(ButcherImage *image, bool ownimage)
{
    ReleaseImage();

    image_=image;
    ownsimage_=image_ && ownimage;

    ImageChanged();
}

void ButcherImageViewControl::ImageChanged()
{
    Refresh();
}

wxRect ButcherImageViewControl::GetDrawRect()
{
    wxRect rc(-1, -1, -1, -1);

    if (image_)
    {
        if (fit_)
        {
            float xscale=(float)GetClientSize().GetWidth() / (float)image_->GetWidth();
            float yscale=(float)GetClientSize().GetHeight() / (float)image_->GetHeight();
            float scale=(xscale>yscale?yscale:xscale);

            long rw=(int)(image_->GetWidth()*scale)-1;
            long rh=(int)(image_->GetHeight()*scale)-1;
            if (rw<1) rw=1;
            if (rh<1) rh=1;

            rc.SetLeft((int)(GetClientSize().GetWidth()/2.0)-(int)(rw/2.0));
            rc.SetTop((int)(GetClientSize().GetHeight()/2.0)-(int)(rh/2.0));
            rc.SetRight(rc.GetLeft()+rw);
            rc.SetBottom(rc.GetTop()+rh);

            // calculate repetition, make rect smaller to fit all on one screen
            if (repeat_>1)
            {
                rc.SetLeft(0);
                rc.SetTop(0);
                rc.SetWidth(rc.GetWidth()/repeat_);
                rc.SetHeight(rc.GetHeight()/repeat_);
            }
        }
        else
        {
            rc=image_->GetImageRect();

            rc.SetWidth((int)(rc.GetWidth()*(zoom_/100.0)));
            rc.SetHeight((int)(rc.GetHeight()*(zoom_/100.0)));
        }
    }
    return rc;
}



wxPoint ButcherImageViewControl::GetImagePixel(wxPoint position)
{
    wxPoint ret(-1, -1);
    if (image_)
    {
        wxRect rc=GetDrawRect();
        // as the aspect is always kept, just do a proportional calculation
        if (rc.Contains(position))
        {
            ret.x=static_cast<int>((static_cast<float>(image_->GetWidth())/static_cast<float>(rc.GetWidth()))*(position.x-rc.GetLeft()));
            ret.y=static_cast<int>((static_cast<float>(image_->GetHeight())/static_cast<float>(rc.GetHeight()))*(position.y-rc.GetTop()));
        }
    }
    return ret;
}

wxColor ButcherImageViewControl::GetImageColor(wxPoint imagepixel)
{
    if (image_)
    {
        return image_->GetPixelColor(imagepixel.x, imagepixel.y);
    }
    return *wxBLACK;
}



void ButcherImageViewControl::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(this);
    //dc.SetPen(*wxBLACK_PEN);
    dc.SetPen(*wxTRANSPARENT_PEN);
    //dc.SetBrush(*wxWHITE_BRUSH);
    wxBrush *b=wxTheBrushList->FindOrCreateBrush(GetBackgroundColour());
    dc.SetBrush(*b);

    dc.DrawRectangle(GetSize());

    if (image_)
    {
        wxRect rc=GetDrawRect();
        wxRect dr;
        for (int rx=0; rx<repeat_; rx++)
            for (int ry=0; ry<repeat_; ry++)
            {
                dr=rc;
                dr.SetLeft(dr.GetLeft()+dr.GetWidth()*rx);
                dr.SetTop(dr.GetTop()+dr.GetHeight()*ry);
                image_->Draw(dc, dr);
            }
    }
    else if (!emptytitle_.IsEmpty())
    {
        wxColour colFg(~GetBackgroundColour().Red(), ~GetBackgroundColour().Green(), ~GetBackgroundColour().Blue());
        dc.SetTextForeground(colFg);

        wxSize tsz=dc.GetTextExtent(emptytitle_);
        dc.DrawText(emptytitle_, (GetSize().GetWidth()/2)-(tsz.GetWidth()/2), (GetSize().GetHeight()/2)-(tsz.GetHeight()/2));
    }

    ButcherImageViewDrawEvent cevent(this, &dc, GetUpdateRegion(), GetId());
    cevent.SetEventObject(this);
    ProcessEvent(cevent);

    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);


    event.Skip();
}



void ButcherImageViewControl::OnErase(wxEraseEvent &event)
{
    // do nothing
}

void ButcherImageViewControl::SetSizeFromImage(int left, int top)
{
    if (image_)
    {
        SetSize(left, top, ((int)(image_->GetWidth()*(zoom_/100.0)))*repeat_,
            ((int)(image_->GetHeight()*(zoom_/100.0)))*repeat_);
    }
    else
    {
        SetSize(left, top, 0, 0);
    }
}

void ButcherImageViewControl::OnMouse(wxMouseEvent &event)
{
    if (event.GetEventType()==wxEVT_RIGHT_DOWN)
    {
        wxCommandEvent cevent(wxEVT_BUTCHERIMAGEVIEWMENU_ACTION);
        cevent.SetEventObject(this);
        ProcessEvent(cevent);
    }
    else if (selectcolor_)
    {
        //wxWindowDC dc(this);
        wxColor c;
        wxPoint ip=GetImagePixel(wxPoint(event.GetX(), event.GetY()));
        if (ip.x>-1)
        {
            c=GetImageColor(ip);

            ButcherImageViewColorEvent::select_t sel=ButcherImageViewColorEvent::CS_HOVER;
            if (event.GetEventType()==wxEVT_LEFT_DOWN) sel=ButcherImageViewColorEvent::CS_SELECT;
            if (event.GetEventType()==wxEVT_LEFT_DCLICK) sel=ButcherImageViewColorEvent::CS_CONFIRM;

            ButcherImageViewColorEvent cevent(c, sel, ip);
            cevent.SetEventObject(this);
            ProcessEvent(cevent);

        }
    }

    event.Skip();
}

void ButcherImageViewControl::OnChildFocus(wxChildFocusEvent &event)
{
	// BUGFIX: fix auto scroll bug
	return;
}


void ButcherImageViewControl::SetSelectColor(bool s)
{
    selectcolor_=s;
    if (selectcolor_)
        SetCursor(*wxCROSS_CURSOR);
    else
        SetCursor(*wxSTANDARD_CURSOR);
}


/////////////////////////////////
// CLASS
//      ButcherImageView
/////////////////////////////////

DEFINE_EVENT_TYPE( wxEVT_BUTCHERIMAGEVIEWSAVEFILE_ACTION )

BEGIN_EVENT_TABLE(ButcherImageView, wxControl)
    EVT_SIZE(ButcherImageView::OnSize)
    EVT_BUTCHERIMAGEVIEW_MENU(wxID_ANY, ButcherImageView::OnViewMenu)
    EVT_RIGHT_DOWN(ButcherImageView::OnMouseRight)
END_EVENT_TABLE()



ButcherImageView::ButcherImageView(wxWindow* parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size,
    long style, const wxValidator &validator, const wxString& name) :
    wxControl(parent, id, pos, size, style, validator, name),
    scr_(NULL), fullsize_(false), margin_(0)
{
    view_=new ButcherImageViewControl(this, id, wxDefaultPosition, wxDefaultSize);
    view_->SetFit(true);
}

ButcherImageView::~ButcherImageView()
{
    // view_ and src_ are deleted by parent relationship
}

void ButcherImageView::FullsizeChanged()
{
    if (fullsize_)
    {
        if (!scr_)
        {
            scr_=new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
            scr_->SetScrollRate(10, 10);
            scr_->SetBackgroundColour(GetBackgroundColour());
            scr_->Connect(wxID_ANY, wxEVT_RIGHT_DOWN, wxMouseEventHandler(ButcherImageView::OnMouseRight), NULL, this);
        }
        view_->Reparent(scr_);
        view_->SetFit(false);
        scr_->Show(true);
    }
    else
    {
        if (scr_)
        {
            view_->Reparent(this);
            scr_->Show(false);
            view_->SetFit(true);
        }
    }
    RepositionControls();
}

void ButcherImageView::RepositionControls()
{
    wxRect cr(GetClientRect());
    if (margin_>0)
    {
        cr.Offset(margin_, margin_);
        cr.SetWidth(cr.GetWidth()-(margin_*2));
        cr.SetHeight(cr.GetHeight()-(margin_*2));
    }

    if (fullsize_)
    {
        int sw, sh;
        scr_->CalcUnscrolledPosition(0, 0, &sw, &sh);

        scr_->SetSize(cr);
        view_->SetSizeFromImage(0-sw, 0-sh);
        scr_->SetVirtualSize(view_->GetSize().GetWidth(), view_->GetSize().GetHeight());
    }
    else
        view_->SetSize(cr);
    view_->Refresh();
}



void ButcherImageView::OnSize(wxSizeEvent &event)
{
    RepositionControls();
    event.Skip();
}

void ButcherImageView::SetImage(ButcherImage *image, bool ownimage)
{
    view_->SetImage(image, ownimage);
    RepositionControls();
}

void ButcherImageView::OnViewMenu(wxCommandEvent &event)
{
    ShowMenu();
    event.Skip();
}

void ButcherImageView::ShowMenu()
{
    enum { VM_FULLSIZE, VM_REPEAT, VM_ZOOM50, VM_ZOOM100, VM_ZOOM150, VM_ZOOM200,
        VM_ZOOM300, VM_ZOOM500, VM_ZOOM1000, VM_ZOOMCUSTOM, VM_BGCOLOR, 
		VM_MARGIN, VM_SAVETOFILE };

    wxPopupMenu pm(_("Preview"));

    pm.AppendCheckItem(VM_FULLSIZE, _("View &full size"))->Check(fullsize_);
    pm.AppendCheckItem(VM_REPEAT, _("&Repeat image"))->Check(GetRepeat()>1);
    pm.AppendCheckItem(VM_MARGIN, _("&Margin"))->Check(margin_>0);
    pm.Append(VM_BGCOLOR, _("&Background color"));
    if (fullsize_)
    {
        pm.AppendSeparator();
        pm.AppendCheckItem(VM_ZOOM50, _("50%"))->Check(GetZoom()==50);
        pm.AppendCheckItem(VM_ZOOM100, _("100%"))->Check(GetZoom()==100);
        pm.AppendCheckItem(VM_ZOOM150, _("150%"))->Check(GetZoom()==150);
        pm.AppendCheckItem(VM_ZOOM200, _("200%"))->Check(GetZoom()==200);
        pm.AppendCheckItem(VM_ZOOM300, _("300%"))->Check(GetZoom()==300);
        pm.AppendCheckItem(VM_ZOOM500, _("500%"))->Check(GetZoom()==500);
        pm.AppendCheckItem(VM_ZOOM1000, _("1000%"))->Check(GetZoom()==1000);
        pm.Append(VM_ZOOMCUSTOM, _("&Custom zoom"));
    }
#ifndef HTMLBUTCHER_DEMO
    pm.AppendSeparator();
    pm.Append(VM_SAVETOFILE, _("&Save to file..."));
#endif

    PopupMenu(&pm);
    if (pm.GetSelectedId()!=-1)
    {
        switch (pm.GetSelectedId())
        {
            case VM_FULLSIZE:
                SetFullSize(!GetFullSize());
                break;
            case VM_REPEAT:
                if (GetRepeat()==1)
                    SetRepeat(3);
                else
                    SetRepeat(1);
                break;
            case VM_MARGIN:
                if (GetMargin()==0)
                    SetMargin(10);
                else
                    SetMargin(0);
                break;
            case VM_ZOOM50:
                SetZoom(50);
                break;
            case VM_ZOOM100:
                SetZoom(100);
                break;
            case VM_ZOOM150:
                SetZoom(150);
                break;
            case VM_ZOOM200:
                SetZoom(200);
                break;
            case VM_ZOOM300:
                SetZoom(300);
                break;
            case VM_ZOOM500:
                SetZoom(500);
                break;
            case VM_ZOOM1000:
                SetZoom(1000);
                break;
            case VM_ZOOMCUSTOM:
                {
                    long n=wxGetNumberFromUser(_("Enter zoom value"), _("Zoom (%)"), _("Zoom"), GetZoom(), 1, 1000, this);
                    if (n>0)
                        SetZoom(n);
                }
                break;
            case VM_BGCOLOR:
                {
                    wxColourData cd;
                    cd.SetColour(GetBackgroundColour());
                    wxColourDialog d(this, &cd);
                    if (d.ShowModal()==wxID_OK)
                    {
                        SetBGColor(d.GetColourData().GetColour());
                    }
                }
                break;
#ifndef HTMLBUTCHER_DEMO
            case VM_SAVETOFILE:
                {
                    wxFileDialog d(this, _("Save file"), wxEmptyString, wxEmptyString,
                        _("JPEG image (*.jpg)|*.jpg|GIF image (*.gif)|*.gif|PNG image (*.png)|*.png|BMP image (*.bmp)|*.bmp|TIFF image (*.tif)|*.tif|XPM image (*.xpm)|*.xpm"),
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
                    if (d.ShowModal()==wxID_OK)
                    {
                        wxFileName fn(d.GetPath());
                        GetImage()->Save(ButcherImage::GetExtFormat(fn.GetExt()), d.GetPath(), 0);
                    }
                }
                break;
#endif
        }
    }

}



wxSize ButcherImageView::DoGetBestSize() const
{
    wxSize size = GetMinSize();
    if ( !size.IsFullySpecified() )
    {
        size.SetDefaults(GetSize());
        wxConstCast(this, ButcherImageView)->SetMinSize(size);
    }

    // return as-is, unadjusted by the client size difference.
    return size;
}

void ButcherImageView::OnMouseRight(wxMouseEvent &event)
{
    ShowMenu();
    event.Skip();
}

void ButcherImageView::SetBGColor(const wxColor &bg)
{
    view_->SetBackgroundColour(bg);
    if (scr_)
        scr_->SetBackgroundColour(bg);
    SetBackgroundColour(bg);
    Refresh();
}


