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
        return "jpg";
    case FMT_GIF:
        return "gif";
    case FMT_PNG:
        return "png";
    case FMT_BMP:
        return "bmp";
    case FMT_TIFF:
        return "tif";
    case FMT_XPM:
        return "xpm";
    default:
        return "unknown";
    }
}

ButcherImage::format_t ButcherImage::GetExtFormat(const wxString &ext)
{
    if (ext=="jpg") return FMT_JPG;
    if (ext=="jpeg") return FMT_JPG;
    if (ext=="gif") return FMT_GIF;
    if (ext=="png") return FMT_PNG;
    if (ext=="bmp") return FMT_BMP;
	if (ext=="tif") return FMT_TIFF;
	if (ext=="tiff") return FMT_TIFF;
	if (ext=="xpm") return FMT_XPM;
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


