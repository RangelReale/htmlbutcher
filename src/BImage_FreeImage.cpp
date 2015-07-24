//=============================================================================
/**
 *  @file    BImage_FreeImage.cpp
 *
 *  $Id: BImage_FreeImage.cpp,v 1.11 2008/02/07 20:27:27 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-24
 */
//=============================================================================
#include <memory>
#include "BImage_FreeImage.h"
#include <wx/mstream.h>



using namespace std;

/////////////////////////////////
// CLASS
//      ButcherImage_FreeImage
/////////////////////////////////



ButcherImage_FreeImage::ButcherImage_FreeImage() :
    ButcherImage(), filesize_(0)
{
    image_=new wxFreeImage();
}

ButcherImage_FreeImage::ButcherImage_FreeImage(wxFreeImage *img, unsigned long filesize) :
    ButcherImage(), image_(img), filesize_(filesize)
{

}

ButcherImage_FreeImage::~ButcherImage_FreeImage()
{
    if (image_) delete image_;
}


long ButcherImage_FreeImage::GetWidth()
{
    return image_->GetWidth();
}

long ButcherImage_FreeImage::GetHeight()
{
    return image_->GetHeight();
}



bool ButcherImage_FreeImage::CanLoad(const wxString &filename, bool doload)
{
    try
    {
        wxFreeImage *imgtest=new wxFreeImage;
        bool ret=imgtest->Load(filename);
        if (ret && doload)
        {
            if (image_) delete image_;
            image_=imgtest;
        }
        else
            delete imgtest;
        return ret;

    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
        return false;
    }
}

void ButcherImage_FreeImage::Load(const wxString &filename)
{
    try
    {
        if (!image_->Load(filename))
            throw ButcherException(_("Error loading image"));
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
    }
}

bool ButcherImage_FreeImage::CanLoad(wxInputStream &stream, bool doload)
{
	wxFreeImage *imgtest=new wxFreeImage;
	try
	{
		//ButcherImage_FreeImage_wxStreamIO sio(stream);
		//bool ret=imgtest->loadFromHandle(sio.GetIO(), &sio);
		bool ret=imgtest->Load(stream);
		if (ret && doload)
		{
			if (image_) delete image_;
			image_=imgtest;
		}
		else
			delete imgtest;
		return ret;
	} catch (wxFreeImageException &e) {
		delete imgtest;
		throw ButcherException(e.what());
	} catch (...) {
		delete imgtest;
		throw ButcherException(_("Error loading image"));
	}
	return false; // should never happen
}

void ButcherImage_FreeImage::Load(wxInputStream &stream)
{
    if (!CanLoad(stream, true))
        throw ButcherException(_("Error loading image"));
}



bool ButcherImage_FreeImage::CanLoad(const wxImage &image, bool doload)
{
    try
    {
        wxFreeImage *imgtest=new wxFreeImage;

        //ButcherImage_FreeImage_wxStreamIO sio(stream);
        //bool ret=imgtest->loadFromHandle(sio.GetIO(), &sio);
        bool ret=imgtest->Load(image);
        if (ret && doload)
        {
            if (image_) delete image_;
            image_=imgtest;
        }
        else
            delete imgtest;
        return ret;
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
        return false;
    }
}

void ButcherImage_FreeImage::Load(const wxImage &image)
{
    if (!CanLoad(image, true))
        throw ButcherException(_("Error loading image"));
}

ButcherImage *ButcherImage_FreeImage::Clone()
{
    try
    {
        return new ButcherImage_FreeImage(new wxFreeImage(*image_));
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
        return NULL;
    }
}




ButcherImage *ButcherImage_FreeImage::SubImage(const wxRect &r)
{
    try
    {
        std::unique_ptr<wxFreeImage> si(new wxFreeImage());
        image_->Copy(*si, r);
        return new ButcherImage_FreeImage(si.release());
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
        return NULL;
    }
}

