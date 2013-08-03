//=============================================================================
/**
 *  @file    wxFreeImage.cpp
 *
 *  $Id: wxFreeImage.cpp,v 1.13 2008/06/14 19:10:14 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-25
 */
//=============================================================================

#include <memory>
#include "wxFreeImage.h"

/////////////////////////////////
// CLASS
//      wxFreeImage_wxStreamIO
/////////////////////////////////
class wxFreeImage_wxStreamIO
{
public:
    wxFreeImage_wxStreamIO(wxInputStream &stream) : istream_(&stream), ostream_(NULL), owns_(false) { Initialize(); }
    wxFreeImage_wxStreamIO(wxInputStream *stream) : istream_(stream), ostream_(NULL), owns_(true) { Initialize(); }
    wxFreeImage_wxStreamIO(wxOutputStream &stream) : istream_(NULL), ostream_(&stream), owns_(false) { Initialize(); }
    wxFreeImage_wxStreamIO(wxOutputStream *stream) : istream_(NULL), ostream_(stream), owns_(true) { Initialize(); }
    ~wxFreeImage_wxStreamIO() { if (owns_) { if (istream_) delete istream_; if (ostream_) delete ostream_; } }

    wxInputStream *GetIStream() { return istream_; }
    wxOutputStream *GetOStream() { return ostream_; }
    FreeImageIO *GetIO() { return &io_; }
private:
    void Initialize();

    wxInputStream *istream_;
    wxOutputStream *ostream_;
    bool owns_;
    FreeImageIO io_;
};


unsigned DLL_CALLCONV wxFreeImage_wxStreamIO_Read(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
    wxFreeImage_wxStreamIO *s=(wxFreeImage_wxStreamIO *)handle;
    unsigned ret=0;
    wxFileOffset rdata;
    char *bchar=static_cast<char*>(buffer);
    for (unsigned i=0; i<count; i++)
    {
        rdata=s->GetIStream()->Read(bchar, size).LastRead();
        if (rdata!=size)
            break;
        ret++;
        bchar+=size;
    }
    return ret;
}

unsigned DLL_CALLCONV wxFreeImage_wxStreamIO_Write(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
    wxFreeImage_wxStreamIO *s=(wxFreeImage_wxStreamIO *)handle;
    unsigned ret=0;
    wxFileOffset rdata;
    char *bchar=static_cast<char*>(buffer);
    for (unsigned i=0; i<count; i++)
    {
        rdata=s->GetOStream()->Write(bchar, size).LastWrite();
        if (rdata!=size)
            break;
        ret++;
        bchar+=size;
    }
    return ret;
}

int DLL_CALLCONV wxFreeImage_wxStreamIO_Seek (fi_handle handle, long offset, int origin)
{
    wxFreeImage_wxStreamIO *s=(wxFreeImage_wxStreamIO *)handle;
    wxSeekMode sm;
    switch (origin)
    {
    case SEEK_SET:
        sm=wxFromStart;
        break;
    case SEEK_END:
        sm=wxFromEnd;
        break;
    default:
        sm=wxFromCurrent;
        break;
    }
    off_t newo;
    if (s->GetIStream())
        newo=s->GetIStream()->SeekI(offset, sm);
    else
        newo=s->GetOStream()->SeekO(offset, sm);
    if (newo==wxInvalidOffset || newo!=offset) return 1;
    return 0;
}

long DLL_CALLCONV wxFreeImage_wxStreamIO_Tell (fi_handle handle)
{
    wxFreeImage_wxStreamIO *s=(wxFreeImage_wxStreamIO *)handle;
    if (s->GetIStream())
        return s->GetIStream()->TellI();
    else
        return s->GetOStream()->TellO();
}

void wxFreeImage_wxStreamIO::Initialize()
{
    memset(&io_, 0, sizeof(io_));
    if (istream_)
        io_.read_proc=&wxFreeImage_wxStreamIO_Read;
    if (ostream_)
        io_.write_proc=&wxFreeImage_wxStreamIO_Write;
    io_.seek_proc=&wxFreeImage_wxStreamIO_Seek;
    io_.tell_proc=&wxFreeImage_wxStreamIO_Tell;
}

/////////////////////////////////
// CLASS
//      wxFreeImage
/////////////////////////////////
bool wxFreeImage::isinit = false;

wxFreeImage::wxFreeImage() :
    wxObject(), dib_(NULL), owns_(false), modifyct_(0), draw_(NULL)
{

}

wxFreeImage::wxFreeImage(const wxFreeImage &src) :
    wxObject(), dib_(NULL), owns_(false), modifyct_(0), draw_(NULL)
{
    if (src.IsOk())
    {
        dib_=FreeImage_Clone(src.dib_);
        owns_=true;
        CheckOk();
    }
}


