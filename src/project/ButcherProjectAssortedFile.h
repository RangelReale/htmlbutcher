//=============================================================================
/**
 *  @file   ButcherProjectAssortedFile.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTASSORTEDFILE_H__
#define __BPROJECT_BUTCHERPROJECTASSORTEDFILE_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseBLId.h"
#include "ButcherOutputFile.h"
#include "BImage.h"
#include "ButcherProjectBaseIDList.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"

class ButcherProject;
class ButcherProjectAssortedFiles;

/**
 * @class ButcherProjectAssortedFile
 *
 * @brief Project assorted file
 */
class ButcherProjectAssortedFile : public ButcherProjectBaseBLId, public ButcherOutputFile {
public:
    enum filetype_t { AFT_UNKNOWN, AFT_IMAGE };

    ButcherProjectAssortedFile(ButcherProject *project, const wxString &filename);
    virtual ~ButcherProjectAssortedFile();

    const wxString &GetFilename() { return filename_; }
    void SetFilename(const wxString &f);

    filetype_t GetFileType() { return filetype_; }
    void SetFileType(filetype_t f) { filetype_=f; ProjectModified(); }

    BLID_t GetFilePath() { return filepath_; }
    void SetFilePath(BLID_t f) { filepath_=f; ProjectModified(); }

    const wxString &GetSaveFilename() { return savefilename_; }
    void SetSaveFilename(const wxString &s) { savefilename_=s; ProjectModified(); }

    virtual wxString GetOutputFileURL(BLID_t id = 0);
    virtual wxString GetOutputFilename(const wxString &basepath = wxEmptyString, BLID_t id = 0);
    virtual void SaveOutputFile(const wxString &filename, BLID_t id = 0);


    //wxString GetOutputFileURL();
    //wxString GetOutputFilename(const wxString &basepath = wxEmptyString);

    void SaveFile(const wxString &filename);
    void SaveFileToPath(const wxString &path);
    void SaveFile(wxOutputStream &ostream);

    bool IsImage() { return filetype_==AFT_IMAGE; }
    ButcherImage *CreateImage();

    wxString GetPathFile();
    wxString GetPathDir();

    ButcherProjectBaseIDList &Groups() { return groups_; }

    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
    void UpdateMetadata(ButcherMetadataData *metadata, bool reload = false);
protected:
    ButcherProjectAssortedFile(ButcherProject *project);

    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_ASSORTEDFILEMODIFIED; }

    friend class ButcherProjectAssortedFiles;
    friend class ButcherList<ButcherProjectAssortedFile>;
private:
    wxString filename_;
    ButcherProjectBaseIDList groups_;
    ButcherMetadataDataItem_Binary *filedata_;
    filetype_t filetype_;
    BLID_t filepath_;
    wxString savefilename_;
};

#endif // __BPROJECT_BUTCHERPROJECTASSORTEDFILE_H__