wxFreeImage *ButcherImage_FreeImage::ImageFormatPrepare(format_t format, int flags,
    transparentcolors_t *tcolors, transparentpixels_t *tpixels)
{
    // color quantize
    FREE_IMAGE_QUANTIZE q=FIQ_NNQUANT;
    if ((flags & GEN_QUANT_WU)==GEN_QUANT_WU)
        q=FIQ_WUQUANT;

    try
    {
        // GIF
        if (format == ButcherImage::FMT_GIF)
        {
            if (image_->GetBPP() > 8 || (tcolors && tcolors->size()>0))
            {
                std::unique_ptr<wxFreeImage> img(new wxFreeImage(*image_));
                if (image_->GetBPP() > 8)
                {
                    // must be 24 bits do quantize
                    if (image_->GetBPP()!=24)
                        img->Convert(wxFreeImage::CV_24BITS);

                    // convert to 8 bits
                    img->ColorQuantize(q);
                }

                // transparent colors
                if ((tcolors && tcolors->size()>0) || (tpixels && tpixels->size()>0))
                {
                    img->ApplyTransparentColors(tcolors, tpixels);
                }
                return img.release();
            }
        // JPG / XPM
        } else if (format == ButcherImage::FMT_JPG || format == ButcherImage::FMT_XPM) {

            // convert to 24 bits
            if (image_->NeedConvert(wxFreeImage::CV_24BITS))
            {
                std::unique_ptr<wxFreeImage> img(new wxFreeImage(*image_));
                img->Convert(wxFreeImage::CV_24BITS);
                return img.release();
            }
        // PNG
        } else if (format == ButcherImage::FMT_PNG || format == ButcherImage::FMT_BMP || format == ButcherImage::FMT_TIFF) {
            // convert to compatible bits
			wxFreeImage::convert_t cv=(format == ButcherImage::FMT_BMP?wxFreeImage::CV_DEFAULT:wxFreeImage::CV_24BITS);
            if ((flags&GEN_FORCE32BIT)==GEN_FORCE32BIT)
                cv=wxFreeImage::CV_32BITS;
			else if (format == ButcherImage::FMT_BMP && ((flags&GEN_FORCE16BIT)==GEN_FORCE16BIT))
				cv=wxFreeImage::CV_16BITS555;
            else if ((flags&GEN_FORCE8BIT)==GEN_FORCE8BIT)
                cv=wxFreeImage::CV_8BITS;

            if (image_->NeedConvert(cv) || (tcolors && tcolors->size()>0))
            {
                std::unique_ptr<wxFreeImage> img(new wxFreeImage(*image_));
                // convert
                if (image_->NeedConvert(cv))
                {
                    if (cv==wxFreeImage::CV_8BITS)
                        // convert to 8 bits
                        img->ColorQuantize(q);
                    else
                        img->Convert(cv);
                }
                // transparent colors
                if ((img->GetBPP()==8||img->GetBPP()==32) && tcolors &&
                    ((tcolors && tcolors->size()>0) || (tpixels && tpixels->size()>0)))
                {
                    img->ApplyTransparentColors(tcolors, tpixels);
                }

                return img.release();
            }
        }
        return image_;
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
        return NULL;
    }
}

void ButcherImage_FreeImage::Save(format_t format, const wxString &filename, int flags,
    transparentcolors_t *tcolors, transparentpixels_t *tpixels)
{
    try
    {
        wxFreeImage *img=ImageFormatPrepare(format, flags, tcolors, tpixels);
        //bool res=FreeImage_Save(FIFormat(format), *img,  filename.mb_str(wxConvUTF8), FIFlags(flags));
        bool res=img->Save(FIFormat(format), filename, FIFlags(flags));
        if (img!=image_) delete img;

        if (!res)
			throw ButcherException(wxString::Format(_("Error saving image %s"), filename.c_str()));
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
    }
}