wxFreeImage::wxFreeImage(FIBITMAP *dib, bool owns) :
    wxObject(), dib_(dib), owns_(owns), modifyct_(0), draw_(NULL)
{
    CheckOk();
}

wxFreeImage::wxFreeImage(const wxImage &image) :
    wxObject(), dib_(NULL), owns_(false), modifyct_(0), draw_(NULL)
{
    Load(image);
    CheckOk();
}

wxFreeImage::~wxFreeImage()
{
    Close();
}

// operators
wxFreeImage& wxFreeImage::operator=(const wxFreeImage& src)
{
    if (this!=&src)
    {
        Close();
        if (src.IsOk())
        {
            dib_=FreeImage_Clone(src.dib_);
            owns_=true;
            Modified();
            CheckOk();
        }
    }
    return *this;
}

// loading / saving / creating
bool wxFreeImage::New(FREE_IMAGE_TYPE image_type, int width, int height, int bpp, unsigned red_mask,
    unsigned green_mask, unsigned blue_mask)
{
    Close();

	if((dib_ = FreeImage_AllocateT(image_type, width, height, bpp, red_mask, green_mask, blue_mask)) == NULL)
		return false;

	if(image_type == FIT_BITMAP) {
		// Create palette if needed
		switch(bpp)	{
			case 1:
			case 4:
			case 8:
				RGBQUAD *pal = FreeImage_GetPalette(dib_);
				for(unsigned i = 0; i < FreeImage_GetColorsUsed(dib_); i++) {
					pal[i].rgbRed = i;
					pal[i].rgbGreen = i;
					pal[i].rgbBlue = i;
				}
				break;
		}
	}
	Modified();
    return true;
}

bool wxFreeImage::Load(FIBITMAP *dib, bool owns)
{
    if (!dib)
        throw wxFreeImageException(_("Invalid image handle"));

    Close();

    dib_=dib;
    owns_=owns;
    Modified();

    return true;
}

bool wxFreeImage::Load(const wxImage &image)
{
    if (!image.IsOk()) return false;

    // wxImage is always 24 bits/ BGR
    New(FIT_BITMAP, image.GetWidth(), image.GetHeight(), 24);//, FI_RGBA_BLUE_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_RED_MASK );
    CheckOk();

    unsigned y;
    unsigned char *data, *imgdata=image.GetData();
    int linesize=GetWidth()*(GetBPP()/8);

    for (y=0; y<FreeImage_GetHeight(dib_); y++)
    {
        data=FreeImage_GetScanLine(dib_, y);
        if (!data)
            throw wxFreeImageException(_("Error getting image bits"));

        memcpy(data, imgdata, linesize);
        imgdata+=linesize;
    }
    Modified();
    return true;
}

bool wxFreeImage::Load(const wxString &filename, int flags, FREE_IMAGE_FORMAT fif)
{
    Close();

	//FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and get its format
	// (the second argument is currently not used by FreeImage)
	if(fif == FIF_UNKNOWN)
        fif = FreeImage_GetFileType(filename.mb_str(wxConvISO8859_1), 0);
	if(fif == FIF_UNKNOWN) {
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(filename.mb_str(wxConvISO8859_1));
	}
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// Load the file
		dib_ = FreeImage_Load(fif, filename.mb_str(wxConvISO8859_1), flags);
		if(dib_ == NULL)
			return false;
        Modified();
		return true;
	}
	return false;
}

bool wxFreeImage::Load(wxInputStream &stream, int flags, FREE_IMAGE_FORMAT fif)
{
    Close();

    wxFreeImage_wxStreamIO sio(stream);

	// check the file signature and get its format
	if(fif == FIF_UNKNOWN)
        fif = FreeImage_GetFileTypeFromHandle(sio.GetIO(), &sio, 16);
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// Load the file
		dib_ = FreeImage_LoadFromHandle(fif, sio.GetIO(), &sio, flags);
		if(dib_ == NULL)
			return false;
        Modified();
		return true;
	}
    return false;
}

bool wxFreeImage::Save(FREE_IMAGE_FORMAT fif, const wxString &filename, int flags)
{
    CheckOk();

	//FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	bool bSuccess = false;

	// Try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN )
        fif = FreeImage_GetFIFFromFilename(filename.mb_str(wxConvISO8859_1));
	if(fif != FIF_UNKNOWN )
	{
		// Check that the dib can be saved in this format
		bool bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib_);
		if(image_type == FIT_BITMAP) {
			// standard bitmap type
			unsigned short bpp = FreeImage_GetBPP(dib_);
			bCanSave = (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp));
		} else {
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(fif, image_type)?true:false;
		}

		if(bCanSave) {
			try {
				bSuccess = FreeImage_Save(fif, dib_, filename.mb_str(wxConvISO8859_1), flags)?true:false;
			} catch (...) {
				bSuccess=false;
			}
			return bSuccess;
		}
	}
	return bSuccess;
}

