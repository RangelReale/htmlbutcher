//=============================================================================
/**
 *  @file   ButcherProjectAreaImageInfo.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTAREAIMAGEINFO_H__
#define __BPROJECT_BUTCHERPROJECTAREAIMAGEINFO_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseModify.h"
#include "ButcherOutputFile.h"
#include "BImage.h"
#include "ButcherProjectEventNotify.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectImageFormat.h"

class ButcherProjectAreaConfigBase;
class ButcherProjectImageFormat;

/**
 * @class ButcherProjectAreaImageInfo
 *
 * @brief Project area image info
 */
class ButcherProjectAreaImageInfo : public ButcherProjectBaseModify, public ButcherOutputFile
{
public:
    ButcherProjectAreaImageInfo(ButcherProjectAreaConfigBase *areaconfig, int infoid);

    bool GetEnabled() { return enabled_; }
    void SetEnabled(bool b) { enabled_=b; ProjectModified(); }

    BLID_t GetImageFormatId() { return imageformat_; }
    void SetImageFormatId(BLID_t imageformat) { imageformat_=imageformat; ProjectModified(); }

    ButcherProjectImageFormat *GetImageFormat();

    const wxString &GetFilename() { return filename_; }
    void SetFilename(const wxString &name) { filename_=name; ProjectModified(); }

    bool GetFilenamePrepend() { return filename_prepend_; }
    void SetFilenamePrepend(bool p) { filename_prepend_=p; ProjectModified(); }

    const ButcherImage::transparentcolors_t &TransparentColors() { return transparentcolors_; }
    ButcherImage::transparentcolors_t &TransparentColorsModify() { return transparentcolors_; ProjectModified(); }

    const ButcherImage::transparentpixels_t &TransparentPixels() { return transparentpixels_; }
    ButcherImage::transparentpixels_t &TransparentPixelsModify() { return transparentpixels_; ProjectModified(); }

    ButcherImage::saveparams_t GetSaveParams() {
        ButcherImage::saveparams_t ret;
        memset(&ret, 0, sizeof(ret));
        ret.format=GetImageFormat()->GetFormat();
        ret.flags=GetImageFormat()->GetFlags();
        ret.tcolors=&transparentcolors_;
        ret.tpixels=&transparentpixels_;
        return ret;
    }

    bool IsOutputImage();
	virtual void Copy(const ButcherProjectAreaImageInfo &other);

    virtual wxString GetOutputFileURL(BLID_t id = 0);
    virtual wxString GetOutputFilename(const wxString &basepath = wxEmptyString, BLID_t id = 0);
    virtual void SaveOutputFile(const wxString &filename, BLID_t id = 0);

    ButcherProjectAreaConfigBase *GetAreaConfig() { return areaconfig_; }
    int GetInfoId() { return infoid_; }
    void ResetAreaConfig() { areaconfig_=NULL; }

    virtual void ProjectEvent(ButcherProjectEventNotify &event);

    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
protected:
    virtual void SendProjectModified(ButcherProjectEvent::event_t mod, BLID_t id, BLID_t id2);
private:
    ButcherProjectAreaConfigBase *areaconfig_;
    int infoid_;
    bool enabled_;
    BLID_t imageformat_;
    wxString filename_;
    bool filename_prepend_;
    ButcherImage::transparentcolors_t transparentcolors_;
    ButcherImage::transparentpixels_t transparentpixels_;

    friend class ButcherProjectAreaConfigBase;
};


#endif // __BPROJECT_BUTCHERPROJECTAREAIMAGEINFO_H__
