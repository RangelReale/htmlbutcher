//=============================================================================
/**
 *  @file    BImage.cpp
 *
 *  $Id: BImage.cpp,v 1.11 2008/02/04 15:17:58 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-18
 */
//=============================================================================

#include <memory>
#include "BImage.h"

#include "BImage_FreeImage.h"



using namespace std;

/////////////////////////////////
// CLASS
//      ButcherImage
/////////////////////////////////
wxString ButcherImage::GetFormatExt(format_t format)
{
    switch (format)
    {
    case FMT_JPG:
        return wxT("jpg");
    case FMT_GIF:
        return wxT("gif");
    case FMT_PNG:
        return wxT("png");
    case FMT_BMP:
        return wxT("bmp");
    case FMT_TIFF:
        return wxT("tif");
    case FMT_XPM:
        return wxT("xpm");
    default:
        return wxT("unknown");
    }
}

ButcherImage::format_t ButcherImage::GetExtFormat(const wxString &ext)
{
    if (ext==wxT("jpg")) return FMT_JPG;
    if (ext==wxT("jpeg")) return FMT_JPG;
    if (ext==wxT("gif")) return FMT_GIF;
    if (ext==wxT("png")) return FMT_PNG;
    if (ext==wxT("bmp")) return FMT_BMP;
	if (ext==wxT("tif")) return FMT_TIFF;
	if (ext==wxT("tiff")) return FMT_TIFF;
	if (ext==wxT("xpm")) return FMT_XPM;
    throw ButcherException(_("Invalid image file extension"));
}

void ButcherImage::DrawRepeat(wxDC &dc, const wxSize &s, const wxRect &rarea, repeat_t repeat)
{
    dc.SetClippingRegion(rarea);

    if (repeat == DR_NONE)
    {
        Draw(dc, wxRect(rarea.GetTopLeft(), s));
    }
    else
    {
        wxRect darea(rarea.GetTopLeft(), s);
        while (darea.GetLeft()<rarea.GetRight() && darea.GetTop()<rarea.GetBottom())
        {
            Draw(dc, darea);
            if (repeat!=DR_VERTICAL)
            {
                darea.SetLeft(darea.GetLeft()+s.GetWidth());
                if (darea.GetLeft()>=rarea.GetRight())
                {
                    if (repeat==DR_ALL)
                    {
                        darea.SetLeft(rarea.GetLeft());
                        darea.SetTop(darea.GetTop()+s.GetHeight());
                    }
                }
            }
            else
            {
                darea.SetLeft(rarea.GetLeft());
                darea.SetTop(darea.GetTop()+s.GetHeight());
            }
        }
    }

    dc.DestroyClippingRegion();
}

/////////////////////////////////
// CLASS
//      ButcherImageFactory
/////////////////////////////////

ButcherImage* ButcherImageFactory::Load(const wxString &filename)
{
    ButcherImage* ret;

    ret=new ButcherImage_FreeImage();
    if (ret->CanLoad(filename, true)) return ret;
    delete ret;

    return NULL;
}



ButcherImage* ButcherImageFactory::Load(wxInputStream &stream)
{
    ButcherImage* ret;

    ret=new ButcherImage_FreeImage();
	try 
	{
		if (ret->CanLoad(stream, true)) return ret;
	} catch (...) {
		delete ret;
		throw;
	}
    delete ret;

    return NULL;
}

ButcherImage* ButcherImageFactory::Load(const wxImage &image)
{
    ButcherImage* ret;

    ret=new ButcherImage_FreeImage();
    if (dynamic_cast<ButcherImage_FreeImage*>(ret)->CanLoad(image, true)) return ret;
    delete ret;

    return NULL;
}


