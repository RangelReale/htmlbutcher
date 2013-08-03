//=============================================================================
/**
 *  @file   ButcherProjectFile.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTFILE_H__
#define __BPROJECT_BUTCHERPROJECTFILE_H__

#include <wx/wx.h>
#include <wx/file.h>
#include "BList.h"
#include "ButcherProjectBaseBLId.h"
#include "ButcherOutputFile.h"
#include "BImage.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"

/**
 * @class ButcherProjectFile
 *
 * @brief Project file
 */
class ButcherProjectFile : public ButcherProjectBaseBLId, public ButcherOutputFile {
public:
    ButcherProjectFile(ButcherProject *project, const wxString &name,
        const wxString &filename,
        unsigned long imagewidth, unsigned long imageheight);
    ButcherProjectFile(ButcherProject *project, const wxString &name,
        wxInputStream &filedata,
        unsigned long imagewidth, unsigned long imageheight);
    virtual ~ButcherProjectFile();

    const wxString &GetName() { return name_; }
    void SetName(const wxString &n) { name_=n; ProjectModified(); }

    const wxString &GetFilename() { return filename_; }
    void SetFilename(const wxString &n);

	void SetFileData(wxInputStream &imageloaddata);

    wxString GetDisplayName();

    unsigned long GetImageWidth() { return imagewidth_; }
    unsigned long GetImageHeight() { return imageheight_; }

    wxString GetPathFile();
    wxString GetPathDir();

    virtual wxString GetOutputFileURL(BLID_t id = 0);
    virtual wxString GetOutputFilename(const wxString &basepath = wxEmptyString, BLID_t id = 0);
    virtual void SaveOutputFile(const wxString &filename, BLID_t id = 0);

    ButcherImage *GetImage();
    ButcherImage *GetSubImage(const wxRect &r);
    ButcherImage *CreateImage();

    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
    void UpdateMetadata(ButcherMetadataData *metadata, bool reload = false);
protected:
    ButcherProjectFile(ButcherProject *project);

    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_FILEMODIFIED; }

    void Set(const wxString &name, const wxString &filename, unsigned long imagewidth,
        unsigned long imageheight);

    friend class ButcherProjectFiles;
    friend class ButcherList_2<ButcherProjectFile>;
private:
    wxString name_;
    wxString filename_;
    ButcherImage *image_;
    unsigned long imagewidth_, imageheight_;
    wxInputStream *imageloaddata_;
    ButcherMetadataDataItem_Binary *imagedata_;
    bool isloaderror_;
};

#endif // __BPROJECT_BUTCHERPROJECTFILE_H__
