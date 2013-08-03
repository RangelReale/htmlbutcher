//=============================================================================
/**
 *  @file    wxFreeImage.h
 *
 *  $Id: wxFreeImage.h,v 1.7 2008/02/07 20:27:27 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-01-25
 */
//=============================================================================
#ifndef __WXFREEIMAGE_H_
#define __WXFREEIMAGE_H_

#include <wx/wx.h>
#include "FreeImage.h"
#include <exception>
#include <deque>

using namespace std;

// forward declarations
class wxFreeImageDraw;

/**
 * @class wxFreeImageException
 *
 * @brief FreeImage image exception
 */
class wxFreeImageException : public exception {
public:
    wxFreeImageException(const wxString &what) throw() : exception()
        { what_=what; }

    virtual ~wxFreeImageException() throw () {}

    const wxString &what() { return what_; }
private:
    wxString what_;
};

/**
 * @class wxFreeImage
 *
 * @brief FreeImage image
 */

typedef deque<wxColour> wxFreeImageColorList_t;
typedef deque<wxPoint> wxFreeImagePixelList_t;

class wxFreeImage : public wxObject
{
public:
    enum convert_t { CV_4BITS, CV_8BITS, CV_GRAYSCALE, CV_16BITS555, CV_16BITS565, CV_24BITS,
        CV_32BITS, CV_RGBF, CV_DEFAULT };
    enum flip_t { FL_HORIZONTAL, FL_VERTICAL };

    wxFreeImage();
    wxFreeImage(const wxFreeImage &src);
    wxFreeImage(FIBITMAP *dib, bool owns = true);
    wxFreeImage(const wxImage &image);
    virtual ~wxFreeImage();

    //operators
	wxFreeImage& operator=(const wxFreeImage& src);

    // handle
    FIBITMAP *GetHandle() { return dib_; }

    // loading / saving /creating
    bool New(int width, int height, int bpp) { return New(FIT_BITMAP, width, height, bpp); }
    bool New(FREE_IMAGE_TYPE image_type, int width, int height, int bpp) { return New(image_type, width, height, bpp, 0, 0, 0); }
    bool New(FREE_IMAGE_TYPE image_type, int width, int height, int bpp, unsigned red_mask,
        unsigned green_mask, unsigned blue_mask);

    bool Load(FIBITMAP *dib, bool owns = true);
    bool Load(const wxImage &image);
    bool Load(const wxString &filename, int flags = 0, FREE_IMAGE_FORMAT fif = FIF_UNKNOWN);
    bool Load(wxInputStream &stream, int flags = 0, FREE_IMAGE_FORMAT fif = FIF_UNKNOWN);

    bool Save(FREE_IMAGE_FORMAT fif, const wxString &filename, int flags = 0);
    bool Save(FREE_IMAGE_FORMAT fif, wxOutputStream &stream, int flags = 0);
    bool Save(wxImage &image);

    void Close();

    // copy
    wxFreeImage *Clone();
    void Clone(wxFreeImage &dib);
    void Copy(wxFreeImage &dib, const wxRect &rect);
    void Crop(const wxRect &rect);

    // operations
    bool NeedConvert(convert_t convert);
    void Convert(convert_t convert);
    void Threshold(unsigned char t);
    void ColorQuantize(FREE_IMAGE_QUANTIZE algorithm);
    void Dither(FREE_IMAGE_DITHER algorithm);
    void Rescale(unsigned long new_width, unsigned long new_height, FREE_IMAGE_FILTER filter);
    void Composite(bool usefilebg = false, RGBQUAD *bkcolor = NULL, wxFreeImage *bg = NULL);
    void CompositeFG(wxFreeImage *fg, bool usefilebg = false, RGBQUAD *bkcolor = NULL);
    void ApplyColorMapping(RGBQUAD *srccolors, RGBQUAD *dstcolors, unsigned count,
        bool ignore_alpha = true, bool swap = false);
    void Flip(flip_t flip);
    bool ApplyTransparentColors(const wxFreeImageColorList_t *colors, const wxFreeImagePixelList_t *pixels = NULL);

    // transparency
	bool GetIsTransparent();
	bool GetIsActiveTransparent();
	unsigned long GetTransparencyCount();
	unsigned char *GetTransparencyTable();
	void SetTransparencyTable(unsigned char *table, int count);
	bool HasBackgroundColor();
	bool GetBackgroundColor(RGBQUAD *bkcolor);
	bool SetBackgroundColor(RGBQUAD *bkcolor);

    // drawing
    void Draw(wxDC &dc) { Draw(dc, GetImageRect()); }
    void Draw(wxDC &dc, const wxRect &rect);

    // properties
    long GetWidth();
    long GetHeight();
    long GetBPP();
    FREE_IMAGE_TYPE GetImageType();
    wxRect GetImageRect() { return wxRect(0, 0, GetWidth()-1, GetHeight()-1); }
    unsigned long GetImageSize();
    unsigned char* GetScanLine(unsigned short scanline);
    unsigned char* GetBits();
    wxColor GetPixel(unsigned x, unsigned y);

    // status
    bool IsOk() const { return dib_!=NULL; }

    // modify
    void Modified() { modifyct_++; }
    unsigned long GetModified() { return modifyct_; }

    // static
    static void Initialize();
    static void Finalize();
private:
    void CheckOk();

    FIBITMAP *dib_;
    bool owns_;
    unsigned long modifyct_;
    wxFreeImageDraw *draw_;

    static bool isinit;
};

/**
 * @class wxFreeImageDraw
 *
 * @brief FreeImage image draw
 */
class wxFreeImageDraw : public wxObject
{
public:
    wxFreeImageDraw(wxFreeImage &image) :
        wxObject(), image_(image), displayimage_(NULL), lastmodifyct_(0) {}
    virtual ~wxFreeImageDraw();

    void Draw(wxDC &dc) { if (image_.IsOk()) Draw(dc, image_.GetImageRect()); }
    void Draw(wxDC &dc, const wxRect &rect);
private:
    void GenerateDisplay(const wxRect &rect);

    wxFreeImage &image_;
    wxFreeImage *displayimage_;
    unsigned long lastmodifyct_;
};

#endif //__WXFREEIMAGE_H_