ButcherImage *ButcherImage_FreeImage::Save(format_t format, int flags,
    transparentcolors_t *tcolors, transparentpixels_t *tpixels)
{
    try
    {
        wxFreeImage *img=ImageFormatPrepare(format, flags, tcolors, tpixels);

        wxMemoryOutputStream savemem;

        //fipMemoryIO mem;
        //bool ret=img->saveToMemory(FIFormat(format), mem, FIFlags(flags));
        bool ret=img->Save(FIFormat(format), savemem, FIFlags(flags));
        if (img!=image_) delete img;
        if (!ret)
            throw ButcherException(_("Unable to convert image"));

        //mem.seek(0, SEEK_SET);
        savemem.SeekO(0, wxFromStart);

        wxMemoryInputStream loadmem(savemem);

        wxFreeImage *imgret=new wxFreeImage;
        //if (!imgret->loadFromMemory(mem))
        if (!imgret->Load(loadmem))
            throw ButcherException(_("Unable to load converted image"));
        //mem.seek(0, SEEK_END);

        return new ButcherImage_FreeImage(imgret, loadmem.GetLength());
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
        return NULL;
    }
}



FREE_IMAGE_FORMAT ButcherImage_FreeImage::FIFormat(ButcherImage::format_t format)
{
    switch (format)
    {
    case ButcherImage::FMT_GIF:
        return FIF_GIF;
    case ButcherImage::FMT_PNG:
        return FIF_PNG;
	case ButcherImage::FMT_BMP:
        return FIF_BMP;
	case ButcherImage::FMT_TIFF:
		return FIF_TIFF;
	case ButcherImage::FMT_XPM:
		return FIF_XPM;
    default:
        return FIF_JPEG;
    }
}

int ButcherImage_FreeImage::FIFlags(int flags)
{
    int ret=0;
    if ((flags & JPG_QUAL_100) == JPG_QUAL_100) ret|=JPEG_QUALITYSUPERB;
    if ((flags & JPG_QUAL_75) == JPG_QUAL_75) ret|=JPEG_QUALITYGOOD;
    if ((flags & JPG_QUAL_50) == JPG_QUAL_50) ret|=JPEG_QUALITYNORMAL;
    if ((flags & JPG_QUAL_25) == JPG_QUAL_25) ret|=JPEG_QUALITYAVERAGE;
    if ((flags & JPG_QUAL_10) == JPG_QUAL_100) ret|=JPEG_QUALITYBAD;
    if ((flags & JPG_PROGRESSIVE) == JPG_QUAL_100) ret|=JPEG_PROGRESSIVE;

    return ret;
}

void ButcherImage_FreeImage::Draw(wxDC &dc, const wxRect &r)
{
    try
    {
        image_->Draw(dc, r);
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
    }
}



void ButcherImage_FreeImage::Composite(const wxBitmap &bitmap)
{
    try
    {
/*
        RGBQUAD bgcolor;
        memset(&bgcolor, 0, sizeof(bgcolor));
        bgcolor.rgbBlue=255;
*/

        RGBQUAD s, d;
        memset(&s, 0, sizeof(s));
        memset(&d, 0, sizeof(d));
        s.rgbBlue=255;
        s.rgbReserved=255;
        d.rgbBlue=255;

        wxImage img(bitmap.ConvertToImage());
        std::unique_ptr<wxFreeImage> cmp(new wxFreeImage(img));
        cmp->Convert(wxFreeImage::CV_32BITS);
        cmp->ApplyColorMapping(&s, &d, 1, false);

        image_->CompositeFG(&*cmp, false, NULL);
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
    }
}



wxColor ButcherImage_FreeImage::GetPixelColor(unsigned x, unsigned y)
{
    try
    {
        return image_->GetPixel(x, y);
    } catch (wxFreeImageException &e) {
        throw ButcherException(e.what());
        return *wxBLACK;
    }
}