bool wxFreeImage::Save(FREE_IMAGE_FORMAT fif, wxOutputStream &stream, int flags)
{
    CheckOk();

	bool bSuccess = FALSE;

	if(fif != FIF_UNKNOWN ) {
        wxFreeImage_wxStreamIO sio(stream);

		// Check that the dib can be saved in this format
		bool bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib_);
		if(image_type == FIT_BITMAP) {
			// standard bitmap type
			unsigned short bpp = FreeImage_GetBPP(dib_);
			bCanSave = (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp));
		} else {
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(fif, image_type)?true:false;
		}

		if(bCanSave) {
			bSuccess = FreeImage_SaveToHandle(fif, dib_, sio.GetIO(), &sio, flags)?true:false;
			return bSuccess;
		}
	}
	return bSuccess;
}

bool wxFreeImage::Save(wxImage &image)
{
    CheckOk();

    auto_ptr<wxFreeImage> simg(this->Clone());

    simg->Convert(CV_24BITS);
    //simg->Flip(wxFreeImage::FL_VERTICAL); // don't flip here, we need to compare the pixels to do the transparency

    for (long i = 0; i < simg->GetHeight(); ++i) {
        RGBTRIPLE *scanline = (RGBTRIPLE *) simg->GetScanLine(i);

        for (long j = 0; j < simg->GetWidth(); ++j) {
            BYTE tmp = scanline[j].rgbtRed;
            scanline[j].rgbtRed = scanline[j].rgbtBlue;
            scanline[j].rgbtBlue = tmp;
        }
    }

    image.Create(simg->GetWidth(), simg->GetHeight(), false);
    //image.Resize(simg->GetImageRect().GetSize(), wxPoint(0, 0));

    unsigned x, y;
    unsigned char *data, *imgdata=image.GetData(), *origdata=NULL;
    int linesize=simg->GetWidth()*(simg->GetBPP()/8);
    unsigned char *alpha=NULL, *alphapt, *ttable=NULL;
    bool isalpha=false;

    if (FreeImage_IsTransparent(dib_) &&
        (GetBPP()==8 || GetBPP()==32))
    {
        isalpha=true;
        if (GetBPP()==8)
        {
            if (FreeImage_GetTransparencyCount(dib_)>0)
                ttable=FreeImage_GetTransparencyTable(dib_);
            else
                isalpha=false;
        }

        if (isalpha)
        {
            alpha=static_cast<unsigned char*>(malloc(simg->GetWidth()*simg->GetHeight()));
            memset(alpha, 0xFF, simg->GetWidth()*simg->GetHeight());
        }
    }

    alphapt=alpha;
    for (y=0; y<FreeImage_GetHeight(simg->dib_); y++)
    {
        data=FreeImage_GetScanLine(simg->dib_, y);
        if (!data)
            throw wxFreeImageException(_("Error getting image bits"));
        if (isalpha)
        {
            origdata=FreeImage_GetScanLine(dib_, y);
            if (!origdata)
                throw wxFreeImageException(_("Error getting image bits"));
        }

        memcpy(imgdata, data, linesize);

        if (isalpha)
        {
            for (x=0; x<FreeImage_GetWidth(dib_); x++)
            {
                switch (GetBPP())
                {
                case 8:
                    if (ttable[origdata[y*FreeImage_GetWidth(dib_)+x]]==0)
                        *alphapt=0;
                    break;
                case 32:
                    *alphapt=reinterpret_cast<RGBQUAD*>(origdata)[x].rgbReserved;
                    break;
                }
                alphapt++;
            }
        }


        imgdata+=linesize;
    }
    if (isalpha)
    {
        if (!image.HasAlpha())
            image.InitAlpha();
        image.SetAlpha(alpha);
    }
    image=image.Mirror(false);
    return true;
}

void wxFreeImage::Close()
{
    if (dib_ && owns_)
    {
        FreeImage_Unload(dib_);
    }
    if (dib_)
    {
        Modified();
        dib_=NULL;
        owns_=false;
    }
    if (draw_)
    {
        delete draw_;
        draw_=NULL;
    }
}

// copy
wxFreeImage *wxFreeImage::Clone()
{
    CheckOk();
    return new wxFreeImage(FreeImage_Clone(dib_));
}

