//=============================================================================
/**
 *  @file    BImage.h
 *
 *  $Id: BImage.h,v 1.16 2008/02/07 20:27:27 rreale Exp $
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2007-12-18
 */
//=============================================================================
#ifndef __BIMAGE_H__
#define __BIMAGE_H__

#include <wx/wx.h>
#include "BExcept.h"
#include <deque>

using namespace std;


/**
 * @class ButcherImage
 *
 * @brief Image
 */
class ButcherImage
{
public:
    typedef deque<wxColour> transparentcolors_t;
    typedef deque<wxPoint> transparentpixels_t;
    enum format_t { FMT_JPG=0, FMT_GIF, FMT_PNG, FMT_BMP, FMT_TIFF, FMT_XPM };
    enum flags_t {
        GEN_NOFLAG = 0,
        JPG_QUAL_100 = 1,
        JPG_QUAL_75 = 2,
        JPG_QUAL_50 = 4,
        JPG_QUAL_25 = 8,
        JPG_QUAL_10 = 16,
        JPG_PROGRESSIVE = 32,
        GEN_QUANT_WU = 64,
        GEN_QUANT_NN = 128,
        GEN_FORCE8BIT = 256,
        GEN_FORCE16BIT = 512,
        GEN_FORCE24BIT = 1024,
        GEN_FORCE32BIT = 2048
    };
    enum repeat_t { DR_NONE, DR_HORIZONTAL, DR_VERTICAL, DR_ALL };

    struct saveparams_t {
        format_t format;
        int flags;
        transparentcolors_t *tcolors;
        transparentpixels_t *tpixels;
    };

    ButcherImage() {}
    virtual ~ButcherImage() {}

    virtual long GetWidth() = 0;
    virtual long GetHeight() = 0;
    wxRect GetImageRect() { return wxRect(0, 0, GetWidth(), GetHeight()); }

    virtual unsigned long GetFileSize() = 0;

    virtual ButcherImage *Clone() = 0;
    virtual ButcherImage *SubImage(const wxRect &r) = 0;
    ButcherImage *Save(const saveparams_t &saveparams) {
        return Save(saveparams.format, saveparams.flags, saveparams.tcolors,
            saveparams.tpixels);
    }
    virtual ButcherImage *Save(format_t format, int flags,
        transparentcolors_t *tcolors = NULL, transparentpixels_t *tpixels = NULL) = 0;

    virtual void Save(const saveparams_t &saveparams, const wxString &filename) {
        Save(saveparams.format, filename, saveparams.flags, saveparams.tcolors,
            saveparams.tpixels);
    }
    virtual void Save(format_t format, const wxString &filename, int flags,
        transparentcolors_t *tcolors = NULL, transparentpixels_t *tpixels = NULL) = 0;

    virtual void Draw(wxDC &dc, const wxRect &r) = 0;
    virtual void DrawRepeat(wxDC &dc, const wxSize &s, const wxRect &rarea, repeat_t repeat);
    virtual void Composite(const wxBitmap &bitmap) = 0;

    virtual wxColor GetPixelColor(unsigned x, unsigned y) = 0;

    static wxString GetFormatExt(format_t format);
    static format_t GetExtFormat(const wxString &ext);
protected:
    virtual bool CanLoad(const wxString &filename, bool doload = false) = 0;
    virtual void Load(const wxString &filename) = 0;
    virtual bool CanLoad(wxInputStream &stream, bool doload = false) = 0;
    virtual void Load(wxInputStream &stream) = 0;

    friend class ButcherImageFactory;
private:
};


/**
 * @class ButcherImageFactory
 *
 * @brief Image factory
 */
class ButcherImageFactory
{
public:
    static ButcherImage* Load(const wxString &filename);
    static ButcherImage* Load(wxInputStream &stream);
    static ButcherImage* Load(const wxImage &image);
};


#endif //__BIMAGE_H__
