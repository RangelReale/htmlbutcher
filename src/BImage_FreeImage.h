//=============================================================================
/**
 *  @file    BImage_FreeImage.h
 *
 *  $Id: BImage_FreeImage.h,v 1.7 2008/02/07 20:27:27 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-24
 */
//=============================================================================
#ifndef __BIMAGE_FREEIMAGE_H__
#define __BIMAGE_FREEIMAGE_H__

#include <memory>
#include <wx/wx.h>
#include "BExcept.h"
#include "BImage.h"
#include "FreeImage.h"
//#include "FreeImagePlus.h"
#include "wxFreeImage.h"

/**
 * @class ButcherImage_FreeImage
 *
 * @brief Image - FreeImage
 */
class ButcherImage_FreeImage : public ButcherImage
{
public:
    ButcherImage_FreeImage();
    virtual ~ButcherImage_FreeImage();

    virtual long GetWidth();
    virtual long GetHeight();

    virtual unsigned long GetFileSize() { return filesize_; };

    virtual ButcherImage *Clone();
    virtual ButcherImage *SubImage(const wxRect &r);
    virtual ButcherImage *Save(format_t format, int flags,
        transparentcolors_t *tcolors = NULL, transparentpixels_t *tpixels = NULL);

    virtual void Save(format_t format, const wxString &filename, int flags,
        transparentcolors_t *tcolors = NULL, transparentpixels_t *tpixels = NULL);

    virtual void Draw(wxDC &dc, const wxRect &r);
    virtual void Composite(const wxBitmap &bitmap);

    virtual wxColor GetPixelColor(unsigned x, unsigned y);
protected:
    virtual bool CanLoad(const wxString &filename, bool doload = false);
    virtual void Load(const wxString &filename);
    virtual bool CanLoad(wxInputStream &stream, bool doload = false);
    virtual void Load(wxInputStream &stream);
    bool CanLoad(const wxImage &image, bool doload = false);
    void Load(const wxImage &image);

    wxFreeImage *ImageFormatPrepare(format_t format, int flags,
        transparentcolors_t *tcolors = NULL, transparentpixels_t *tpixels = NULL);

    friend class ButcherImageFactory;
private:
    ButcherImage_FreeImage(wxFreeImage *img, unsigned long filesize = 0);

    int FIFlags(int flags);
    FREE_IMAGE_FORMAT FIFormat(ButcherImage::format_t format);

    wxFreeImage *image_;
    unsigned long filesize_;
};


#endif //__BIMAGE_FREEIMAGE_H__