void wxFreeImage::Clone(wxFreeImage &dib)
{
    CheckOk();
    dib.Load(FreeImage_Clone(dib_));
}

void wxFreeImage::Copy(wxFreeImage &dib, const wxRect &rect)
{
    CheckOk();
    //dib.Load(FreeImage_Copy(dib_, rect.GetLeft(), rect.GetTop(), rect.GetRight()+1, rect.GetBottom()+1)); // add 1 because of FreeImage handling

	wxRect adjrect(rect);
	if (rect.GetLeft() > GetWidth())
		adjrect.SetLeft(GetWidth()-1);
	if (rect.GetTop() > GetHeight()-1)
		adjrect.SetTop(GetHeight());
	if (rect.GetRight() > GetWidth())
		adjrect.SetRight(GetWidth()-1);
	if (rect.GetBottom() > GetHeight())
		adjrect.SetBottom(GetHeight()-1);

	// FREEIMAGE BUG: RIGHT/BOTTOM can't be equals WIDTH/HEIGHT
	if (rect.GetRight() == GetWidth())
		adjrect.SetRight(GetWidth()-1);
	if (rect.GetBottom() == GetHeight())
		adjrect.SetBottom(GetHeight()-1);

	wxFreeImage srccopy(FreeImage_Copy(dib_, adjrect.GetLeft(), adjrect.GetTop(), adjrect.GetRight()+1, adjrect.GetBottom()+1)); // add 1 because of FreeImage handling

	dib.New(rect.GetWidth(), rect.GetHeight(), GetBPP());
	if (!FreeImage_Paste(dib.GetHandle(), srccopy.GetHandle(), 0, 0, 256))
        throw wxFreeImageException(_("Error copying image"));
}

void wxFreeImage::Crop(const wxRect &rect)
{
    Copy(*this, rect);
}

// operations
bool wxFreeImage::NeedConvert(convert_t convert)
{
    switch (convert)
    {
    case CV_4BITS:
        return GetBPP()!=4;
    case CV_8BITS:
        return GetBPP()!=8;
    case CV_GRAYSCALE:
        return GetBPP()!=8;
    case CV_16BITS555:
        return GetBPP()!=16;
    case CV_16BITS565:
        return GetBPP()!=16;
    case CV_24BITS:
        return GetBPP()!=24;
    case CV_32BITS:
        return GetBPP()!=32;
    case CV_RGBF:
        return GetBPP()!=96;
    default:
        return true;
    }
}

void wxFreeImage::Convert(convert_t convert)
{
    switch (convert)
    {
    case CV_4BITS:
        Load(FreeImage_ConvertTo4Bits(dib_));
        break;
    case CV_8BITS:
        Load(FreeImage_ConvertTo8Bits(dib_));
        break;
    case CV_GRAYSCALE:
        Load(FreeImage_ConvertToGreyscale(dib_));
        break;
    case CV_16BITS555:
        Load(FreeImage_ConvertTo16Bits555(dib_));
        break;
    case CV_16BITS565:
        Load(FreeImage_ConvertTo16Bits565(dib_));
        break;
    case CV_24BITS:
        Load(FreeImage_ConvertTo24Bits(dib_));
        break;
    case CV_32BITS:
        Load(FreeImage_ConvertTo32Bits(dib_));
        break;
    case CV_RGBF:
        Load(FreeImage_ConvertToRGBF(dib_));
        break;
    default:
        break;
    }
}

void wxFreeImage::Threshold(unsigned char t)
{
    Load(FreeImage_Threshold(dib_, t));
}

void wxFreeImage::ColorQuantize(FREE_IMAGE_QUANTIZE algorithm)
{
    if (GetBPP()!=24)
        Convert(CV_24BITS);
    Load(FreeImage_ColorQuantize(dib_, algorithm));
}

void wxFreeImage::Dither(FREE_IMAGE_DITHER algorithm)
{
    Load(FreeImage_Dither(dib_, algorithm));
}

void wxFreeImage::Rescale(unsigned long new_width, unsigned long new_height,
    FREE_IMAGE_FILTER filter)
{
    Load(FreeImage_Rescale(dib_, new_width, new_height, filter));
}

void wxFreeImage::Composite(bool usefilebg, RGBQUAD *bkcolor, wxFreeImage *bg)
{
    if (GetBPP()!=32)
        // must be 8 or 32 bits
        Convert(CV_32BITS);

    auto_ptr<wxFreeImage> newbg(NULL);
    if (bg->GetBPP()!=24)
    {
        // must be 24 bits
        newbg.reset(bg->Clone());
        newbg->Convert(CV_24BITS);
        bg=&*newbg;
    }


    //applycolormapping

    Load(FreeImage_Composite(dib_, usefilebg, bkcolor, (bg?bg->GetHandle():NULL)));
}

