//=============================================================================
/**
 *  @file   ButcherProjectCSSFile.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTCSSFILE_H__
#define __BPROJECT_BUTCHERPROJECTCSSFILE_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseBLId.h"
#include "ButcherOutputFile.h"
#include "ButcherProjectEventNotify.h"
#include "ButcherMetadataFile.h"

/**
 * @class ButcherProjectCSSFile
 *
 * @brief Project CSS file
 */
class ButcherProjectCSSFile : public ButcherProjectBaseBLId, public ButcherOutputFile {
public:
    ButcherProjectCSSFile(ButcherProject *project, const wxString &name);
    virtual ~ButcherProjectCSSFile();

    const wxString &GetName() { return name_; }
    void SetName(const wxString &n) { name_=n; ProjectModified(); }

    const wxString &GetCSSText() { return csstext_; }
    void SetCSSText(const wxString &c) { csstext_=c; ProjectModified(); }

    BLID_t GetFilePathId() { return filepathid_; }
    void SetFilePathId(BLID_t f) { filepathid_=f; ProjectModified(); }

    const wxString &GetSaveFilename() { return savefilename_; }
    void SetSaveFilename(const wxString &s) { savefilename_=s; ProjectModified(); }

    virtual wxString GetOutputFileURL(BLID_t id = 0);
    virtual wxString GetOutputFilename(const wxString &basepath = wxEmptyString, BLID_t id = 0);
    virtual void SaveOutputFile(const wxString &filename, BLID_t id = 0);

    void SaveFile(const wxString &filename);
    void SaveFileToPath(const wxString &path);
    void SaveFile(wxOutputStream &ostream);

    virtual void ProjectEvent(ButcherProjectEventNotify &event);

    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
protected:
    ButcherProjectCSSFile(ButcherProject *project);

    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_CSSFILEMODIFIED; }

    friend class ButcherProjectCSSFiles;
    friend class ButcherList<ButcherProjectCSSFile>;
private:
    wxString name_;
    wxString csstext_;
    BLID_t filepathid_;
    wxString savefilename_;
};

#endif // __BPROJECT_BUTCHERPROJECTCSSFILE_H__
