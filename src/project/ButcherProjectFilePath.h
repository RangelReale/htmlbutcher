//=============================================================================
/**
 *  @file   ButcherProjectFilePath.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTFILEPATH_H__
#define __BPROJECT_BUTCHERPROJECTFILEPATH_H__

#include <wx/wx.h>
#include "BList.h"
#include "ButcherProjectBaseBLId.h"
#include "ButcherMetadataFile.h"
#include "ButcherProjectEvent.h"

/**
 * @class ButcherProjectFilePath
 *
 * @brief Project file path
 */
class ButcherProjectFilePath : public ButcherProjectBaseBLId {
public:
    ButcherProjectFilePath(ButcherProject *project, const wxString &name,
        const wxString &path = wxEmptyString);
    virtual ~ButcherProjectFilePath();

    const wxString &GetName() { return name_; }
    void SetName(const wxString &name) { name_=name; ProjectModified(); }

    const wxString &GetPath() { return path_; }
    void SetPath(const wxString &path) { path_=path; ProjectModified(); }

    static wxString GetFormattedPath(const wxString &path,
        const wxString &basepath, bool includetrailing = true);
    wxString GetFormattedPath(const wxString &basepath = wxEmptyString, bool includetrailing = true);
    static void ForcePathCreate(const wxString &path);
    static wxString GetValidFilename(const wxString &path);


    void LoadMetadata(ButcherMetadataData *metadata);
    void SaveMetadata(ButcherMetadataData *metadata);
protected:
    ButcherProjectFilePath(ButcherProject *project);

    virtual ButcherProjectEvent::event_t GetProjectModifiedItem() { return ButcherProjectEvent::BPE_FILEPATHMODIFIED; }

    void Set(const wxString &name, const wxString &path)
        { name_=name; path_=path; ProjectModified(); }

    friend class ButcherProjectFilePaths;
    friend class ButcherList<ButcherProjectFilePath>;
private:
    wxString name_, path_;
};

#endif // __BPROJECT_BUTCHERPROJECTFILEPATH_H__