void wxFreeImage::CompositeFG(wxFreeImage *fg, bool usefilebg, RGBQUAD *bkcolor)
{
    auto_ptr<wxFreeImage> newfg(NULL);
    if (fg->GetBPP()!=32)
    {
        // must be 8 or 32 bits
        newfg.reset(fg->Clone());
        newfg->Convert(CV_24BITS);
        fg=&*newfg;
    }

    if (GetBPP()!=24)
        // must be 24 bits
        Convert(CV_24BITS);


    //applycolormapping

    Load(FreeImage_Composite(fg->GetHandle(), usefilebg, bkcolor, dib_));
}

void wxFreeImage::ApplyColorMapping(RGBQUAD *srccolors, RGBQUAD *dstcolors, unsigned count,
    bool ignore_alpha, bool swap)
{
    CheckOk();

    FreeImage_ApplyColorMapping(dib_, srccolors, dstcolors, count, ignore_alpha, swap);
}

void wxFreeImage::Flip(flip_t flip)
{
    CheckOk();

    bool ret;
    if (flip==FL_HORIZONTAL)
		ret=FreeImage_FlipHorizontal(dib_)?true:false;
    else
		ret=FreeImage_FlipVertical(dib_)?true:false;
    if (!ret)
        throw wxFreeImageException(_("Error flipping image"));
}

bool wxFreeImage::ApplyTransparentColors(const wxFreeImageColorList_t *colors,
    const wxFreeImagePixelList_t *pixels)
{
    CheckOk();
    if (!colors && !pixels) return false;
    if (colors && colors->size()==0 && !pixels) return false;
    if (pixels && pixels->size()==0 && !colors) return false;

    if (GetBPP()==16||GetBPP()==24)
    {
        Convert(CV_32BITS);
    }
    FreeImage_SetTransparent(dib_, true);

    FREE_IMAGE_COLOR_TYPE it = FreeImage_GetColorType(dib_);
    if (it==FIC_PALETTE)
    {
        deque<unsigned char> paltranslist;

        // for palletized images, change all pallete indexes to the first available color
        RGBQUAD* pal=FreeImage_GetPalette(dib_);
        int paltrans=-1;
        // locate existing transparent colors
        if (colors)
        {
            for (wxFreeImageColorList_t::const_iterator ci=colors->begin(); ci!=colors->end(); ci++)
            {
                for (unsigned i=0; i<256; i++)
                {
                    if (pal[i].rgbRed==ci->Red() &&
                        pal[i].rgbGreen==ci->Green() &&
                        pal[i].rgbBlue==ci->Blue())
                    {
                        if (paltrans==-1) paltrans=i;
                        paltranslist.push_back(i);
                    }
                }
            }
        }
        // no pallete item found for any of the passed colors, if only transparent color do nothing
        if (paltrans==-1 && (!pixels || pixels->size()==0)) return false;


        // no to-be-transparent pallete item found, try to get a default
        if (paltrans==-1) paltrans=FreeImage_GetTransparentIndex(dib_);

        // no to-be-transparent pallete item found, found if there is an unused palette item
        if (paltrans==-1)
        {
            unsigned char* pval;
            long pused[256];
            memset(pused, 0, sizeof(pused));
            for (unsigned pline=0; pline<FreeImage_GetHeight(dib_); pline++)
            {
                pval=FreeImage_GetScanLine(dib_, pline);
                for (unsigned pcol=0; pcol<FreeImage_GetWidth(dib_); pcol++)
                    pused[pval[pcol]]++;
            }

            // locate unused item
            for (unsigned ppal=0; ppal<256; ppal++)
                if (pused[ppal]==0)
                {
                    paltrans=ppal;
                    break;
                }
        }


        // no pallete item found for any of the passed colors, do nothing
        if (paltrans==-1) return false;


        // transparent indexes
        if (paltranslist.size()>0)
        {
            // change all indexes to the transparent one
#ifndef _MSC_VER
			unsigned char srcindices[paltranslist.size()];
            unsigned char dstindices[paltranslist.size()];
#else
			auto_ptr<unsigned char> srcindices_data(new unsigned char[paltranslist.size()]);
			auto_ptr<unsigned char> dstindices_data(new unsigned char[paltranslist.size()]);
			unsigned char *srcindices=srcindices_data.get();
            unsigned char *dstindices=dstindices_data.get();
#endif

            int ct=0;
            for (deque<unsigned char>::const_iterator ptli=paltranslist.begin(); ptli!=paltranslist.end(); ptli++)
            {
                srcindices[ct]=*ptli;
                dstindices[ct]=paltrans;
                ct++;
            }
            FreeImage_ApplyPaletteIndexMapping(dib_, srcindices, dstindices, static_cast<unsigned int>(paltranslist.size()), false);
        }

        // transparent pixels
        if (pixels)
        {
            unsigned char ptbyte=paltrans;
            for (wxFreeImagePixelList_t::const_iterator ci=pixels->begin(); ci!=pixels->end(); ci++)
            {
                // in freeimage pixels are upside down
                FreeImage_SetPixelIndex(dib_, ci->x, FreeImage_GetHeight(dib_)-ci->y-1, &ptbyte);
            }
        }


        // set transparent index
        FreeImage_SetTransparentIndex(dib_, paltrans);
    }
    else if (it==FIC_RGB || it==FIC_RGBALPHA)
    {
        if (colors)
        {
            // for 32bit images, change all colors to transparent alpha
            RGBQUAD *srccolors=new RGBQUAD[colors->size()];
            RGBQUAD *dstcolors=new RGBQUAD[colors->size()];
            int ct=0;
            for (wxFreeImageColorList_t::const_iterator ci=colors->begin(); ci!=colors->end(); ci++)
            {
                srccolors[ct].rgbRed=ci->Red();
                srccolors[ct].rgbGreen=ci->Green();
                srccolors[ct].rgbBlue=ci->Blue();
                srccolors[ct].rgbReserved=0xFF;
                dstcolors[ct]=srccolors[ct];
                dstcolors[ct].rgbReserved=0;
                ct++;
            }
            FreeImage_ApplyColorMapping(dib_, srccolors, dstcolors, static_cast<unsigned int>(colors->size()), false, false);
        }

        if (pixels)
        {
            // change the alpha of transparent pixels
            RGBQUAD cvalue;
            for (wxFreeImagePixelList_t::const_iterator ci=pixels->begin(); ci!=pixels->end(); ci++)
            {
                // in freeimage pixels are upside down
                if (FreeImage_GetPixelColor(dib_, ci->x, FreeImage_GetHeight(dib_)-ci->y-1, &cvalue))
                {
                    cvalue.rgbReserved=0;
                    FreeImage_SetPixelColor(dib_, ci->x, FreeImage_GetHeight(dib_)-ci->y-1, &cvalue);
                }
            }
        }
    }
    else
        throw wxFreeImageException(_("This image cannot be transparent"));
    return false;
}

// transparency
bool wxFreeImage::GetIsTransparent()
{
	return FreeImage_IsTransparent(dib_)?true:false;
}

bool wxFreeImage::GetIsActiveTransparent()
{
    if (FreeImage_IsTransparent(dib_) &&
        (GetBPP()==8 || GetBPP()==32))
    {
        if (GetBPP()==8)
        {
            if (FreeImage_GetTransparencyCount(dib_)<=0)
				return false;
        }
		return true;
	}
	return false;
}

unsigned long wxFreeImage::GetTransparencyCount()
{
	return FreeImage_GetTransparencyCount(dib_);
}

unsigned char *wxFreeImage::GetTransparencyTable()
{
	return FreeImage_GetTransparencyTable(dib_);
}

void wxFreeImage::SetTransparencyTable(unsigned char *table, int count)
{
	FreeImage_SetTransparencyTable(dib_, table, count);
    Modified();
}

bool wxFreeImage::HasBackgroundColor()
{
	return FreeImage_HasBackgroundColor(dib_)?true:false;
}

bool wxFreeImage::GetBackgroundColor(RGBQUAD *bkcolor)
{
	return FreeImage_GetBackgroundColor(dib_, bkcolor)?true:false;
}

bool wxFreeImage::SetBackgroundColor(RGBQUAD *bkcolor)
{
	if (FreeImage_SetBackgroundColor(dib_, bkcolor))
	{
        Modified();
        return true;
	}
	return false;
}

// drawing
void wxFreeImage::Draw(wxDC &dc, const wxRect &rect)
{
    if (!draw_)
    {
        draw_=new wxFreeImageDraw(*this);
    }
    draw_->Draw(dc, rect);
}

// properties

long wxFreeImage::GetWidth()
{
    CheckOk();
	return FreeImage_GetWidth(dib_);
}

long wxFreeImage::GetHeight()
{
    CheckOk();
	return FreeImage_GetHeight(dib_);
}

long wxFreeImage::GetBPP()
{
    CheckOk();
	return FreeImage_GetBPP(dib_);
}

FREE_IMAGE_TYPE wxFreeImage::GetImageType()
{
    CheckOk();
	return FreeImage_GetImageType(dib_);
}

unsigned long wxFreeImage::GetImageSize()
{
    CheckOk();
	return FreeImage_GetDIBSize(dib_);
}

unsigned char* wxFreeImage::GetScanLine(unsigned short scanline)
{
    CheckOk();

	if(scanline < FreeImage_GetHeight(dib_)) {
		return FreeImage_GetScanLine(dib_, scanline);
	}
	return NULL;
}

unsigned char* wxFreeImage::GetBits()
{
    CheckOk();
    return FreeImage_GetBits(dib_);
}

wxColor wxFreeImage::GetPixel(unsigned x, unsigned y)
{
    CheckOk();
    FREE_IMAGE_COLOR_TYPE it = FreeImage_GetColorType(dib_);

    // in freeimage pixel height is stored upside down, invert the y parameter
    y=GetHeight()-y-1;

    if (it==FIC_PALETTE)
    {
        RGBQUAD* pal=FreeImage_GetPalette(dib_);
        unsigned char value;
        if (!FreeImage_GetPixelIndex(dib_, x, y, &value))
            throw wxFreeImageException(_("Could not get pixel color"));
        return wxColor(pal[value].rgbRed, pal[value].rgbGreen, pal[value].rgbBlue);
    }
    else if (it==FIC_RGB || it==FIC_RGBALPHA)
    {
        RGBQUAD value;
        if (!FreeImage_GetPixelColor(dib_, x, y, &value))
            throw wxFreeImageException(_("Could not get pixel color"));
        return wxColor(value.rgbRed, value.rgbGreen, value.rgbBlue);
    }
    else
        throw wxFreeImageException(_("Invalid color type for this operation"));
}

void wxFreeImage::CheckOk()
{
    if (!IsOk())
        throw wxFreeImageException(_("No image available"));
}

void wxFreeImage::Initialize()
{
#ifdef FREEIMAGE_LIB
    if (!wxFreeImage::isinit)
    {
        FreeImage_Initialise();
        wxFreeImage::isinit=true;
    }
#endif //FREEIMAGE_LIB
}

void wxFreeImage::Finalize()
{
#ifdef FREEIMAGE_LIB
    if (wxFreeImage::isinit)
    {
        FreeImage_DeInitialise();
        wxFreeImage::isinit=false;
    }
#endif //FREEIMAGE_LIB
}

/////////////////////////////////
// CLASS
//      wxFreeImageDraw
/////////////////////////////////
wxFreeImageDraw::~wxFreeImageDraw()
{
    if (displayimage_)
    {
        if (displayimage_!=(&image_))
            delete displayimage_;
        displayimage_=NULL;
    }
}


#if 0
//#ifdef __WXMSW__
void wxFreeImageDraw::Draw(wxDC &dc, const wxRect &rect)
{
    rect.GetSize().IncBy(1, 1); // add 1 because of FreeImage handling

    GenerateDisplay(rect);
    if (!displayimage_) return;

    // Draw the dib
	SetStretchBltMode((HDC)dc.GetHDC(), COLORONCOLOR);
	StretchDIBits((HDC)dc.GetHDC(), rect.GetLeft(), rect.GetTop(),
		rect.GetWidth(), rect.GetHeight(),
		0, 0, displayimage_->GetWidth(), displayimage_->GetHeight(),
		FreeImage_GetBits(displayimage_->GetHandle()), FreeImage_GetInfo(displayimage_->GetHandle()),
		DIB_RGB_COLORS, SRCCOPY);
}

void wxFreeImageDraw::GenerateDisplay(const wxRect &rect)
{
    if (displayimage_==NULL || lastmodifyct_!=image_.GetModified())
    {
        if (displayimage_)
        {
            if (displayimage_!=(&image_))
                delete displayimage_;
            displayimage_=NULL;
        }
        if (!image_.IsOk()) return;

		FREE_IMAGE_TYPE image_type = image_.GetImageType();
		if(image_type == FIT_BITMAP) {
			displayimage_=&image_;
/*

			bool bHasBackground = image_.HasBackgroundColor();
			bool bIsTransparent = image_.GetIsTransparent();

			if(!bIsTransparent && (!bHasBackground || !useFileBkg)) {
				// Copy pointer
				_display_dib = _dib;
			}
			else {
				// Create the transparent / alpha blended image
				_display_dib = FreeImage_Composite(_dib, useFileBkg, appBkColor, bg);
				if(_display_dib) {
					// Remember to delete _display_dib
					_bDeleteMe = TRUE;
				} else {
					// Something failed: copy pointers
					_display_dib = _dib;
				}
			}
*/
		} else {
			// Convert to a standard dib for display

			if(image_type == FIT_COMPLEX) {
				// Convert to type FIT_DOUBLE
				FIBITMAP *dib_double = FreeImage_GetComplexChannel(image_.GetHandle(), FICC_MAG);
				// Convert to a standard bitmap (linear scaling)
				displayimage_=new wxFreeImage(FreeImage_ConvertToStandardType(dib_double, TRUE));
				// Free image of type FIT_DOUBLE
				FreeImage_Unload(dib_double);
			} else if((image_type == FIT_RGBF) || (image_type == FIT_RGB16)) {
				// Apply a tone mapping algorithm and convert to 24-bit
				displayimage_=new wxFreeImage(FreeImage_ToneMapping(image_.GetHandle(), FITMO_DRAGO03, 0, 0));
			} else if(image_type == FIT_RGBA16) {
				// Convert to 32-bit
				FIBITMAP *dib32 = FreeImage_ConvertTo32Bits(image_.GetHandle());
				if(dib32) {
					// Create the transparent / alpha blended image
					displayimage_=new wxFreeImage(FreeImage_Composite(dib32, false, NULL, NULL));
					FreeImage_Unload(dib32);
				}
			} else {
				// Other cases: convert to a standard bitmap (linear scaling)
				displayimage_=new wxFreeImage(FreeImage_ConvertToStandardType(image_.GetHandle(), true));
			}
		}
	}
    lastmodifyct_=image_.GetModified();
}
//#elif defined(__WXGTK__)
#endif

void wxFreeImageDraw::Draw(wxDC &dc, const wxRect &rect)
{
    rect.GetSize().IncBy(1, 1); // add 1 because of FreeImage handling

    GenerateDisplay(rect);
    if (!displayimage_) return;

	#ifdef __WXMSW__
	if (!displayimage_->GetIsActiveTransparent()) // StretchDIBits does not draw transparent images, fallback to wx drawing
	{
		// Draw the dib
		SetStretchBltMode((HDC)dc.GetHDC(), COLORONCOLOR);
		StretchDIBits((HDC)dc.GetHDC(), rect.GetLeft(), rect.GetTop(),
			rect.GetWidth(), rect.GetHeight(),
			0, 0, displayimage_->GetWidth(), displayimage_->GetHeight(),
			FreeImage_GetBits(displayimage_->GetHandle()), FreeImage_GetInfo(displayimage_->GetHandle()),
			DIB_RGB_COLORS, SRCCOPY);
	} else {
	#endif //__WXMSW__
    //wxImage ximg(displayimage_->GetWidth(), displayimage_->GetHeight(), displayimage_->GetBits(), true);
    wxImage ximg;
    if (!displayimage_->Save(ximg)) return;
    //ximg.SetAlpha();
    wxBitmap xbmp(ximg/*, imgdraw.getBitsPerPixel()*/);

    //wxMemoryDC temp_dc;
    //temp_dc.SelectObject(xbmp);

    //dc.Blit(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight(), &temp_dc, 0, 0);

    dc.DrawBitmap(xbmp, rect.GetLeft(), rect.GetTop(), false);
	#ifdef __WXMSW__
	}
	#endif //__WXMSW__
}

void wxFreeImageDraw::GenerateDisplay(const wxRect &rect)
{
    if (displayimage_==NULL ||
        displayimage_->GetWidth()!=rect.GetWidth() ||
        displayimage_->GetHeight()!=rect.GetHeight() ||
        lastmodifyct_!=image_.GetModified())
    {
        if (displayimage_)
        {
            if (displayimage_!=(&image_))
                delete displayimage_;
            displayimage_=NULL;
        }
        if (!image_.IsOk()) return;

        displayimage_=image_.Clone();
        displayimage_->Rescale(rect.GetWidth(), rect.GetHeight(), FILTER_BOX);
/*
        displayimage_->Convert(wxFreeImage::CV_24BITS);
        displayimage_->Flip(wxFreeImage::FL_VERTICAL);

        for (unsigned int i = 0; i < displayimage_->GetHeight(); ++i) {
            RGBTRIPLE *scanline = (RGBTRIPLE *) displayimage_->GetScanLine(i);

            for (unsigned int j = 0; j < displayimage_->GetWidth(); ++j) {
                BYTE tmp = scanline[j].rgbtRed;
                scanline[j].rgbtRed = scanline[j].rgbtBlue;
                scanline[j].rgbtBlue = tmp;
            }
        }
*/
	}

    lastmodifyct_=image_.GetModified();
}

//#endif
